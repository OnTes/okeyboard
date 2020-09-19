#include <Arduino.h>
#include <HID.h>

// Size of the keyboard
#define ROW_COUNT 4
#define COL_COUNT 12

// Maximum number of keys pressed at once
#define ROLLOVER 6

// Delay between pressing and releasing a key
#define REPEAT_TIME 50

// Pinout
const uint8_t ROW_PINS[ROW_COUNT] = { 21, 7, 16, 8 };
const uint8_t COL_PINS[COL_COUNT] = { 20, 19, 18, 15, 14, 10, 9, 6, 5, 4, 3, 2 };

enum Key : unsigned char
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

// Position of FN key
#define FN_KEY_ROW 3
#define FN_KEY_COL 4

// Layout of base layer
const Key LAYOUT_BASE[ROW_COUNT][COL_COUNT] =
{
    { ESC, Q, W, E, R, T, Y, U, I, O, P, DELETE },
    { TAB, A, S, D, F, G, H, J, K, L, SEMICOLON, BACKSPACE },
    { LSHIFT, Z, X, C, V, B, N, M, COMMA, PERIOD, UP, ENTER },
    { LCTRL, LGUI, LALT, RALT, NONE, SPACE, SPACE, SLASH, APOSTROPHE, LEFT, DOWN, RIGHT }
};

// Layout of FN layer
const Key LAYOUT_FN[ROW_COUNT][COL_COUNT] = 
{
    { ESC, N1, N2, N3, N4, N5, N6, N7, N8, N9, N0, DELETE },
    { TAB, F1, F2, F3, F4, F5, F6, MINUS, EQUAL, LEFTBRACE, RIGHTBRACE, BACKSPACE },
    { LSHIFT, F7, F8, F9, F10, F11, F12, MUTE, VOLUMEDOWN, VOLUMEUP, PAGEUP, ENTER },
    { LCTRL, LGUI, LALT, RALT, NONE, SPACE, SPACE, BACKSLASH, GRAVE, HOME, PAGEDOWN, END }
};

bool keyState[ROW_COUNT][COL_COUNT] = {0};
unsigned long keyTime[ROW_COUNT][COL_COUNT] = {0};

struct Report
{
    uint8_t modifiers;
    uint8_t consumer;
    Key keys[ROLLOVER];
};

Report report;
unsigned char reportKeyCount = 0;

const uint8_t description[] PROGMEM =
{
    0x05, 0x01,         // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,         // USAGE (Keyboard)
    0xA1, 0x01,         // COLLECTION (Application)
    0x85, 0x02,	        // REPORT_ID (2)

    0x05, 0x07,         // USAGE_PAGE (Keyboard)
    0x19, 0xE0,         // USAGE_MINIMUM (LCTRL)
    0x29, 0xE7,         // USAGE_MAXIMUM (RGUI)
    0x15, 0x00,         // LOGICAL_MINIMUM (0)
    0x25, 0x01,         // LOGICAL_MAXIMUM (1)
    0x75, 0x01,         // REPORT_SIZE (1)
    0x95, 0x08,         // REPORT_COUNT (8)
    0x81, 0x02,         // INPUT (Data,Var,Abs)

    0x05, 0x0C,         // USAGE_PAGE (Consumer)
    0x95, 0x01,         // REPORT_COUNT (1)
    0x75, 0x08,         // REPORT_SIZE (8)
    0x15, 0x00,         // LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,   // LOGICAL_MAXIMUM (255)
    0x19, 0x00,         // USAGE_MINIMUM (0)
    0x29, 0xFF,         // USAGE_MAXIMUM (255)
    0x81, 0x00,         // INPUT (Data,Ary,Abs)

    0x05, 0x07,         // USAGE_PAGE (Keyboard)
    0x95, ROLLOVER,     // REPORT_COUNT (ROLLOVER)
    0x75, 0x08,         // REPORT_SIZE (8)
    0x15, 0x00,         // LOGICAL_MINIMUM (0)
    0x26, 0xE7, 0x00,   // LOGICAL_MAXIMUM (231)
    0x19, 0x00,         // USAGE_MINIMUM (NULL)
    0x29, 0xE7,         // USAGE_MAXIMUM (RGUI)
    0x81, 0x00,         // INPUT (Data,Ary,Abs)

    0xC0                // END_COLLECTION
};

HIDSubDescriptor descriptor(description, sizeof(description));

void addKeyToReport(Key key)
{
    if (key == NONE)
        return;

    // Modifier key
    if (key >= LCTRL)
        report.modifiers |= (1 << ((uint8_t)key - (uint8_t)LCTRL));
    
    if (reportKeyCount >= ROLLOVER)
        return;
    
    // Regular key
    report.keys[reportKeyCount] = key;
    reportKeyCount++;
}

void setup()
{
    // Setup input
    for (unsigned char i = 0; i < COL_COUNT; i++)
        pinMode(COL_PINS[i], INPUT_PULLUP);

    // Setup output
    HID().begin();
    HID().AppendDescriptor(&descriptor);
}

void loop()
{
    // Clear the report
    memset(&report, 0, sizeof(Report));
    reportKeyCount = 0;

    // Iterate trough all the keys
    for (unsigned char row = 0; row < ROW_COUNT; row++)
    {
        pinMode(ROW_PINS[row], OUTPUT);
        digitalWrite(ROW_PINS[row], LOW);

        for (unsigned char col = 0; col < COL_COUNT; col++)
        {
            // Update the status of the key
            // NOTE: digitalRead() returns oposite value than what we want
            if (keyState[row][col] == digitalRead(COL_PINS[col])
                && millis() > keyTime[row][col])
            {
                keyState[row][col] = !keyState[row][col];
                keyTime[row][col] = millis() + REPEAT_TIME;
            }

            // Add the key to the report if pressed
            if (keyState[row][col])
            {
                if (!keyState[FN_KEY_ROW][FN_KEY_COL])
                    addKeyToReport(LAYOUT_BASE[row][col]);
                else
                    addKeyToReport(LAYOUT_FN[row][col]);
            }
        }

        pinMode(ROW_PINS[row], INPUT);
    }
    
    // Send the report
    HID().SendReport(2, &report, sizeof(Report));
}