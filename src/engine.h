#ifndef _ENGINE_H_
#define _ENGINE_H_


// deinitialize everything and quit
void Quit();
void Quit(int code);




#include <SDL.h>
#include <list>

class Engine;

#include "char.h"
#include "fstack.h"
#include "view.h"

using std::list;

struct mouseStruct {
	int x;
	int y;
	bool btn1Down;
	bool btn2Down;	
};

bool IsGameLost();
void LoseGame();


/** getEngine()
 * global function for easy acces to current Engine object
 */
Engine* getEngine();


class Engine
{
public:
	Engine(SDL_Surface* screen);
	~Engine();
	
	void mainLoop();

	/** openView()
	 * stacks a view on the view stack
	 */
	void openView(View* view);

	/** closeView()
	 * closes the top view
	 */
	void closeView();

	int getHeight();
	int getWidth();

	SDL_Surface* getScreen();
	Char* getPlayer();
	list<Char*>* getParty();
	mouseStruct* getMouse();

	void addScore(int i);
	int getScore();

private:
	void loadGame();

	/** runGameEngine()
	 * handles input, runs simulations, etc.
	 * we loop this and the game runs
	 */
	void runGameEngine(long dt);

	/** captureInput()
	 * capture input and give it to the top View
	 */
	void captureInput();
	void handleEvent(SDL_Event& event);

	/** tryRender
	 * we update the internal game engine 60 times per second
	 * even if we don't display 60 frames per second
	 */
	void tryRender();

	/** render()
	 * asks the top View to render
	 */
	void render();

	/** wait()
	 * sleeps until next frame
	 */
	long wait();


	SDL_Surface* screen; // backbuffer
	mouseStruct mouse;

	Fstack<View*> views;
	View* newView;
	bool closingView;

	int FPS; // frames displayed per second
	int UPS; // engine updates per second

	Char* player;
	list<Char*> party; // list of characters in party
	
	World* world;

	int score;
};

#endif // _ENGINE_H_

