#ifndef _MOB_H_
#define _MOB_H_

#include <SDL.h>

#include "sprite.h"
#include "world.h"

class Mob : public Sprite
{
public:
	Mob(const char* type, World* world);

	/** update()
	 * Update AI; randomly moves left, right, or straight for 0.5 to 2
	 * seconds at a time.
	 */
	void update(int dt);

	/** draw()
	 * Draws location to screen buffer taking rotation, etc. into account.
	 */
	void draw(SDL_Surface* screen);

	char* spawnItem();

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

	char type[64];

	bool dead;
	int hp, mhp;
	int dmg;

	bool damagable;
	const char* graphicDmg;

	int dir, dur;
};

Mob* newRandomMob(World* world);

#endif // _MOB_H_

