#ifndef _CHAR_H_
#define _CHAR_H_

class Char;

#include "inventory.h"
#include "item.h"
#include "mob.h"
#include "sprite.h"
#include "world.h"

class Char : public Sprite
{
public:
	Char(QString graphicId);
	virtual ~Char();

	void update(int dx, int dy, int dt);
	void update(int dt);
	void draw(SDL_Surface* screen);
	void doCollision(Mob* mob);
	void pickUp(Item* item);

	bool isPlayer();
	void setPlayer(bool player);
	void setInput(SDL_Rect moveVector);

	int getHP();
	void setHP(int hp);
	int getMaxHP();
	Inventory* getInventory();

	bool isDead();

	void setWorld(World* world);
	World* getWorld();

private:
	World* world;
	Inventory inventory;

	bool currentChar;
	int hp, mhp;

	int blinking;

	bool dead;

	SDL_Rect playerMoveVector;
};

#endif // _CHAR_H_
