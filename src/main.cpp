#include <QFile>
#include <QString>

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

static SDL_Cursor* createCursor(SDL_Surface* surf)
{
	// The width must be a multiple of 8 (SDL requirement)

#ifdef __APPLE__
	size_t cursor_width = 16;
#else
	size_t cursor_width = surf->w;
	if ((cursor_width%8) != 0) {
		cursor_width += 8 - (cursor_width%8);
	}
#endif
	std::vector<Uint8> data((cursor_width*surf->h)/8,0);
	std::vector<Uint8> mask(data.size(),0);

	// See http://sdldoc.csn.ul.ie/sdlcreatecursor.php for documentation
	// on the format that data has to be in to pass to SDL_CreateCursor
	bool locked = false;
	if (SDL_MUSTLOCK(surf))
		locked = SDL_LockSurface(surf) == 0;
	const Uint32* const pixels = reinterpret_cast<Uint32*>(surf->pixels);
	for (int y = 0; y != surf->h; ++y) {
		for (int x = 0; x != surf->w; ++x) {

			if (static_cast<size_t>(x) < cursor_width) {
				Uint8 r,g,b,a;
				SDL_GetRGBA(pixels[y*surf->w + x],surf->format,&r,&g,&b,&a);

				const size_t index = y*cursor_width + x;
				const size_t shift = 7 - (index % 8);

				const Uint8 trans = (a < 128 ? 0 : 1) << shift;
				const Uint8 black = (trans == 0 || (r+g + b) / 3 > 128 ? 0 : 1) << shift;

				data[index/8] |= black;
				mask[index/8] |= trans;
			}
		}
	}
	SDL_UnlockSurface(surf);

	return SDL_CreateCursor(&data[0],&mask[0],cursor_width,surf->h,15,15);
}

/*
 * Loads an XPM file into an SDL_Cursor struct.
 * This is cheaply written and terribly implimented.
 * Can crash.
 */
static SDL_Cursor* loadCursor(QString filename)
{
	QFile file(filename);
	if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
		warn("Couldn't open cursor file " + filename);
		return NULL;
	}

	char* data = NULL, *mask = NULL; // XXX
	int i = 0;

	for (int i = 0; i < 2; i++)
		file.readLine();
	QString line = file.readLine();
	while (!line.isEmpty()) {
		for (int j = 0; j < line.length(); j++, i++)
			data[i] = mask[i] = line[j] == '1';
		line = file.readLine();
	}
	file.close();

	return NULL;
}


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

	// fullscreen?
	bool fullscreen = globals->getBool("Window", "Fullscreen", false);
	if (fullscreen)
		videoFlags |= SDL_FULLSCREEN;

	// Use OpenGL?
	bool opengl = globals->getBool("Graphics", "Use-GL", false);
	if (opengl) {
		printf("Using OpenGL! (not a good idea yet..)\n");
		videoFlags |= SDL_OPENGL;
	}
	else {
		// HWSURFACE should *not* be used with OpenGL
		videoFlags |= SDL_HWSURFACE;
		videoFlags |= SDL_DOUBLEBUF;
		printf("Using plain-old SDL for rendering....\n");
	}

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		err("Failed to initialize SDL");

	screen = SDL_SetVideoMode(width, height, depth, videoFlags);
	if (screen == NULL)
		err("Failed to initialize SDL window");

	if (opengl) {
		// enable double-buffering with OGL
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		/*
		// SDL doesn't trigger off a ResizeEvent at startup, but as we need
		// this for OpenGL, we do this ourselves
		SDL_Event resize_event;
		resize_event.type = SDL_VIDEORESIZE;
		resize_event.resize.w = width;
		resize_event.resize.h = height;

		SDL_PushEvent(&resize_event);
		*/
	}

	SDL_WM_SetCaption("Xombie", "Xombie");
	SetIcon();

	SDL_Surface* cursorImage = IMG_Load("gfx/cursor.png");
	SDL_Cursor* cursor = createCursor(cursorImage);
	SDL_SetCursor(cursor);

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

