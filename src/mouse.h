#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <SDL.h>

#include "vector.h"

enum CursorType
{
	ARROW,
	BLANK,
	CROSSHAIR,
	COLOR,
};

class Mouse
{
public:
	Mouse();
	~Mouse();

	void setCursor(CursorType type);

	Vector getPosition();
	void setPosition(SDL_Event& event);

private:
	void unsetCursor();
	SDL_Cursor* createCursor(const char* filename);
	SDL_Cursor* createCursor(SDL_Surface* surface);

	bool softwareCursor;
	Vector position;
	CursorType cursorType; 

	SDL_Cursor* arrow;
	SDL_Cursor* crosshair;
};

#endif
