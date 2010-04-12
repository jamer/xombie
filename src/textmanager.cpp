#include "textmanager.h"

TextManager::TextManager()
{
	if (TTF_Init() == -1) {
		fprintf(stderr, "Error: unable to initialize SDL_ttf, %s\n",
			TTF_GetError());
		return false;
	}

	font = loadFont((char*)"LiberationSans-Regular.ttf", 14);
}

TTF_Font* TextManager::loadFont(char* file, int ptsize)
{
	TTF_Font* font;
	font = TTF_OpenFont(file, ptsize);
	if (font == NULL) {
		fprintf(stderr, "Unable to load font '%s': %s\n", file,
			TTF_GetError());
		exit(0);
	}
	return font;
}

