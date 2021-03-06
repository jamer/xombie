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
	WorldView(World* world);
	void update(int dt);
	void draw();

	void doMouseDown(MouseButton button);
	void doMouseUp(MouseButton button);
	void doKeyDown(int key);
	void doKeyUp(int key);

	void close();

private:
	void dropItem();
	void generateInfoText(char** tldisplay, char** trdisplay,
			char** bldisplay, char** brdisplay);

	bool shooting;
	bool leftPressed, rightPressed, upPressed, downPressed;

	World* world;
};

#endif // _WORLDVIEW_H_
