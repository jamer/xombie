#include "pistol.h"

Pistol::Pistol(World* world)
	: Weapon(world, "pistol")
{
	strcpy(name, "Pistol");
}
