#include <Arduino.h>
#include <HID.h>

#include "config.hpp"
#include "description.hpp"

struct Report {
    uint8_t modifiers;
    uint8_t consumer;
    Key keys[ROLLOVER];
};

Report report = {0};
bool fnLayer = false;
bool states[ROW_COUNT][COL_COUNT] = {0};
unsigned long times[ROW_COUNT][COL_COUNT] = {0};

void addKey(Key key) {
    if (key >= LCTRL)
        report.modifiers |= (1 << ((uint8_t)key - (uint8_t)LCTRL));

    for (int i = 0; i < ROLLOVER; i++) {
        if (report.keys[i] == NONE) {
            report.keys[i] = key;
            break;
        }
    }
}

void removeKey(Key key) {
    if (key >= LCTRL)
        report.modifiers &= ~(1 << ((uint8_t)key - (uint8_t)LCTRL));

    for (int i = 0; i < ROLLOVER; i++) {
        if (report.keys[i] == key) {
            report.keys[i] = NONE;
        }
    }
}

void update(unsigned char row, unsigned char col, bool state) {
    if (states[row][col] != state && millis() > times[row][col]) {
        states[row][col] = state;
        times[row][col] = millis() + REPEAT_TIME;
        
        Key key = (fnLayer? LAYOUT_FN : LAYOUT_BASE)[row][col];

        if (key == FN) {
            fnLayer = state;
            memset(&report, 0, sizeof(report));
        } else {
            (state? addKey : removeKey)(key);
        }

        HID().SendReport(2, &report, sizeof(report));
    }
}

void setup() {
    for (int i = 0; i < COL_COUNT; i++)
        pinMode(COL_PINS[i], INPUT_PULLUP);

    HID().begin();
    HID().AppendDescriptor(&descriptor);
}

void loop() {
    for (int row = 0; row < ROW_COUNT; row++) {
        pinMode(ROW_PINS[row], OUTPUT);
        digitalWrite(ROW_PINS[row], LOW);

        for (int col = 0; col < COL_COUNT; col++)
            update(row, col, !digitalRead(COL_PINS[col]));

        pinMode(ROW_PINS[row], INPUT);
    }
}