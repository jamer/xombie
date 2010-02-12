#ifndef _WORLDVIEW_H_
#define _WORLDVIEW_H_

#include <list>

#include "char.h"
#include "engine.h"
#include "item.h"
#include "mob.h"
#include "shot.h"
#include "view.h"
#include "world.h"

using std::list;

class WorldView : public View
{
public:
	WorldView();
	void update(int dt);
	void draw();
	void doKeyDown(int key);
	void doKeyUp(int key);

	void setWorld(World* world);

private:
	void dropItem();
	void generateInfoText();

	char tldisplay[512]; // top left
	char bldisplay[512]; // bottom left
	char trdisplay[512]; // top right
	char brdisplay[512]; // bottom right

	bool leftPressed, rightPressed, upPressed, downPressed;

	Engine* engine;
	World* world;
};

#endif // _WORLDVIEW_H_

