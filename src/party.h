#ifndef _PARTY_H_
#define _PARTY_H_

#include <list>

#include "Char.h"

using std::list;

class Party
{
public:
	list<Char*> getParty();

private:
	list<Char*> party;
};

#endif // _PARTY_H_
