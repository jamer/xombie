#include <math.h>

#include "audio.h"
#include "char.h"
#include "common.h"
#include "conf.h"
#include "engine.h"
#include "random.h"

Char::Char(const char* graphicId)
	: Sprite(), currentChar(false), hp(3), mhp(3), blinking(0), dead(false)
{
	setSpeed(200.0);

	setGraphicId(graphicId);
	inv = new Inventory(this);
}

Char::~Char()
{
	free(inv);
}

/**
 * update()
 *
 * TODO: Now that I've combined two different methods into one, it needs a new
 * description.
 */
void Char::update(int dt)
{
	if (blinking)
		blinking = max(0, blinking - dt);

	if (currentChar) {
		int x = playerMoveVector.x;
		int y = playerMoveVector.y;

		setAngleFromXY(x, y);

		// Are we moving?
		if (x || y)
			setSpeed(200.0);
		else
			setSpeed(0.0);
		move(dt); 

		// Look at the mouse, yes
		mouseStruct* mouse = getEngine()->getMouse();
		SDL_Rect* ploc = getLoc();

		// Remember, logical screen coordinates have Y values reversed
		setAngleFromXY(ploc->x - mouse->x, mouse->y - ploc->y);
	}
	else {
		// Face nearest mob
		Mob* mob = world->findClosestMob(getLoc());
		if (mob) {
			SDL_Rect* l = getLoc();
			SDL_Rect* m = mob->getLoc();
			setAngleFromXY(l->x - m->x, m->y - l->y);
		}
	}

	stayOnScreen();
}

void Char::draw(SDL_Surface* screen)
{
	if (blinking)
		if ((blinking / 200) % 2 == 0) // one blink-cycle takes 400 ms
			return;
	Sprite::draw(screen);
}

void Char::doCollision(Mob* mob)
{
	// player is invulnerable while blinking, no collisions
	if (blinking)
		return;

	blinking = 2500; // 2.5 seconds
	setHP(getHP() - mob->getDamage());

	if (hp > 0) { // never play both death and hit sounds
		switch (randInt(1, 3)) {
			case 1:
				getAudio()->play("Player ouch 1");
				break;
			case 2:
				getAudio()->play("Player ouch 2");
				break;
			case 3:
				getAudio()->play("Player ouch 3");
				break;
		}
	}
}

void Char::pickUp(Item* item)
{
	inv->addItem(item);
}

bool Char::isPlayer()
{
	return currentChar;
}

void Char::setPlayer(bool player)
{
	currentChar = player;
}

void Char::setInput(SDL_Rect moveVector)
{
	playerMoveVector = moveVector;
}

int Char::getHP()
{
	return hp;
}

void Char::setHP(int HP)
{
	hp = HP;
	if (hp > mhp)
		hp = mhp;

	if (hp <= 0) {
		dead = true;
		getAudio()->play("Player death");
		if (isPlayer())
			LoseGame();
	}
}

int Char::getMaxHP()
{
	return mhp;
}

Inventory* Char::getInventory()
{
	return inv;
}

bool Char::isDead()
{
	return dead;
}

void Char::setWorld(World* w)
{
	world = w;
}

World* Char::getWorld()
{
	return world;
}

