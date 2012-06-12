#ifndef _SHOT_H_
#define _SHOT_H_

class Shot;

#include "mob.h"
#include "range.h"
#include "sprite.h"

class Shot : public Sprite {
public:
	Shot(QString type, Sprite* src, Range inaccuracy);
	virtual ~Shot();

	void update(int dt);
	void hit(Mob* mob);

	bool isDead();

private:
	bool dead;
	int lifespan;
	real entropy; // percentage, 0.01 to 1.00
	Range damage;
};

#endif // _SHOT_H_
