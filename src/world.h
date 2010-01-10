#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>

#include "engine.h"
#include "mob.h"
#include "shot.h"

using std::list;


class World
{
public:
	World(const char* worldName);

	list<Item*>* getItems();
	list<Mob*>* getMobs();
	list<Char*>* getNeutrals();
	list<Shot*>* getShots();

	Mob* findClosestMob(SDL_Rect* point);

	void update(int dt);
	void playerShoots();

private:
	void updateChars(int dt);
	void updateMobs(int dt);
	void updateItems();
	void updateShots(int dt);

	list<Item*> items;
	list<Mob*> mobs;
	list<Char*> neutrals;
	list<Shot*> shots;

	Engine* engine;

	// TODO: provide getter functions
	int mobCount;
	int peopleCount;

	int itemSpawnDelay;
	int itemSpawnTimer;

	int mobSpawnDelay;
	int mobSpawnTimer;
	int mobSpawnAcceleration;
	int mobSpawnMinimum;
};

#endif // _WORLD_H_

