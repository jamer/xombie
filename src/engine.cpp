#include <SDL.h> 
#include <SDL_image.h>

#include "audio.h"
#include "common.h"
#include "conf.h"
#include "engine.h"
#include "imgbase.h"
#include "worldview.h"

Engine* engine;

bool gameLost = false;

Engine* getEngine()
{
	return engine;
}

void LoseGame()
{
	gameLost = true;
}

bool IsGameLost()
{
	return gameLost;
}

Engine::Engine(SDL_Surface* s) : views(10), score(0)
{
	engine = this;

	screen = s;
	newView = NULL;
	closingView = false;

	memset(&mouse, 0, sizeof(mouse));

	FPS = globals->getInt("Game", "FPS", 30);
	UPS = 60;

	loadGame();
	mainLoop();
}

void Engine::loadGame()
{
	// Create a Player with graphic "player.[jpg|png|gif|bmp]".
	player = new Char("player"); 
	player->setPlayer(true);
	party.push_back(player);

	player->setLoc(0, 0);
	player->setAngle(0.0);

	/* Hmm, we should probably load a main menu view rather than jump
	 * straight into combat. Let the player create their main character and
	 * whatnot. */
	CombatView* cv = new CombatView;
	views.push(cv);

	World* world = new World("conf/worlds/default.conf");
	player->setWorld(world);
	cv->setWorld(world);

	getAudio()->startMusic();
}

void Engine::openView(View* view)
{
	newView = view;
}

void Engine::closeView()
{
	closingView = true;
}

int Engine::getHeight()
{
	return screen->h;
}

int Engine::getWidth()
{
	return screen->w;
}

SDL_Surface* Engine::getScreen()
{
	return screen;
}

Char* Engine::getPlayer()
{
	return player;
}

std::list<Char*>* Engine::getParty()
{
	return &party;
}

mouseStruct* Engine::getMouse()
{
	return &mouse;
}

void Engine::addScore(int i)
{
	score += i;
}

int Engine::getScore()
{
	return score;
}


void Engine::runGameEngine(long dt)
{
	View* topView = views.top();
	topView->update(dt);

	if (newView != NULL) {
		views.push(newView);
		newView = NULL;
	}

	if (topView == views.top()) {
		while (closingView) {
			views.pop();
			closingView = false;
		}
	}

	if (views.size() == 0)
		Quit();
}

void Engine::tryRender()
{
	static int count = 0;
	int upf = UPS/FPS; // updates per frame

	if (++count == upf) {
		count = 0;
		render();
	}
}

void Engine::render()
{
	views.top()->draw();
}

/**
 * captureInput()
 *
 * Process 
 */
void Engine::captureInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		handleEvent(event);
	}
}

void Engine::handleEvent(SDL_Event& event)
{
	switch (event.type) {

		case SDL_QUIT:
			Quit();
			break;

		case SDL_MOUSEMOTION:
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					mouse.btn1Down = true;
					break;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					mouse.btn1Down = false;
					break;
			}
			break;

		case SDL_KEYDOWN:
			views.top()->doKeyDown(event.key.keysym.sym);
			break;

		case SDL_KEYUP:
			views.top()->doKeyUp(event.key.keysym.sym);
			break;

		default:
			break;
	}
}



/**
 * wait()
 * 
 * Pauses the game, waiting for the next frame.
 * Returns time in milliseconds since last frame.
 */
long Engine::wait()
{
	static long start = 0, end, dt;
	if (start == 0)
		start = SDL_GetTicks();

	do {
		SDL_Delay(1);
		end = SDL_GetTicks();
	} while (end - start < 1000/UPS);
	dt = end - start;
	start = end;

	return dt;
}

/**
 * mainLoop()
 * runs the game
 */
void Engine::mainLoop()
{
	long dt = 0; // delta time since last frame

	while (true) {
		captureInput();
		runGameEngine(dt);
		tryRender();

		// Sleep 'till next frame, don't waste the CPU
		dt = wait();
	}
}

