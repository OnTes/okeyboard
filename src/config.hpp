#pragma once

#include "key.hpp"

// size of the keyboard
#define ROW_COUNT 4
#define COL_COUNT 12

// minimal time (in ms) between pressing and releasing a key
#define REPEAT_TIME 50

// pinout
const unsigned char ROW_PINS[ROW_COUNT] = { 21, 7, 16, 8 };
const unsigned char COL_PINS[COL_COUNT] = { 20, 19, 18, 15, 14, 10, 9, 6, 5, 4, 3, 2 };

// layout of base layer
const Key LAYOUT_BASE[ROW_COUNT][COL_COUNT] = {
    { ESC, Q, W, E, R, T, Y, U, I, O, P, DELETE },
    { TAB, A, S, D, F, G, H, J, K, L, SEMICOLON, BACKSPACE },
    { LSHIFT, Z, X, C, V, B, N, M, COMMA, PERIOD, UP, ENTER },
    { LCTRL, LGUI, LALT, RALT, FN, SPACE, SPACE, SLASH, APOSTROPHE, LEFT, DOWN, RIGHT }
};

// layout of FN layer
const Key LAYOUT_FN[ROW_COUNT][COL_COUNT] = {
    { ESC, N1, N2, N3, N4, N5, N6, N7, N8, N9, N0, DELETE },
    { TAB, F1, F2, F3, F4, F5, F6, MINUS, EQUAL, LEFTBRACE, RIGHTBRACE, BACKSPACE },
    { LSHIFT, F7, F8, F9, F10, F11, F12, MUTE, VOLUMEDOWN, VOLUMEUP, PAGEUP, ENTER },
    { LCTRL, LGUI, LALT, RALT, FN, SPACE, SPACE, BACKSLASH, GRAVE, HOME, PAGEDOWN, END }
};