#ifndef NEWMOB_H
#define NEWMOB_H

#include <list>
#include "range.h"
#include "sprite.h"
#include "xml.h"

using namespace xml;

class MobPrototype;

struct GraphicDamage
{
	Sprite* graphic;
	int health;
};

class Mob
{
public:
	Mob(MobPrototype* type);

private:
	void updateGraphic();

	int hp, mhp;
	Range damage;
	int speed;

	// Update on health change.
	Sprite g;

	// Update from here.
	std::list<Sprite>* graphics;
	
	MobPrototype* type;
};

class MobPrototype
{
public:
	MobPrototype(int chance, Range hp, Range damage, int speed, IndexedList<Sprite>* graphics);
	Mob* generate();

private:
	int chance;
	Range hp;
	Range damage;
	int speed;

	IndexedList<Sprite>* graphics;
};

class MobFactory
{
public:
	MobFactory(const char* xml);
	Mob* generate();

private:
	void parse(document* xml);
	void parseOne(element* el);

	std::list<MobPrototype> l;
};

#endif // NEWMOB_H
