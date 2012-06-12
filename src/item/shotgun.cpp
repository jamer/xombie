#include "shotgun.h"

Shotgun::Shotgun(World* world)
	: Weapon(world, "shotgun")
{
	strcpy(name, "Shotgun");
}
