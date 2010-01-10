#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

// For loading directories
#include <dirent.h>

#include "common.h"
#include "conf.h"
#include "imgbase.h"

// Pre-render rotations with this number of angles
//  more looks better but takes a bit longer to load
//  and uses more memory
int imageAngles;

ImgBase images;


void InitImgBase()
{
	imageAngles = globals->getInt("Graphics", "Pre-rendered angles", 36);
}

int INDEX_FROM_ANGLE(double angle)
{
	angle += 2 * M_PI / imageAngles / 2;
	while (angle > 2 * M_PI)
		angle -= 2 * M_PI;
	while (angle < 0)
		angle += 2 * M_PI;
	return floor(angle * imageAngles / (2*M_PI));
}


SDL_Surface* ImgBase::getImage(const char* name, int frame, bool rotates)
{
	int h = hash(name);
	std::map<int,ImgRef>::iterator img;
	SDL_Surface* sur;

	// finally, just get image
	img = base.find(h);
	if (img != base.end()) {
		ImgRef* ir = &img->second;
		sur = ir->images[frame];
		return sur;
	}

	return lookForImg(name, frame, rotates);
}

SDL_Surface* ImgBase::lookForImg(const char* name, int frame, bool rotates)
{
	SDL_Surface* sur;

	DIR* dir = opendir("gfx");
	if (dir == NULL) {
		fprintf(stderr, "Couldn't open directory 'gfx'\n");
		Quit(1);
	}

	dirent* dent = NULL;
	char* fileName = NULL;
	while ((dent = readdir(dir))) {
		if (startsWith(name, dent->d_name)) { 
			fileName = dent->d_name;
			break;
		}
	}

	closedir(dir);

	if (fileName == NULL) {
		fprintf(stderr, "Couldn't find the '%s' graphic\n", name);
		throw;
	}

	char fnbuf[512];
	sprintf(fnbuf, "gfx/%s", fileName);
	sur = IMG_Load(fnbuf);

	DWORD flags = IB_PUT;
	if (rotates)
		flags |= IB_IMAGE_ROTATES;
	put(name, sur, flags);

	if (rotates)
		return getImage(name, frame);
	else
		return sur;
}


void ImgBase::put(const char* name, SDL_Surface* img, DWORD flags)
{
	int h = hash(name);
	ImgRef* ref = (ImgRef*)malloc(sizeof(ImgRef));
	ref->images = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * imageAngles);
	strcpy(ref->name, name);

	bool rotate = (flags & IB_IMAGE_ROTATES) != 0;

	// Add and pre-render rotations
	if (rotate) {
		ref->images[0] = img;
		for (int i = 1; i < imageAngles; i++) {
			SDL_Surface* sur = rotozoomSurface(img, 360/imageAngles*i,
					1, SMOOTHING_ON);
			ref->images[i] = sur;
		}
	}

	// Just add
	else {
		ref->images[0] = img;
		for (int i = 1; i < imageAngles; i++)
			ref->images[i] = NULL;
	}

	// Save
	base[h] = *ref;
}


