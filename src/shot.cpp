#include <math.h>

#include "audio.h"
#include "conf.h"
#include "shot.h"
#include "random.h"

Conf shots("conf/shots.conf");

Shot::Shot(QString type, Sprite* src, Range inaccuracy) :
	entropy(inaccuracy.get()),
	damage(shots.getRange(type, "Damage", 1, 2))
{
	setGraphicId(type);
	orient = src->getOrientation();

	// XXX use MountPoint

	// move to sprite's "face", this way we look like we're shooting from
	//  head of weapon
	setSpeed((getBoundaries()->w + src->getBoundaries()->w) / 2);
	move(1000);

	Angle anglent = M_PI / 100 * entropy;
	setAngle(getAngle() + anglent * 2 * (randAngle() - 0.5));

	// set speed, not all shots move equally
	real maxVariation = 0.1; // maximum variation in either direction
	real variation = ((randReal() - 0.5) * maxVariation) + 1.0;
	setSpeed(shots.getInt(type, "Speed", 400) * variation);

	dur = shots.getInt(type, "Duration", 10000);
	time = 0;
	dead = false;
}

Shot::~Shot()
{
}

void Shot::update(int dt)
{
	time += dt;
	move(dt);

	// die if time's up or off screen
	if (dur != -1 && time > dur)
		dead = true;
	if (!isOnScreen())
		dead = true;
}

void Shot::hit(Mob* mob)
{
	mob->setHP(mob->getHP() - damage.get());
	dead = true;

	getAudio()->play("Bullet hit body");
}

bool Shot::isDead()
{
	return dead;
}

