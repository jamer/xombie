#ifndef _INVENTORY_H_
#define _INVENTORY_H_

class Inventory;

#include "item.h"
#include "weapon.h"

// Permanent maximum inventory size
#define INV_SZ 6

class Inventory
{
public:
	Inventory(Char* ch);
	~Inventory();

	bool addItem(Item* item);
	Item* getItem(int i);

	Weapon* getWeapon();
	Weapon* dropWeapon();

	int getCount();
	int getFreeSlot();

private:
	Weapon* weapon;
	Item** items;
	Char* ch;
};

#endif // _INVENTORY_H_

