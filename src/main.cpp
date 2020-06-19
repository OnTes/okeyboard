#include "input.hpp"
#include "output.hpp"

namespace Layout
{
    using namespace Output;

    // Position of FN key
    static const unsigned char fnKeyRow = 3, fnKeyCol = 4;

    static const Key base[ROW_COUNT][COLUMN_COUNT] =
    {
        { Key::ESC, Key::Q, Key::W, Key::E, Key::R, Key::T, Key::Y, Key::U, Key::I, Key::O, Key::P, Key::DELETE },
        { Key::TAB, Key::A, Key::S, Key::D, Key::F, Key::G, Key::H, Key::J, Key::K, Key::L, Key::SEMICOLON, Key::BACKSPACE },
        { Key::LSHIFT, Key::Z, Key::X, Key::C, Key::V, Key::B, Key::N, Key::M, Key::COMMA, Key::PERIOD, Key::UP, Key::ENTER },
        { Key::LCTRL, Key::LGUI, Key::LALT, Key::RALT, Key::NONE, Key::SPACE, Key::SPACE, Key::SLASH, Key::APOSTROPHE, Key::LEFT, Key::DOWN, Key::RIGHT }
    };
    static const Key fn[ROW_COUNT][COLUMN_COUNT] = 
    {
        { Key::ESC, Key::N1, Key::N2, Key::N3, Key::N4, Key::N5, Key::N6, Key::N7, Key::N8, Key::N9, Key::N0, Key::DELETE },
        { Key::TAB, Key::F1, Key::F2, Key::F3, Key::F4, Key::F5, Key::F6, Key::MINUS, Key::EQUAL, Key::LEFTBRACE, Key::RIGHTBRACE, Key::BACKSPACE },
        { Key::LSHIFT, Key::F7, Key::F8, Key::F9, Key::F10, Key::F11, Key::F12, Key::MUTE, Key::VOLUMEDOWN, Key::VOLUMEUP, Key::PAGEUP, Key::ENTER },
        { Key::LCTRL, Key::LGUI, Key::LALT, Key::RALT, Key::NONE, Key::SPACE, Key::SPACE, Key::BACKSLASH, Key::GRAVE, Key::HOME, Key::PAGEDOWN, Key::END}
    };
}

void setup()
{
    Serial.begin(9600);
    Input::begin();
    Output::begin();
}

bool fnState;

void loop()
{
    Input::update();

    fnState = Input::keys[Layout::fnKeyRow][Layout::fnKeyCol];

    for (unsigned char row = 0; row < ROW_COUNT; row++)
        for (unsigned char col = 0; col < COLUMN_COUNT; col++)
        {
            if (Input::keys[row][col])
            {
                if (!fnState)
                    Output::add(Layout::base[row][col]);
                else
                    Output::add(Layout::fn[row][col]);
            }
        }
    
    Output::send();
}