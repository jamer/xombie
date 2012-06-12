#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "char.h"
#include "common.h"
#include "font.h"
#include "invview.h"
#include "worldview.h"

using std::list;

WorldView::WorldView(World* w)
	: shooting(false),
	leftPressed(false), rightPressed(false),
	upPressed(false), downPressed(false), world(w)
{
}

void WorldView::update(int dt)
{
	// Once you die, we display a message for three seconds and quit
	if (IsGameLost()) {
		static unsigned int deathTimer = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() >= deathTimer)
			Engine::instance()->requestQuit();

		return;
	}

	// Give the player character his movement input.
	SDL_Rect vector;
	vector.x = leftPressed - rightPressed;
	vector.y = downPressed - upPressed;
	Engine::instance()->getPlayer()->setInput(vector);

	world->update(dt);

	// Shoot on mouse click
	if (shooting)
		world->playerShoots();
}

void WorldView::draw()
{
	// Blank screen buffer
	SDL_Surface* screen = Engine::instance()->getScreen();
	SDL_FillRect(screen, NULL, 0x000000);

	// Death message if dead
	if (IsGameLost()) {
		char buf[256];
		sprintf(buf, "You become a zombie. Score: %d",
				Engine::instance()->getScore());
		SDL_Surface* text = renderFont(buf);

		SDL_Rect loc;
		loc.x = (Engine::instance()->getWidth()  - text->w) / 2;
		loc.y = (Engine::instance()->getHeight() - text->h) / 2;

		SDL_BlitSurface(text, NULL, screen, &loc);
		SDL_FreeSurface(text);

		SDL_Flip(Engine::instance()->getScreen());

		return;
	}

	// Lets get our variable ready! This was fun to type out. Just kidding.
	list<Char*>* neutrals = world->getNeutrals();
	list<Char*>* party = Engine::instance()->getParty();

	list<Item*>* items = world->getItems();
	list<Mob*>* mobs = world->getMobs();
	list<Shot*>* shots = world->getShots();

	list<Item*>::iterator ii;
	list<Mob*>::iterator mi;
	list<Shot*>::iterator si;

	// A reverse iterator will ensure that the player will always be
	// rendered on top of other characters.
	list<Char*>::reverse_iterator ci;

	// Render items, mobs, shots, characters
	for (ii = items->begin(); ii != items->end(); ii++)
		(*ii)->draw(screen);
	for (mi = mobs->begin(); mi != mobs->end(); mi++)
		(*mi)->draw(screen);
	for (si = shots->begin(); si != shots->end(); si++)
		(*si)->draw(screen);
	for (ci = neutrals->rbegin(); ci != neutrals->rend(); ci++)
		(*ci)->draw(screen);
	for (ci = party->rbegin(); ci != party->rend(); ci++)
		(*ci)->draw(screen);

	Engine::instance()->getMouse().draw(screen);

	// Show it
	SDL_Flip(Engine::instance()->getScreen());
}

void WorldView::dropItem()
{
/*
	Char* p = Engine::instance()->getPlayer();
	Weapon* w = p->getInventory()->dropWeapon();
	if (w == NULL)
		return;

	SDL_Rect* xy = p->getLoc(), *wh = p->getDispLoc();

	// TODO: what is this math?
	w->setLoc(xy->x +  (30 + wh->w) * cos(p->getAngle()),
	          xy->y -  (30 + wh->h) * sin(p->getAngle()));
	items.push_back(w);
*/
}

void WorldView::doMouseDown(MouseButton button)
{
	switch (button) {
	case LEFT:
		shooting = true;
		break;
	default:
		break;
	}
}

void WorldView::doMouseUp(MouseButton button)
{
	switch (button) {
	case LEFT:
		shooting = false;
		break;
	default:
		break;
	}
}

void WorldView::doKeyDown(int key)
{
	InventoryView* i;

	if (IsGameLost())
		return;

	switch (key) {
		case SDLK_a:
			leftPressed = true;
			break;
		case SDLK_d:
			rightPressed = true;
			break;
		case SDLK_s:
			downPressed = true;
			break;
		case SDLK_w:
			upPressed = true;
			break;
		case SDLK_i:
			i = new InventoryView;
			Engine::instance()->openView(i);
			break;
		case SDLK_BACKSLASH:
//			dropItem();
			break;
		case SDLK_ESCAPE:
			Engine::instance()->closeView();
			break;
		default:
			break;
	}
}

void WorldView::doKeyUp(int key)
{
	if (IsGameLost())
		return;

	switch (key) {
		case SDLK_a:
			leftPressed = false;
			break;
		case SDLK_d:
			rightPressed = false;
			break;
		case SDLK_s:
			downPressed = false;
			break;
		case SDLK_w:
			upPressed = false;
			break;
		default:
			break;
	}
}

void WorldView::close()
{
}
