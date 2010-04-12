#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

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


// Default video flags
int videoFlags;
SDL_Surface* screen;
SDL_Surface* icon;

SDL_Surface* getWindowIcon()
{
	return icon;
}

void SetIcon()
{
	unsigned int colorkey;

	icon = IMG_Load(globals->getString("Window", "Icon", "gfx/icon.jpg"));
	colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
	SDL_WM_SetIcon(icon, NULL);
}


SDL_Surface* InitScreen()
{
	SDL_Surface* screen;

	int width = globals->getInt("Window", "Width", 640);
	int height = globals->getInt("Window", "Height", 480);
	int depth = globals->getInt("Window", "Color depth", 32);

	// fullscreen?
	bool fullscreen = globals->getBool("Window", "Fullscreen", false);
	if (fullscreen)
		videoFlags |= SDL_FULLSCREEN;

	// Use OpenGL?
	bool opengl = globals->getBool( "Graphics", "Use-GL", false );
	if (opengl) {
		printf( "Using OpenGL! (not a good idea yet..)\n" );
		videoFlags |= SDL_OPENGL;
	}
	else {
		// HWSURFACE should *not* be used with OpenGL
		videoFlags |= SDL_HWSURFACE;
		videoFlags |= SDL_DOUBLEBUF;
		printf( "Using plain-old SDL for rendering....\n" );
	}

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		err(1, "Failed to initialize SDL");

	screen = SDL_SetVideoMode(width, height, depth, videoFlags);
	if (screen == NULL)
		err(1, "Failed to initialize SDL window");

	if (opengl) {
		// enable double-buffering with OGL
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// SDL doesn't trigger off a ResizeEvent at startup, but as we need
		// this for OpenGL, we do this ourselves
		SDL_Event resize_event;
		resize_event.type = SDL_VIDEORESIZE;
		resize_event.resize.w = width;
		resize_event.resize.h = height;

		SDL_PushEvent(&resize_event);
	}

	SDL_WM_SetCaption("Xombie", "Xombie");
	SetIcon();

	printf("Created a %ix%i %i-bit window\n", width, height, depth);
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

