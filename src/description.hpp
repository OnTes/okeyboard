#pragma once

#include <HID.h>

// maximum number of keys pressed at once
#define ROLLOVER 6

const uint8_t description[] PROGMEM = {
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