#pragma once
#include <SDL/SDL_keyboard.h>

typedef bool (*key_handle_func)(SDL_Scancode key, bool pressed, bool repeat);
typedef bool (*char_handle_func)(char chr);