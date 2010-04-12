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
 * 2010-01-21 - Paul
 * 	Add APIs that make caching the hash more practical.
 */

#include <map>
#include <math.h>
#include <SDL.h>

#include "common.h"

// Flags for ImgBase::put
#define IB_PUT			0x000000 // no flags
#define IB_IMAGE_ROTATES	0x000001 // rotating image

class ImgBase
{
public:
	ImgBase();
	~ImgBase();

	SDL_Surface* getImage(const char* name,
	        int frame = 0, bool rotate = false);
	SDL_Surface* getImage(const char* name, uint32_t hash,
	        int frame = 0, bool rotate = false);

	// For adding custom images
	void put(const char* name, SDL_Surface* img, DWORD flags);

	/**
	 * Get an image index for if you're using a rotational object
	 */
	int indexFromAngle(double angle);

private:
	// Look for && load image not in db
	SDL_Surface* lookForImg(const char* name, int frame, bool rotates);

	struct ImgRef {
		char name[128];
		SDL_Surface** images;
	};

	std::map<int, ImgRef*> base;
	
	// Pre-render rotations with this number of angles. The more angles, the better
	//  it looks but it takes a bit longer to load and uses more memory.
	int imageAngles;
};

ImgBase* getImgBase();

#endif // _IMGBASE_H_

