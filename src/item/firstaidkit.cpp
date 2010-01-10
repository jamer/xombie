#include "audio.h"
#include "firstaidkit.h"

FirstAidKit::FirstAidKit()
{
	strcpy(name, "First Aid Kit");
	Sprite::init("firstaidkit");
}

bool FirstAidKit::doCollision(Char* ch)
{
	if (ch->getHP() >= ch->getMaxHP())
		return false;
	ch->setHP(ch->getHP() + 1);
	getAudio()->play("First Aid Kit");

	dead = true;
	return true;
}

