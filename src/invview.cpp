#include "audio.h"
#include "engine.h"
#include "imgbase.h"
#include "invview.h"

InventoryView::InventoryView()
{
	needsDraw = true;

	audioPlay("Open inventory");
}

void InventoryView::draw()
{
	if (!needsDraw)
		return;

	needsDraw = false;

	Engine* engine = Engine::instance();
	SDL_Surface* screen = engine->getScreen();

	SDL_Surface* invbg = ImgBase::instance()->getImage("inventory");
	SDL_Rect bgrect;

	bgrect.w = invbg->w;
	bgrect.h = invbg->h;
	bgrect.x = (engine->getWidth() - bgrect.w) / 2;
	bgrect.y = (engine->getHeight() - bgrect.h) / 2;

	SDL_BlitSurface(invbg, NULL, screen, &bgrect);

	drawItems(screen);

	// Show it
	SDL_Flip(screen);
}

void InventoryView::drawItems(SDL_Surface* screen)
{
	Char* player = Engine::instance()->getPlayer();
	Inventory* inv = player->getInventory();

	Item* item = inv->getItem(0);
	if (item != NULL)
		item->drawInv(screen, 275, 235);
}

void InventoryView::doKeyDown(int key)
{
	switch (key) {
		case SDLK_ESCAPE:
		case SDLK_i:
		case SDLK_RETURN:
			audioPlay("Close inventory");
			Engine::instance()->closeView();
			break;
	}
}
