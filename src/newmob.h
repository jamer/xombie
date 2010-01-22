#ifndef NEWMOB_H
#define NEWMOB_H

#include <list>
#include "range.h"
#include "sprite.h"
#include "xml.h"

using namespace xml;

template <class T1, class T2> struct IndexPair {
	T1* a;
	T2* b;
};


template <class T> class IndexedList
{
	std::list<IndexPair<int,T> > l;
};

class Mob
{
	int hp, mhp;
	Range damage;
	int speed;

	// Update on health change.
	Sprite g;

	// Update from here.
	IndexedList<Sprite>* graphics;
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
