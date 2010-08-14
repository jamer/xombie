#ifndef _WEAPON_H_
#define _WEAPON_H_

class Weapon;

#include <list>

#include "char.h"
#include "item.h"
#include "range.h"
#include "shot.h"

using std::list;

enum action {
	SHOOT,
	COOLDOWN,
	LOAD
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

	int getShotsRemaining();
	int getMaxClip();
	bool isEmpty();

protected:
	Weapon(World* world, QString type);

private:
	QString type;
	Char* wielder;

	QString ammo;
	int shots;

	Range inaccuracy;

	int clipsize;
	int clip; // current ammo in clip

	/*
	 * struct Cooldown {
	 * } reload, fire
	 */
	bool cooling;
	bool loading;

	Range cooldownDuration;
	Range loadDuration;

	int coolTime;
	int loadTime;

	QString loadSnd;
};

#endif // _WEAPON_H_

