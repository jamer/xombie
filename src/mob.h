#ifndef _MOB_H_
#define _MOB_H_

#include <SDL.h>

class Mob;

#include "sprite.h"
#include "world.h"

class Mob : public Sprite
{
public:
	Mob(QString type, World* world);

	/** update()
	 * Update AI; randomly moves left, right, or straight for 0.5 to 2
	 * seconds at a time.
	 */
	void update(int dt);

	/** draw()
	 * Draws location to screen buffer taking rotation, etc. into account.
	 */
	void draw(SDL_Surface* screen);

	QString spawnItem(); // XXX what is this?

	void setHP(int hp);
	bool isDead();
	int getHP();
	int getMaxHP();
	int getDamage();

private:
	/** generateSpawnPosition()
	 * Sets position to a random offscreen location.
	 */
	void generateSpawnPosition(World* world);

	QString type;

	bool dead;
	int hp, mhp;
	int dmg;

	bool damagable;
	QString graphicDmg;

	int dir, dur;
};

Mob* newRandomMob(World* world);

#endif // _MOB_H_

