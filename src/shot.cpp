#include "audio.h"
#include "conf.h"
#include "math.h"
#include "shot.h"
#include "random.h"

Conf shots("conf/shots.conf");

Shot::Shot(const char* type, Sprite* src, int ent)
{
	init(type);

	entropy = ent;

	double anglent = M_PI / 100 * entropy;
	double angle = src->getAngle() + anglent * 2 * (randDouble() - 0.5);
	setAngle(angle);
	setLoc(src->getLoc()->x, src->getLoc()->y);

	// move to sprite's "face"
	setSpeed((getBoundaries()->w + src->getBoundaries()->w) / 2);
	move(1000);

	// set speed, not all shots move equally
	double variation = 0.1; // maximum variation in either direction
	double multiple = ((randDouble() - 0.5) * variation) + 1.0;
	setSpeed((double)shots.getInt(type, "Speed", 400) * multiple);

	damage = shots.getInt(type, "Damage");
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
	mob->setHP(mob->getHP() - damage);
	dead = true;

	getAudio()->play("Bullet hit body");
}

bool Shot::isDead()
{
	return dead;
}

