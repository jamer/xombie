#ifndef _FIRSTAIDKIT_H_
#define _FIRSTAIDKIT_H_

#include "char.h"
#include "item.h"

class FirstAidKit : public Item
{
public:
	FirstAidKit(World* world);
	bool doCollision(Char* ch);
};

#endif // _FIRSTAIDKIT_H_

