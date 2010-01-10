#include "engine.h"
#include "imgbase.h"
#include "item.h"
#include "random.h"

Item::Item()
		: dead(false)
{
	int x = randInt(100, getEngine()->getWidth () - 100);
	int y = randInt(100, getEngine()->getHeight() - 100);
	setLoc(x, y);
}

Item::~Item()
{
}

void Item::drawInv(SDL_Surface* screen, int xoff, int yoff)
{
	SDL_Rect rect;
	rect.w = invView->w;
	rect.h = invView->h;
	rect.x = xoff - rect.w / 2;
	rect.y = yoff - rect.h / 2;
	SDL_BlitSurface(invView, NULL, screen, &rect);
}


bool Item::isWeapon()
{
	return false;
}

bool Item::isDead()
{
	return dead;
}

