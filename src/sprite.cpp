#include <math.h>
#include <stdio.h>

#include "common.h"
#include "engine.h"
#include "imgbase.h"
#include "sprite.h"

Sprite::Sprite()
	: speed(0.0)
{
	loc.w = loc.h = loc.x = loc.y = 0;
}

Sprite::~Sprite()
{
}

void Sprite::move(long dt)
{
	orient += speed / 1000 * dt;
	loc.x = (Sint16)orient.getLocation().x;
	loc.y = (Sint16)orient.getLocation().y;
}

void Sprite::move(long dt, Angle angle)
{
	real dx = cos(-angle) * speed * dt / 1000.0;
	real dy = sin(-angle) * speed * dt / 1000.0;

	orient.translate(Vector(dx, dy)); // XXX: add Orientation::translate(real x, real y)

	loc.x = (Sint16)orient.getLocation().x;
	loc.y = (Sint16)orient.getLocation().y;
}

void Sprite::stayOnScreen() // XXX: terrible
{
	Vector loc = orient.getLocation();
	real x = loc.x;
	real y = loc.y;

	x = max(x, gfx->w / 2 + 0.001);
	x = min(x, getEngine()->getWidth()  - gfx->w / 2 - 0.001);

	y = max(y, gfx->h / 2 + 0.001);
	y = min(y, getEngine()->getHeight() - gfx->h / 2 - 0.001);

	orient.setLocation(Vector(x, y)); // XXX: add Orientation::setLocation(real x, real y)
}

void Sprite::draw(SDL_Surface* screen)
{
	if (!isOnScreen())
		return;

	SDL_Rect* rect = getDispLoc();
	SDL_BlitSurface(getGraphic(), NULL, screen, rect);
}

void Sprite::setGraphicId(QString id)
{
	// XXX redo the image caching

	gfxId = id;
//	gfxHash = hash(gfxId);

	// grab the original size
	gfx = getImgBase()->getImage(gfxId, 0, true); // !
	origsz.w = gfx->w;
	origsz.h = gfx->h;
}

void Sprite::setAngle(Angle angle)
{
	ImgBase* base = getImgBase();
	gfx = base->getImage(gfxId, base->indexFromAngle(angle), true); // !
	orient.setAngle(-angle);
}

void Sprite::setAngleFromXY(real x, real y)
{
	Angle angle = 0.0;

	// Moving at an angle
	if (x != 0.0 && y != 0.0) {
		angle = atan(y / x);
		if (y < 0.0 && x < 0.0)
			;
		else if (y < 0.0 && x > 0.0)
			angle += M_PI;
		else if (y > 0.0 && x < 0.0)
			angle += M_PI*2;
		else if (y > 0.0 && x > 0.0)
			angle += M_PI;
	}

	// Moving straight
	else {
		if (x < 0.0)
			angle = 0.0;
		else if (x > 0.0)
			angle = M_PI;
		else if (y < 0.0)
			angle = M_PI_2;
		else if (y > 0.0)
			angle = 3*M_PI_2;
	}

	setAngle(angle);
}

void Sprite::setSpeed(real spd)
{
	speed = spd;
}

Angle Sprite::getAngle()
{
	return -orient.getAngle();
}

Orientation& Sprite::getOrientation()
{
	return orient;
}

SDL_Surface* Sprite::getGraphic()
{
	return gfx;
}

SDL_Rect* Sprite::getLoc()
{
	return &loc;
}

SDL_Rect* Sprite::getDispLoc()
{
	tmp.w = gfx->w;
	tmp.h = gfx->h;
	tmp.x = loc.x - gfx->w / 2;
	tmp.y = loc.y - gfx->h / 2;

	return &tmp;
}

SDL_Rect* Sprite::getBoundaries()
{
	tmp.w = origsz.w;
	tmp.h = origsz.h;
	tmp.x = loc.x - origsz.w / 2;
	tmp.y = loc.y - origsz.h / 2;
	return &tmp;
}

void Sprite::setLoc(short x, short y)
{
	orient.setLocation(Vector(x, y));
	loc.x = x;
	loc.y = y;
}

bool Sprite::isOnScreen()
{
	Vector loc = orient.getLocation();
	real x = loc.x;
	real y = loc.y;

	Engine* engine = getEngine();

	short hw = gfx->w / 2;
	short hh = gfx->h / 2;

	if (0 < x + hw ||
	        x - hw < engine->getWidth() ||
	    0 < y + hh ||
	        y - hh < engine->getHeight())
		return true;
	return false;
}

bool Sprite::isCompletelyOnScreen()
{
	Vector loc = orient.getLocation();
	real x = loc.x;
	real y = loc.y;

	Engine* engine = getEngine();

	if (0 <= x - gfx->w/2 &&
	         x + gfx->w/2 <= engine->getWidth())

		if (0 <= y - gfx->h/2 &&
		         y + gfx->h/2 <= engine->getHeight())

			return true;
	return false;
}

