#ifndef _ITEM_H_
#define _ITEM_H_

class Item;

#include <SDL.h>

#include "char.h"
#include "sprite.h"
#include "world.h"

class Item : public Sprite
{
public:
	virtual ~Item();

	/** doCollision()
	 * player collides with an item, trys to pick it up
	 * returns true if can pick it up, false otherwise
	 */
	virtual bool doCollision(Char* ch) = 0;
	void drawInv(SDL_Surface* screen, int xoff, int yoff);

	virtual bool isWeapon();
	bool isDead();

protected:
	Item(World* w);

	char name[256];
	World* world;

	bool dead;

	SDL_Surface* invView; // inventory portrait
};

#endif // _ITEM_H_
