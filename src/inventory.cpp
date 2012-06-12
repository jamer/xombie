#include "inventory.h"
#include "item.h"
#include "weapon.h"

Inventory::Inventory(Char* c)
{
	items = new Item*[INV_SZ];
	for (int i = 0; i < INV_SZ; i++)
		items[i] = NULL;
	weapon = NULL;
	ch = c;
}

Inventory::~Inventory()
{
	for (int i = 0; i < INV_SZ; i++)
		if (items[i])
			delete items[i];
	delete[] items;
	if (weapon)
		delete weapon;
}

bool Inventory::addItem(Item* item)
{
	if (item->isWeapon() && getWeapon() == NULL) {
		weapon = (Weapon*)item;
		weapon->setWielder(ch);
		return true;
	}

	int slot = getFreeSlot();
	if (slot == -1)
		return false;

	items[slot] = item;
	return true;
}

Item* Inventory::getItem(int i)
{
	return items[i];
}

Weapon* Inventory::getWeapon()
{
	return weapon;
}

Weapon* Inventory::dropWeapon()
{
	Weapon* tmp = weapon;
	weapon = NULL;

	return tmp;
}

int Inventory::getCount()
{
	int cnt = 0;
	for (int i = 0; i < INV_SZ; i++)
		if (items[i])
			cnt++;
	return cnt;
}

int Inventory::getFreeSlot()
{
	for (int i = 0; i < INV_SZ; i++)
		if (items[i] == NULL)
			return i;
	return -1;
}
