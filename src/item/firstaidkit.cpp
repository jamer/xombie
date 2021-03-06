#include "audio.h"
#include "firstaidkit.h"

FirstAidKit::FirstAidKit(World* world)
	: Item(world)
{
	strcpy(name, "First Aid Kit");
	Sprite::setGraphicId("firstaidkit");
}

bool FirstAidKit::doCollision(Char* ch)
{
	if (ch->getHP() >= ch->getMaxHP())
		return false;
	ch->setHP(ch->getHP() + 1);
	audioPlay("First Aid Kit");

	dead = true;
	return true;
}
