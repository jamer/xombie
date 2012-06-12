#include <cassert>

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "e.h"
#include "font.h"


enum TextQuality {
	SOLID,  // aliased font w/transparent background
	SHADED, // aliased font w/specified background
	BLENDED // anti-aliased font w/transparent
};

TTF_Font* font;


static void destroy()
{
	TTF_CloseFont(font);
}

TTF_Font* loadFont(char* file, int ptsize)
{
	TTF_Font* font;
	font = TTF_OpenFont(file, ptsize);
	if (font == NULL) {
		err(QString("unable to load font '%1': %2")
			.arg(file).arg(TTF_GetError()));
	}
	return font;
}

bool fontInit()
{
	if (TTF_Init() == -1)
		err(QString("unable to initialize SDL_ttf: %1")
			.arg(TTF_GetError()));

	font = loadFont((char*)"sans", 14);
	atexit(destroy);
	return true;
}

SDL_Surface* drawText(TTF_Font* fonttodraw, Uint8 fgR, Uint8 fgG, Uint8 fgB,
		Uint8 fgA, Uint8 bgR, Uint8 bgG, Uint8 bgB, Uint8 bgA, char text[],
		int /* TextQuality*/ quality)
{
	SDL_Color tmpfontcolor = {fgR, fgG, fgB, fgA};
	SDL_Color tmpfontbgcolor = {bgR, bgG, bgB, bgA};
	SDL_Surface* resulting_text = NULL;

	switch (quality) {
		case SOLID:
			resulting_text = TTF_RenderText_Solid(fonttodraw,
				text, tmpfontcolor);
			break;
		case SHADED:
			resulting_text = TTF_RenderText_Shaded(fonttodraw,
				text, tmpfontcolor, tmpfontbgcolor);
			break;
		case BLENDED:
			resulting_text = TTF_RenderText_Blended(fonttodraw,
				text, tmpfontcolor);
			break;
		default:
			assert(false);
	}

	return resulting_text;
}

SDL_Surface* renderFont(char* text)
{
	return drawText(font,
			0xdd, 0xdd, 0xdd, 0xff, /* grey text color */
			0x00, 0x00, 0x00, 0xff, /* black background color */
			text, SOLID);
}
