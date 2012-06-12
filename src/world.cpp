#include <math.h>

#include "common.h"
#include "conf.h"
#include "engine.h"
#include "random.h"
#include "world.h"

// Includes from src/item/
#include "firstaidkit.h"
#include "pistol.h"


World::World(QString worldName)
{
	engine = getEngine();

	// Load specs.
	Conf lvl(worldName);

	peopleCount = lvl.getInt("People", "Count");

	mobCount = lvl.getInt("Mobs", "Initial Count");
	mobSpawnDelay = lvl.getInt("Mobs", "Spawn Delay");
	mobSpawnTimer = lvl.getInt("Mobs", "Spawn Timer");
	mobSpawnAcceleration = lvl.getInt("Mobs", "Spawn Acceleration");
	mobSpawnMinimum = lvl.getInt("Mobs", "Spawn Minimum");

	itemSpawnDelay = lvl.getInt("Items", "Spawn Delay");
	itemSpawnTimer = lvl.getInt("Items", "Spawn Timer");

	// Init party member, starting mobs, and first weapon.
	for (int i = 0; i < peopleCount; i++) {
		Char* partymem = new Char("susan");
		partymem->setWorld(this);
		partymem->getOrientation().setLocation(randInt(200, 300), randInt(200, 300));
		partymem->setAngle(randAngle());
		partymem->pickUp(new Pistol(this));
		engine->getParty()->push_back(partymem);
	}

	for (int i = 0; i < mobCount; i++) {
		Mob* zombie = newRandomMob(this);
		mobs.push_back(zombie);
	}

	Pistol* pistol = new Pistol(this);
	pistol->getOrientation().setLocation(150, 150); // FIXME: Shouldn't need to call this.
	items.push_back(pistol);

	bounds.x = 0;
	bounds.y = 0;
	bounds.w = lvl.getInt("Map", "Width", 640);
	bounds.h = lvl.getInt("Map", "Height", 480);
}

World::~World()
{
	list<Item*>::iterator itemit;
	list<Mob*>::iterator mobit;
	list<Char*>::iterator charit;
	list<Shot*>::iterator shotit;

	for (itemit = items.begin(); itemit != items.end(); itemit++)
		delete *itemit;
	for (mobit = mobs.begin(); mobit != mobs.end(); mobit++)
		delete *mobit;
	for (charit = neutrals.begin(); charit != neutrals.end(); charit++)
		delete *charit;
	for (shotit = shots.begin(); shotit != shots.end(); shotit++)
		delete *shotit;
	
	items.clear();
	mobs.clear();
	neutrals.clear();
	shots.clear();
}

list<Item*>* World::getItems()
{
	return &items;
}

list<Mob*>* World::getMobs()
{
	return &mobs;
}

list<Char*>* World::getNeutrals()
{
	return &neutrals;
}

list<Shot*>* World::getShots()
{
	return &shots;
}


void World::playerShoots()
{
	Weapon* weapon = engine->getPlayer()->getInventory()->getWeapon();
	if (weapon != NULL && weapon->tryShot() == SHOOT)
		weapon->doShot(&shots);
}

Mob* World::findClosestMob(Point location)
{
	real shortest = 0;
	Mob* nearest = NULL;

	list<Mob*>::iterator mobit;
	for (mobit = mobs.begin(); mobit != mobs.end(); mobit++) {
		Mob* mob = *mobit;
		real dist = location.distanceTo(
				mob->getOrientation().getLocation());
		if (dist < shortest) {
			shortest = dist;
			nearest = mob;
		}
	}

	return nearest;
}

void World::update(int dt)
{
	// Spawn items
	static unsigned int itemTimer = SDL_GetTicks() + itemSpawnDelay;
	while (SDL_GetTicks() >= itemTimer) {
		// TODO: random item generator
		Item* item = new FirstAidKit(this);
		items.push_back(item);
		itemTimer = SDL_GetTicks() + itemSpawnTimer;
	}

	// Spawn mobs
	static unsigned int mobTimer = SDL_GetTicks() + mobSpawnDelay;
	static int spawned = 0;

	while (SDL_GetTicks() >= mobTimer) {
		Mob* mob = newRandomMob(this);
		mobs.push_back(mob);

		int delay = max(mobSpawnMinimum,
		                mobSpawnTimer -
		                mobSpawnAcceleration * spawned);
		spawned++;
		mobTimer = SDL_GetTicks() + delay;
	}

	updateChars(dt);
	updateMobs(dt);
	updateItems();
	updateShots(dt);
}

void World::updateChars(int dt)
{
	list<Char*>* party = engine->getParty();
	list<Char*>::iterator pit;
	for (pit = party->begin(); pit != party->end(); pit++) {
		Char* ch = *pit;

		ch->update(dt);

		Weapon* weapon = ch->getInventory()->getWeapon();
		if (weapon != NULL) {
			weapon->update(dt);
			if (ch->isPlayer() == false
				&& weapon->tryShot() == SHOOT
				&& mobs.size()) {
				weapon->doShot(&shots);
			}
		}

		if (ch->isDead()) {
			pit = party->erase(pit);
			delete ch;
		}
	}
}

void World::updateMobs(int dt)
{
	list<Char*>* party = engine->getParty();

	list<Mob*>::iterator mobit;
	list<Shot*>::iterator shotit;
	list<Char*>::iterator pit;


	for (mobit = mobs.begin(); mobit != mobs.end();) {
		Mob* mob = *mobit;
		SDL_Rect* mrect = mob->getBoundaries();

		mob->update(dt);

		// Collide with party.
		for (pit = party->begin(); pit != party->end(); pit++) {
			Char* ch = *pit;
			if (testCollision(mrect, ch->getBoundaries()))
				ch->doCollision(mob);
		}

		// Collide with shots.
		for (shotit = shots.begin(); shotit != shots.end();) {
			if (mob->isDead())
				break;

			Shot* shot = *shotit;
			bool collide = testCollision(shot->getBoundaries(),
						     mrect);
			if (collide) {
				shot->hit(mob);
				if (shot->isDead()) {
					shotit = shots.erase(shotit);
					delete shot;
					continue;
				}
			}
			shotit++;
		}

		if (mob->isDead()) {
			engine->addScore(1);
			mobit = mobs.erase(mobit);

			// TODO: make random item generator
/*
			char* item = mob->spawnItem();
			if (item != NULL) {
				Item* i = new Item(item, mob->getLoc());
				items.push_back(i);
			}
*/
			delete mob;
			continue;
		}

		mobit++;
	}
}

void World::updateItems()
{
	list<Char*>* party = engine->getParty();

	list<Item*>::iterator itemit;
	list<Char*>::iterator pit;

	if (items.empty())
		return;

	for (itemit = items.begin(); itemit != items.end();) {
		Item* item = *itemit;

		for (pit = party->begin(); pit != party->end(); pit++) {
			Char* ch = *pit;

			bool collide = testCollision(item->getBoundaries(),
			                             ch->getBoundaries());

			if (collide) {
				// See if the player can pick it up
				bool pickedUp = item->doCollision(ch);
				if (pickedUp) {
					itemit = items.erase(itemit);
					if (item->isDead())
						delete item;
					continue;
				}
			}
		}
		itemit++;

	}

}

void World::updateShots(int dt)
{
	list<Shot*>::iterator shotit;

	if (shots.empty())
		return;

	for (shotit = shots.begin(); shotit != shots.end();) {
		Shot* shot = *shotit;

		shot->update(dt);
		if (shot->isDead()) {
			shotit = shots.erase(shotit);
			delete shot;
			continue;
		}

		shotit++;
	}
}



SDL_Rect* World::getBounds()
{
	return &bounds;
}

