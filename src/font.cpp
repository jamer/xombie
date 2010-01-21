#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "font.h"


enum TextQuality {
	SOLID,  // aliased font w/transparent background
	SHADED, // aliased font w/specified background
	BLENDED // anti-aliased font w/transparent
};

// Function prototypes
TTF_Font* loadFont(char* file, int ptsize);
SDL_Surface* drawText(TTF_Font* fonttodraw, Uint8 fgR, Uint8 fgG, Uint8 fgB,
		Uint8 fgA, Uint8 bgR, Uint8 bgG, Uint8 bgB, Uint8 bgA, char text[],
		int /* TextQuality*/ quality);


// Private variables
TTF_Font* font;




//
// Functions
//
bool InitFont()
{
	if (TTF_Init() == -1) {
		fprintf(stderr, "Error: unable to initialize SDL_ttf, %s\n",
			TTF_GetError());
		return false;
	}

	font = loadFont((char*)"LiberationSans-Regular.ttf", 14);
	return true;
}


TTF_Font* loadFont(char* file, int ptsize)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, ptsize);
	if (tmpfont == NULL) {
		fprintf(stderr, "Unable to load font '%s': %s\n", file,
			TTF_GetError());
		exit(0);
	}
	return tmpfont;
}



SDL_Surface* renderFont(char* text)
{
	return drawText(font,
			0xdd, 0xdd, 0xdd, 0xff, /* grey text color */
			0x00, 0x00, 0x00, 0xff, /* black background color */
			text, SOLID);
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
			fprintf(stderr, "Bad argument to drawtext call\n");
	}

	return resulting_text;
}

