#include <stdio.h>
#include <string.h>

#include "audio.h"
#include "common.h"
#include "conf.h"
#include "e.h"
#include "imgbase.h"
#include "weapon.h"

static Conf data("conf/weapons.conf");

Weapon::Weapon(World* world, const char* weaponType)
	: Item(world), wielder(NULL), cooling(false), loading(false)
{
	char buf[256];

	type = strdup(weaponType);

	strcpy(buf, type);
	strcat(buf, ".png");
	Sprite::setGraphicId(buf);

	strcpy(buf, type);
	strcat(buf, "-inv.png");
	invView = getImgBase()->getImage(buf);

	loadSpecs();
	
	clip = clipsize;
}

Weapon::~Weapon()
{
	if (type)
		free(type);
	if (ammo)
		free(ammo);
	if (loadSnd)
		free(loadSnd);
}

void Weapon::loadSpecs()
{
	ammo             = strdup(data.getString(type, "Ammo", NULL));
	if (!ammo)
		err(1, "Weapon must have an ammo type.");

	clipsize         = data.getInt(type, "Clip size", 1);
	inaccuracy       = data.getRange(type, "Inaccuracy", 0, 3);
	cooldownDuration = data.getRange(type, "Cooldown", 700, 900);
	loadDuration     = data.getRange(type, "Load time", 2000, 2200);
	shots            = data.getInt(type, "Shots", 1);

	const char* sound;

	sound           = data.getString(type, "Loading sound", NULL);
	loadSnd         = sound ? strdup(sound) : NULL;
}

bool Weapon::isWeapon()
{
	return true;
}

void Weapon::update(int dt)
{
	if (loading) {
		loadTime -= dt;
		loadTime = max(loadTime, 0);
		if (loadTime == 0) {
			loading = false;
			clip = clipsize;
		}
	}
	else if (cooling) {
		coolTime -= dt;
		coolTime = max(coolTime, 0);
		if (coolTime == 0) {
			cooling = false;
		}
	}
}

bool Weapon::doCollision(Char* ch)
{
	ch->pickUp(this);
	return true;
}

void Weapon::setWielder(Char* ch)
{
	wielder = ch;
}

action Weapon::tryShot()
{
	if (loading)
		return LOAD;
	if (cooling)
		return COOLDOWN;
	return SHOOT;

}

void Weapon::doShot(list<Shot*>* shotlist)
{
	if (cooling || loading)
		return;

	if (getShotsRemaining()) {
		clip--;

		cooling = true;
		coolTime = cooldownDuration.get();

		for (int i = 0; i < shots; i++) {
			Shot* shot = new Shot(ammo, wielder, inaccuracy);
			shotlist->push_back(shot);
		}
	}

	else {
		if (loadSnd)
			getAudio()->play(loadSnd);
		loading = true;
		loadTime = loadDuration.get();
	}
}

int Weapon::getShotsRemaining()
{
	return clip;
}

int Weapon::getMaxClip()
{
	return clipsize;
}

bool Weapon::isEmpty()
{
	return clip == 0;
}

