#ifndef _IMGBASE_H_
#define _IMGBASE_H_

/*
 * Image database/cache
 *
 * Don't want to load images every time we need 'em. Keep 'em here.
 * 
 * 2009-03-11 - Paul
 * 	Redesigned it to load images from disk for you, so no need to
 * 	precache them manually ;-)
 */

#include <map>
#include <math.h>
#include <SDL.h>

#include "common.h"

// Flags for ImgBase::put
#define IB_PUT			0x000000 // no flags
#define IB_IMAGE_ROTATES	0x000001 // rotating image

/**
 * Get an image index for if you're using a rotational object
 */
int INDEX_FROM_ANGLE(double angle);


void InitImgBase();

class ImgBase
{
public:
	SDL_Surface* getImage(const char* name, int frame = 0, bool rotate = false);

	// For adding custom images
	void put(const char* name, SDL_Surface* img, DWORD flags);

private:
	// Look for && load image not in db
	SDL_Surface* lookForImg(const char* name, int frame, bool rotates);

	struct ImgRef {
		char name[128];
		SDL_Surface** images;
	};

	std::map<int, ImgRef> base;
};

extern ImgBase images;

#endif // _IMGBASE_H_

