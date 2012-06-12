#include <math.h>

#include "audio.h"
#include "conf.h"
#include "shot.h"
#include "random.h"

Conf shots("conf/shots.conf");

Shot::Shot(QString type, Sprite* src, Range inaccuracy) :
	entropy(inaccuracy.value() * 0.01),
	damage(shots.getRange(type, "Damage", 1, 2))
{
	setGraphicId(type);
	orient = src->getOrientation();

	// XXX use MountPoint

	// move to sprite's "face", this way we look like we're shooting from
	//  head of weapon
	setSpeed((getBoundaries()->w + src->getBoundaries()->w) / 2);
	move(1000);

	angle nAngle = getAngle() + entropy * (randAngle() - M_PI);
	setAngle(nAngle);

	// set speed, not all shots move equally
	real maxVariation = 0.1; // maximum variation in either direction
	real variation = ((randReal() - 0.5) * maxVariation) + 1.0;
	setSpeed(shots.getInt(type, "Speed", 400) * variation);

	lifespan = shots.getInt(type, "Duration", 10000);
	dead = false;
}

Shot::~Shot()
{
}

void Shot::update(int dt)
{
	lifespan -= dt;
	move(dt);

	if (lifespan <= 0 || !isOnScreen())
		dead = true;
}

void Shot::hit(Mob* mob)
{
	mob->setHP(mob->getHP() - damage.value());
	dead = true;

	audioPlay("Bullet hit body");
}

bool Shot::isDead()
{
	return dead;
}
