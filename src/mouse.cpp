#include <QVarLengthArray>
#include <SDL_image.h>

#include "e.h"
#include "mouse.h"

Mouse::Mouse()
	: softwareCursor(false), cursorType(ARROW)
{
	arrow = SDL_GetCursor();
	crosshair = createCursor("gfx/crosshair.png");
}

Mouse::~Mouse()
{
	SDL_FreeCursor(crosshair);
}

void Mouse::setCursor(CursorType type)
{
	unsetCursor();

	switch (type) {
	case ARROW:
		SDL_SetCursor(arrow);
		break;
	case BLANK:
		SDL_ShowCursor(0);
		break;
	case CROSSHAIR:
		SDL_SetCursor(crosshair);
		break;
	case COLOR:
		break;
	}
}

void Mouse::unsetCursor()
{
	switch (cursorType) {
	case ARROW:
		break;
	case BLANK:
		SDL_ShowCursor(1);
		break;
	case CROSSHAIR:
		break;
	case COLOR:
		break;
	}
}

Vector Mouse::getPosition()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return Vector(x, y);
}

void Mouse::setPosition(SDL_Event& event)
{
	position.x = event.motion.x;
	position.y = event.motion.y;
}

SDL_Cursor* Mouse::createCursor(const char* filename)
{
	SDL_Surface* cursorImage = IMG_Load(filename);
	SDL_Cursor* cursor = createCursor(cursorImage);
	SDL_FreeSurface(cursorImage);
	return cursor;
}

SDL_Cursor* Mouse::createCursor(SDL_Surface* surf)
{
	// The width must be a multiple of 8 (SDL requirement)
	size_t cursorWidth = surf->w;
	if ((cursorWidth % 8) != 0) {
		cursorWidth += 8 - (cursorWidth % 8);
		warn("Cursor width must be a multiple of 8");
	}

	/*
	 * 32 x 32 pixels, 8 pixels per byte
	 */
	#define BASE_SIZE 32 * 32 / 8

	QVarLengthArray<Uint8, BASE_SIZE> data((cursorWidth * surf->h)/8);
	QVarLengthArray<Uint8, BASE_SIZE> mask(data.size());

	memset(data.data(), 0, data.size());
	memset(mask.data(), 0, mask.size());

	// See http://sdldoc.csn.ul.ie/sdlcreatecursor.php for documentation
	// on the format that data has to be in to pass to SDL_CreateCursor
	bool locked = false;
	if (SDL_MUSTLOCK(surf))
		locked = SDL_LockSurface(surf) == 0;
	const Uint32* const pixels = reinterpret_cast<Uint32*>(surf->pixels);
	for (int y = 0; y != surf->h; y++) {
		for (int x = 0; x != surf->w; x++) {
			if (static_cast<size_t>(x) < cursorWidth) {
				Uint8 r,g,b,a;
				SDL_GetRGBA(pixels[y*surf->w + x], surf->format,
						&r, &g, &b, &a);

				const size_t index = y*cursorWidth + x;
				const size_t shift = 7 - (index % 8);

				const Uint8 trans = (a > 127) << shift;
				const Uint8 black = (trans == 0 ||
						(r+g+b) / 3 > 127 ? 0 : 1)
						<< shift;

				data[index/8] |= black;
				mask[index/8] |= trans;
			}
		}
	}
	if (locked)
		SDL_UnlockSurface(surf);

	return SDL_CreateCursor(data.data(), mask.data(),
			cursorWidth, surf->h, 15, 15);
}

