#ifndef _FONT_H_
#define _FONT_H_

#include <SDL.h>

bool fontInit();

// draws grey text
SDL_Surface* renderFont(char* text);

#endif // _FONT_H_
