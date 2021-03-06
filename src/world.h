#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <SDL.h>

class World;

#include "engine.h"
#include "mob.h"
#include "shot.h"
#include "point.h"

using std::list;


class World
{
public:
	World(QString worldName);
	~World();

	list<Item*>* getItems();
	list<Mob*>* getMobs();
	list<Char*>* getNeutrals();
	list<Shot*>* getShots();

	Mob* findClosestMob(Point location);

	void update(int dt);
	void playerShoots();

	SDL_Rect* getBounds();

private:
	void updateChars(int dt);
	void updateMobs(int dt);
	void updateItems();
	void updateShots(int dt);

	list<Item*> items;
	list<Mob*> mobs;
	list<Char*> neutrals;
	list<Shot*> shots;

	// TODO: provide getter functions
	int mobCount;
	int peopleCount;

	int itemSpawnDelay;
	int itemSpawnTimer;

	int mobSpawnDelay;
	int mobSpawnTimer;
	int mobSpawnAcceleration;
	int mobSpawnMinimum;

	SDL_Rect bounds;
};

#endif // _WORLD_H_
