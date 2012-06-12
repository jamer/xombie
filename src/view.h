#ifndef _VIEW_H_
#define _VIEW_H_

enum MouseButton
{
	LEFT = 1,
	MIDDLE,
	RIGHT,
	WHEEL_UP,
	WHEEL_DOWN
};

class View
{
public:
	virtual void update(int dt);
	virtual void draw() = 0;
	virtual void doMouseDown(MouseButton button);
	virtual void doMouseUp(MouseButton button);
	virtual void doKeyDown(int key);
	virtual void doKeyUp(int key);
	virtual void close();
};

#endif // _VIEW_H_
