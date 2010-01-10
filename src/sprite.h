#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL.h>

class Sprite
{
public:
	Sprite();
	void init(const char* graphicId);
	void move(long dt);
	void stayOnScreen();
	virtual void draw(SDL_Surface* screen);

	void setGraphic(const char* gfx);
	void setAngle(double theta);
	void setAngleFromXY(double x, double y);
	void setLoc(short x, short y);
	void setSpeed(double speed);

	double getAngle();
	SDL_Surface* getGraphic();
	SDL_Rect* getLoc();
	SDL_Rect* getDispLoc();
	SDL_Rect* getBoundaries(); // collision boundries

	bool isOnScreen();
	bool isCompletelyOnScreen();

protected:
	char gfxId[128];
	SDL_Rect loc; // public location for getLoc
	SDL_Rect origsz;
	SDL_Rect tmp;
	double angle;
	double speed;
	double x, y; // private accurate doubles

	SDL_Surface* gfx;
};

#endif // _SPRITE_H_

