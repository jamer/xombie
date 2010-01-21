#ifndef _WEAPON_H_
#define _WEAPON_H_

class Weapon;

#include <list>

#include "char.h"
#include "item.h"
#include "shot.h"

using std::list;

enum action {
	SHOOT,
	COOLDOWN,
	RELOAD
};

class Weapon : public Item
{
public:
	virtual ~Weapon();

	bool isWeapon();

	void update(int dt);
	virtual bool doCollision(Char* ch);
	void setWielder(Char* ch);

	action tryShot();
	void doShot(list<Shot*>* shots);

	int getClip();
	int getMaxClip();

protected:
	Weapon(World* world, const char* type);
	void loadSpecs();

private:
	char* type;
	Char* wielder;

	char ammo[256];
	int shots;
	int entropy;

	int clipsize;
	int clip; // current ammo in clip

	bool cooling;
	int cooldown;

	bool reloading;
	int reload;

	int count;
};

#endif // _WEAPON_H_

