#include "engine.h"
#include "imgbase.h"
#include "item.h"
#include "random.h"

Item::Item(World* w)
	: world(w), dead(false)
{
	SDL_Rect* rect = world->getBounds();
	int x = randInt(100, rect->w - 100);
	int y = randInt(100, rect->h - 100);

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

