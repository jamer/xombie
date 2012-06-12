#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>

#include "audio.h"
#include "common.h"
#include "conf.h"
#include "engine.h"
#include "font.h"
#include "imgbase.h"
#include "main.h"
#include "worldview.h"


void Quit()
{
	Quit(0);
}

void Quit(int code)
{
	delete getEngine();
	delete getAudio();
	delete getImgBase();
	DeinitFont();

	SDL_FreeSurface(getWindowIcon());

	TTF_Quit();
	SDL_Quit();

	delete globals;

	exit(code);
}






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

Engine::Engine(SDL_Surface* s) : views(), score(0)
{
	engine = this;

	screen = s;
	newView = NULL;
	closingView = false;

	mouse.setCursor(COLOR);

	FPS = globals->getInt("Game", "FPS", 30);
	UPS = 60;

	loadGame();
	mainLoop();
}

Engine::~Engine()
{
	while (views.size())
		delete views.pop();
	delete player;
}

void Engine::loadGame()
{
	// Create a Player with graphic "player.[jpg|png|gif|bmp]".
	player = new Char("player");
	player->setPlayer(true);
	party.push_back(player);

	player->getOrientation().setLocation(50, 50);
	player->setAngle(0.0);

	World* world = new World("conf/worlds/default.conf");
	player->setWorld(world);

	/* Hmm, we should probably load a main menu view rather than jump
	 * straight into combat. Let the player create their main character and
	 * whatnot. */
	WorldView* wv = new WorldView(world);
	views.push(wv);

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

Mouse& Engine::getMouse()
{
	return mouse;
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

	if (topView == views.top())
		while (closingView) {
			delete views.pop();
			closingView = false;
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

void Engine::captureInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		handleEvent(event);
}

void Engine::handleEvent(SDL_Event& event)
{
	MouseButton button;
	int key;

	switch (event.type) {

	case SDL_QUIT:
		Quit();
		break;

	case SDL_MOUSEMOTION:
		mouse.setPosition(event);
		break;

	case SDL_MOUSEBUTTONDOWN:
		button = static_cast<MouseButton>(event.button.button);
		views.top()->doMouseDown(button);
		break;
	case SDL_MOUSEBUTTONUP:
		button = static_cast<MouseButton>(event.button.button);
		views.top()->doMouseUp(button);
		break;

	case SDL_KEYDOWN:
		key = event.key.keysym.sym;
		views.top()->doKeyDown(key);
		break;
	case SDL_KEYUP:
		key = event.key.keysym.sym;
		views.top()->doKeyUp(key);
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
