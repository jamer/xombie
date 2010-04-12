#ifndef _VIEW_H_
#define _VIEW_H_

class View
{
public:
	virtual void update(int dt);
	virtual void draw() = 0;
	virtual void doKeyDown(int key);
	virtual void doKeyUp(int key);
	virtual void close();
};

#endif // _VIEW_H_

