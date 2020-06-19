#pragma once

#include <Arduino.h>

#define ROW_COUNT 4
#define COLUMN_COUNT 12

namespace Input
{
    // Pinout
    static const uint8_t rows[ROW_COUNT] = { 21, 7, 16, 8 };
    static const uint8_t columns[COLUMN_COUNT] = { 20, 19, 18, 15, 14, 10, 9, 6, 5, 4, 3, 2 };

    bool keys[ROW_COUNT][COLUMN_COUNT];

    void begin()
    {
        for (unsigned char i = 0; i < COLUMN_COUNT; i++)
            pinMode(columns[i], INPUT_PULLUP);
    }

    void update()
    {   
        for (unsigned char row = 0; row < ROW_COUNT; row++)
        {
            pinMode(rows[row], OUTPUT);
            digitalWrite(rows[row], LOW);

            for (unsigned char col = 0; col < COLUMN_COUNT; col++)
                keys[row][col] = !digitalRead(columns[col]);

            pinMode(rows[row], INPUT);
        }
    }
}