#ifndef NEWMOB_H
#define NEWMOB_H

#include <list>
#include "xml.h"

using namespace xml;

class MobPrototype;

class IndexedList<class T>
{
	list<pair<int,T> > l;
}

class Mob
{
public:
	Mob(MobPrototype* type);

private:
	void updateGraphic();
	
	MobPrototype* type;
	int hp;

	// Update on health change.
	graphic g;
};

class MobPrototype
{
public:
	MobPrototype(int chance, range hp, range damage, int speed,
	        IndexedList<graphic>* graphics);
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
