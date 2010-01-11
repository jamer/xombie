#ifndef NEWMOB_H
#define NEWMOB_H

#include <list>
#include "xml.h"

using namespace xml;

class IndexedList<class T>
{
	list<pair<int,T> > l;
}

class Mob
{
	int hp, mhp;
	range damage;
	int speed;

	// Update on health change.
	graphic g;

	// Update from here.
	IndexedList<graphic>* graphics;
};

class MobPrototype
{
public:
	MobPrototype(int chance, range hp, range damage, int speed, IndexedList<graphic>* graphics);
	Mob* generate();

private:
	int chance;
	range hp;
	range damage;
	int speed;

	IndexedList<graphic>* graphics;
};

class MobFactory
{
public:
	MobFactory(const char* xml);
	Mob* generate();

private:
	void parse(document* xml);
	void parseOne(element* el);

	list<MobPrototype> l;
};

#endif // NEWMOB_H
