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

WorldView::WorldView()
	: leftPressed(false), rightPressed(false),
	upPressed(false), downPressed(false), world(NULL)
{
	engine = getEngine();
}

void WorldView::setWorld(World* w)
{
	world = w;
}

void WorldView::update(int dt)
{
	// Once you die, we display a message for three seconds and quit
	if (IsGameLost()) {
		static unsigned int deathTimer = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() >= deathTimer)
			Quit();

		return;
	}

	// Give the player character his movement input.
	SDL_Rect vector;
	vector.x = leftPressed - rightPressed;
	vector.y = downPressed - upPressed;
	engine->getPlayer()->setInput(vector);

	world->update(dt);

	// Shoot on mouse click
	mouseStruct* mouse = engine->getMouse();
	if (mouse->btn1Down) {
		world->playerShoots();
	}
}

void WorldView::draw()
{
	// Blank screen buffer
	SDL_Surface* screen = engine->getScreen();
	SDL_FillRect(screen, NULL, 0x000000);

	// Death message if dead
	if (IsGameLost()) {
		char buf[256];
		sprintf(buf, "You become a zombie. Score: %d",
				engine->getScore());
		SDL_Surface* text = renderFont(buf);

		SDL_Rect loc;
		loc.x = (engine->getWidth()  - text->w) / 2;
		loc.y = (engine->getHeight() - text->h) / 2;

		SDL_BlitSurface(text, NULL, screen, &loc);
		SDL_FreeSurface(text);

		SDL_Flip(engine->getScreen());

		return;
	}

	// Lets get our variable ready! This was fun to type out. Just kidding.
	list<Char*>* neutrals = world->getNeutrals();
	list<Char*>* party = engine->getParty();

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

	// Render text
	SDL_Surface* text;
	SDL_Rect location;

	generateInfoText();

	// top left text
	if (tldisplay[0] != 0) {
		text = renderFont(tldisplay);
		location.x = location.y = 5;
		SDL_BlitSurface(text, NULL, screen, &location);
		SDL_FreeSurface(text);
	}

	// top right text
	if (trdisplay[0] != 0) {
		text = renderFont(trdisplay);
		location.x = engine->getWidth() - 5 - text->w;
		location.y = 5;
		SDL_BlitSurface(text, NULL, screen, &location);
		SDL_FreeSurface(text);
	}

	// bottom left text
	if (bldisplay[0] != 0) {
		text = renderFont(bldisplay);
		location.x = 5;
		location.y = engine->getHeight() - 5 - text->h;
		SDL_BlitSurface(text, NULL, screen, &location);
		SDL_FreeSurface(text);
	}

	// bottom right text
	if (brdisplay[0] != 0) {
		text = renderFont(brdisplay);
		location.x = engine->getWidth() - 5 - text->w;
		location.y = engine->getHeight() - 5 - text->h;
		SDL_BlitSurface(text, NULL, screen, &location);
		SDL_FreeSurface(text);
	}

	// Show it
	SDL_Flip(engine->getScreen());
}

void WorldView::dropItem()
{
/*
	Char* p = engine->getPlayer();
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
			engine->openView(i);
			break;
		case SDLK_BACKSLASH:
//			dropItem();
			break;
		case SDLK_ESCAPE:
			Quit();
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

void WorldView::generateInfoText()
{
	Char* player = engine->getPlayer();

	strcpy(tldisplay, "No movement");
	strcpy(trdisplay, "");

	const char* dir;

	// Get angle
	double theta = player->getAngle();
	if (theta == 0.0)
		dir = "E";
	else if (theta < M_PI_2)
		dir = "NE";
	else if (theta == M_PI_2)
		dir = "N";
	else if (theta < M_PI)
		dir = "NW";
	else if (theta == M_PI)
		dir = "W";
	else if (theta < 3 * M_PI_2)
		dir = "SW";
	else if (theta == 3 * M_PI_2)
		dir = "S";
	else
		dir = "SE";

	sprintf(tldisplay, "theta = %f, dir = %s, x = %f, y = %f", theta, dir,
	        cos(theta), sin(theta));

	sprintf(trdisplay, "Score: %d", engine->getScore());

	sprintf(bldisplay, "HP: %d, you are at (%i, %i)",
	        player->getHP(),
	        player->getLoc()->x, player->getLoc()->y);

	// bottom right display --> weapon status + # of zombies
	char zombies[256];
	int sz = world->getMobs()->size();
	if (sz)
		sprintf(zombies, ", %d zombies", sz);
	else
		strcpy(zombies, "");

	char wdisp[256];
	Weapon* weapon = engine->getPlayer()->getInventory()->getWeapon();
	if (weapon == NULL)
		strcpy(wdisp, "No weapon");
	else {
		int status = weapon->tryShot();
		if (status == SHOOT)
			sprintf(wdisp, "%d/%d shots",
			        weapon->getClip(), weapon->getMaxClip());
		else if (status == COOLDOWN)
			sprintf(wdisp, "cooling %d/%d shots",
			        weapon->getClip(), weapon->getMaxClip());
		else if (status == RELOAD)
			sprintf(wdisp, "reloading 0/%d shots",
			        weapon->getMaxClip());
	}

	sprintf(brdisplay, "%s%s", wdisp, zombies);
}

