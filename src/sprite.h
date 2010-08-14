#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <QString>
#include <SDL.h>

#include "orientation.h"

#define S_CAN_ROTATE 1

class Sprite
{
public:
	Sprite();
	~Sprite();

	void move(long dt);
	void move(long dt, Angle angle);
	void stayOnScreen();
	virtual void draw(SDL_Surface* screen);

	void setGraphicId(QString id);
	void setAngle(Angle theta);
	void setAngleFromXY(real x, real y);
	void setLoc(short x, short y);
	void setSpeed(real speed);

	Angle getAngle();
	Orientation getOrientation();

	SDL_Surface* getGraphic();
	SDL_Rect* getLoc(); // XXX
	SDL_Rect* getDispLoc();
	SDL_Rect* getBoundaries(); // collision boundries

	bool isOnScreen();
	bool isCompletelyOnScreen();

protected:
	QString gfxId;


	SDL_Rect loc; // public location for getLoc // XXX
	SDL_Rect origsz;
	SDL_Rect tmp;

	Orientation orient;
	real speed;

	SDL_Surface* gfx;
};

#endif // _SPRITE_H_

