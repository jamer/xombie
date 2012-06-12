#include <QFile>
#include <QString>
#include <QVarLengthArray>

#include <list>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "audio.h"
#include "common.h"
#include "conf.h"
#include "e.h"
#include "engine.h"
#include "font.h"
#include "imgbase.h"
#include "invview.h"
#include "random.h"
#include "worldview.h"

int videoFlags;
SDL_Surface* screen;
SDL_Surface* icon;


SDL_Surface* getWindowIcon()
{
	return icon;
}

void SetIcon()
{
//	unsigned int colorkey;
	QString iconFile = globals->getString("Window", "Icon", NULL);
	icon = IMG_Load(globals->getCString("Window", "Icon", iconFile));
//	colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
//	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
	SDL_WM_SetIcon(icon, NULL);
}


SDL_Surface* InitScreen()
{
	SDL_Surface* screen;

	int width = globals->getInt("Window", "Width", 640);
	int height = globals->getInt("Window", "Height", 480);
	int depth = globals->getInt("Window", "Color depth", 32);

	bool fullscreen = globals->getBool("Window", "Fullscreen", false);
	if (fullscreen)
		videoFlags |= SDL_FULLSCREEN;

	videoFlags |= SDL_HWSURFACE;
	videoFlags |= SDL_DOUBLEBUF;

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		err("Failed to initialize SDL");

	screen = SDL_SetVideoMode(width, height, depth, videoFlags);
	if (screen == NULL)
		err("Failed to initialize SDL window");

	SDL_WM_SetCaption("Xombie", "Xombie");
	SetIcon();

	return screen;
}

void InitEverything()
{
	globals = new Conf("conf/game.conf");

	new Audio();
	new ImgBase();
	InitRand();
	InitFont();

	screen = InitScreen();

	SDL_WM_SetCaption("Xombie", "Xombie");
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int, char* [])
#endif
{
	InitEverything();
	new Engine(screen);
	Quit();

	return 0;
}
