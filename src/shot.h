#ifndef _SHOT_H_
#define _SHOT_H_

class Shot;

#include "mob.h"
#include "sprite.h"

class Shot : public Sprite {
public:
	Shot(const char* type, Sprite* src, int entropy);
	~Shot();

	void update(int dt);
	void hit(Mob* mob);

	bool isDead();

private:
	bool dead;
	int dur;
	int time;
	int entropy; // percentage, 0-100
	int damage;
};

#endif // _SHOT_H_

