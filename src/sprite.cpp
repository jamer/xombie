#include <math.h>
#include <stdio.h>

#include "common.h"
#include "engine.h"
#include "imgbase.h"
#include "sprite.h"

Sprite::Sprite()
	: angle(0.0), speed(0.0), x(0), y(0)
{
	loc.w = loc.h = loc.x = loc.y = 0;
	gfxId[0] = '\0';
}

Sprite::~Sprite()
{
	
}

////////////////////
// functions
void Sprite::move(long dt)
{
	move(dt, angle);
}

void Sprite::move(long dt, double angle)
{
	double dx =  cos(angle) * speed * dt / 1000.0;
	double dy = -sin(angle) * speed * dt / 1000.0;

	x = (double)x + (double)dx;
	y = (double)y + (double)dy;

	loc.x = (Sint16)x;
	loc.y = (Sint16)y;
}

void Sprite::stayOnScreen()
{
	x = max(x, gfx->w / 2 + 0.001);
	x = min(x, getEngine()->getWidth()  - gfx->w / 2 - 0.001);

	y = max(y, gfx->h / 2 + 0.001);
	y = min(y, getEngine()->getHeight() - gfx->h / 2 - 0.001);
}

void Sprite::draw(SDL_Surface* screen)
{
#if DEBUG
	if (gfxId[0] == '\0')
		throw "Sprite::draw() - invalid gfxId";
#endif
	
	if (!isOnScreen())
		return;

	SDL_Rect* rect = getDispLoc();
	SDL_BlitSurface(getGraphic(), NULL, screen, rect);
}

////////////////////
// setters
void Sprite::setGraphicId(const char* id)
{
	strcpy(gfxId, id);
	gfxHash = hash(gfxId);

	// grab the original size
	gfx = getImgBase()->getImage(gfxId, gfxHash, 0, true);
	origsz.w = gfx->w;
	origsz.h = gfx->h;
}

void Sprite::setAngle(double theta)
{
	ImgBase* base = getImgBase();
	angle = theta;
	gfx = base->getImage(gfxId, gfxHash, base->indexFromAngle(angle), true);
}

void Sprite::setAngleFromXY(double x, double y)
{
	double theta = 0.0;

	// Moving at an angle
	if (x != 0.0 && y != 0.0) {
		theta = atan(y / x);
		if (y < 0.0 && x < 0.0)
			;
		else if (y < 0.0 && x > 0.0)
			theta += M_PI;
		else if (y > 0.0 && x < 0.0)
			theta += M_PI*2;
		else if (y > 0.0 && x > 0.0)
			theta += M_PI;
	}

	// Moving straight
	else {
		if (x < 0.0)
			theta = 0.0;
		else if (x > 0.0)
			theta = M_PI;
		else if (y < 0.0)
			theta = M_PI_2;
		else if (y > 0.0)
			theta = 3*M_PI_2;
	}

	setAngle(theta);
}

void Sprite::setSpeed(double spd)
{
	speed = spd;
}

////////////////////
// getters
double Sprite::getAngle()
{
	return angle;
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

void Sprite::setLoc(short nx, short ny)
{
	x = nx;
	y = ny;
	loc.x = nx;
	loc.y = ny;
}

bool Sprite::isOnScreen()
{
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
	Engine* engine = getEngine();

	if (0 <= x - gfx->w/2 &&
	         x + gfx->w/2 <= engine->getWidth())

		if (0 <= y - gfx->h/2 &&
		         y + gfx->h/2 <= engine->getHeight())

			return true;
	return false;
}

