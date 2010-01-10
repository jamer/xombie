#ifndef _INVENTORYVIEW_H_
#define _INVENTORYVIEW_H_

#include <SDL.h>

#include "view.h"

class InventoryView : public View
{
public:
	InventoryView();
	void draw();
	void drawItems(SDL_Surface* screen);
	void doKeyDown(int key);

private:
	bool needsDraw;

	SDL_Rect wndSz;
	SDL_Rect innerBrdr;
};

#endif // _INVENTORYVIEW_H_

