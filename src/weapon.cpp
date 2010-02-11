#include <stdio.h>
#include <string.h>

#include "audio.h"
#include "common.h"
#include "conf.h"
#include "imgbase.h"
#include "weapon.h"

static Conf data("conf/weapons.conf");

Weapon::Weapon(World* world, const char* weaponType)
	: Item(world), wielder(NULL), cooling(false), reloading(false), count(0)
{
	char buf[256];

	type = strdup(weaponType);

	strcpy(buf, type);
	strcat(buf, ".png");
	Sprite::setGraphicId(buf);

	strcpy(buf, type);
	strcat(buf, "-inv.png");
	invView = images.getImage(buf);

	loadSpecs();
}

Weapon::~Weapon()
{
	free(type);
}

void Weapon::loadSpecs()
{
	strcpy(ammo,      data.getString(type, "Ammo", "bullet"));
	clip = clipsize = data.getInt(type, "Clip size", 1);
	cooldown        = data.getInt(type, "Cooldown", 0);
	reload          = data.getInt(type, "Reload", 0);
	shots           = data.getInt(type, "Shots", 1);
	entropy         = data.getInt(type, "Entropy", 0);
}

bool Weapon::isWeapon()
{
	return true;
}

void Weapon::update(int dt)
{
	if (reloading) {
		count -= dt;
		count = max(count, 0);
		if (count == 0) {
			reloading = false;
			clip = clipsize;
		}
	}
	else if (cooling) {
		count -= dt;
		count = max(count, 0);
		if (count == 0) {
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
	if (reloading)
		return RELOAD;
	if (cooling)
		return COOLDOWN;
	return SHOOT;

}

void Weapon::doShot(list<Shot*>* shotlist)
{
	clip--;

	if (clip == 0) {
		getAudio()->play("Load clip");
		reloading = true;
		count = reload;
	}
	else {
		cooling = true;
		count = cooldown;
	}

	for (int i = 0; i < shots; i++) {
		Shot* shot = new Shot(ammo, wielder, entropy);
		shotlist->push_back(shot);
	}
}

int Weapon::getClip()
{
	return clip;
}

int Weapon::getMaxClip()
{
	return clipsize;
}

