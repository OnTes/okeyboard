#pragma once

#include <HID.h>

#define ROLLOVER 6

namespace Output
{
    enum class Key : unsigned char 
    {
        NONE = 0x00,

        A = 0x04, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        N1, N2, N3, N4, N5, N6, N7, N8, N9, N0,
        ENTER, ESC, BACKSPACE, TAB, SPACE,
        MINUS, EQUAL, LEFTBRACE, RIGHTBRACE, BACKSLASH,
        SEMICOLON = 0x33, APOSTROPHE, GRAVE, COMMA, PERIOD, SLASH,
        CAPSLOCK, 
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        PRINT, SCROLLLOCK, PAUSE, INSERT, HOME, PAGEUP, DELETE, END, PAGEDOWN,
        RIGHT, LEFT, DOWN, UP,

        NUMLOCK, KPSLASH, KPASTERISK, KPMINUS, KPPLUS, KPENTER,
        KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9, KP0, KPDOT,

        APPLICATION = 0x65, POWER, KPEQUAL,
        F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
        EXECUTE, HELP, MENU, SELECT, STOP, REDO, UNDO, CUT, COPY, PASTE, FIND,
        MUTE, VOLUMEUP, VOLUMEDOWN, 

        LCTRL = 0xE0, LSHIFT, LALT, LGUI,
        RCTRL, RSHIFT, RALT, RGUI
    };

    struct Report
    {
        uint8_t modifiers;
        uint8_t consumer;
        Key keys[ROLLOVER];
    };

    static Report report;
    static unsigned char count = 0;

    static const uint8_t description[] PROGMEM =
    {
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x06, // USAGE (Keyboard)
        0xA1, 0x01, // COLLECTION (Application)
        0x85, 0x02,	// REPORT_ID (2)

        // Modifier keys
        0x05, 0x07, // USAGE_PAGE (Keyboard)
        0x95, 0x08, // REPORT_COUNT (8)
        0x75, 0x01, // REPORT_SIZE (1)
        0x15, 0x00, // LOGICAL_MINIMUM (0)
        0x25, 0x01, // LOGICAL_MAXIMUM (1)
        0x19, 0xE0, // USAGE_MINIMUM (LCTRL)
        0x29, 0xE7, // USAGE_MAXIMUM (RGUI)
        0x81, 0x02, // INPUT (Data,Var,Abs)

        // Consumer keys
        0x05, 0x0C, // USAGE_PAGE (Consumer)
        0x95, 0x01, // REPORT_COUNT (1)
        0x75, 0x08, // REPORT_SIZE (8)
        0x15, 0x00, // LOGICAL_MINIMUM (0)
        0x26, 0xFF, // LOGICAL_MAXIMUM (255)
        0x19, 0x00, // USAGE_MINIMUM (0)
        0x29, 0xFF, // USAGE_MAXIMUM (255)
        0x81, 0x00, // INPUT (Data,Ary,Abs)

        // Regular keys
        0x05, 0x07, // USAGE_PAGE (Keyboard)
        0x95, ROLLOVER, // REPORT_COUNT (ROLLOVER)
        0x75, 0x08, // REPORT_SIZE (8)
        0x15, 0x00, // LOGICAL_MINIMUM (0)
        0x26, 0xE7, // LOGICAL_MAXIMUM (231)
        0x19, 0x00, // USAGE_MINIMUM (NULL)
        0x29, 0xE7, // USAGE_MAXIMUM (RGUI)
        0x81, 0x00, // INPUT (Data,Ary,Abs)

        0xC0 // END_COLLECTION
    };
    
    static HIDSubDescriptor descriptor(description, sizeof(description));

    void begin()
    {
        HID().begin();
	    HID().AppendDescriptor(&descriptor);
    }
    
    bool add(Key key)
    {
        if (key == Key::NONE)
            return true;

        // Modifier key
        if (key >= Key::LCTRL)
            report.modifiers |= (1 << ((uint8_t)key - (uint8_t)Key::LCTRL));
        
        if (count >= ROLLOVER)
            return false;
        
        // Regular key
        report.keys[count] = key;
        count++;
        return true;
    }

    void send()
    {
        HID().SendReport(2, &report, sizeof(Report));

        // Clear the report
        report.modifiers = 0;
        report.consumer = 0;
        for (unsigned int i = 0; i < ROLLOVER; i++)
            report.keys[i] = Key::NONE;
        count = 0;
    }
}