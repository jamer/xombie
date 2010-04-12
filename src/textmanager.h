#ifndef _TEXTMANAGER_H_
#define _TEXTMANAGER_H_

#include <SDL.h>
#include <SDL_ttf.h>

class TextManager
{
public:
	TextManager();
	~TextManager();
	
	SDL_Surface* renderString(char* text);

private:
	TTF_Font* loadFont(char* file, int ptsize);

	TTF_Font* font;

};

#endif

