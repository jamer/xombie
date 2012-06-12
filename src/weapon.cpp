#include <stdio.h>
#include <string.h>

#include "audio.h"
#include "common.h"
#include "conf.h"
#include "e.h"
#include "imgbase.h"
#include "weapon.h"

static Conf data("conf/weapons.conf");

Weapon::Weapon(World* world, QString weaponType) :
	Item(world), type(weaponType), wielder(NULL),
	inaccuracy(data.getRange(type, "Inaccuracy", 0, 3)),
	cooling(false), loading(false),
	cooldownDuration(data.getRange(type, "Cooldown", 700, 900)),
	loadDuration(data.getRange(type, "Load time", 2000, 2200))
{
	setGraphicId(QString("%1.png").arg(type));
	invView = ImgBase::instance()->getImage(QString("%1-inv.png").arg(type));

	ammo = data.getString(type, "Ammo", NULL);
	if (ammo.isEmpty())
		err("Weapon must have an ammo type.");

	clipsize = data.getInt(type, "Clip size", 1);
	clip = clipsize;

	shots = data.getInt(type, "Shots", 1);
	loadSnd = data.getString(type, "Loading sound", NULL);
}

Weapon::~Weapon()
{
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
		coolTime = cooldownDuration.value();

		for (int i = 0; i < shots; i++) {
			Shot* shot = new Shot(ammo, wielder, inaccuracy);
			shotlist->push_back(shot);
		}
	}

	else {
		if (!loadSnd.isEmpty())
			audioPlay(loadSnd);
		loading = true;
		loadTime = loadDuration.value();
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
