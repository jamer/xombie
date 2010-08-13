#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include "SDL_rotozoom.h"

// For loading directories
#include <dirent.h>

#include "common.h"
#include "conf.h"
#include "engine.h"
#include "imgbase.h"

ImgBase* images;

int ImgBase::indexFromAngle(double angle)
{
	angle += 2 * M_PI / imageAngles / 2;
	while (angle > 2 * M_PI)
		angle -= 2 * M_PI;
	while (angle < 0)
		angle += 2 * M_PI;
	return (int)floor(angle * imageAngles / (2*M_PI));
}

ImgBase::ImgBase()
{
	images = this;
	imageAngles = globals->getInt("Graphics", "Pre-rendered angles", 36);	
}

ImgBase::~ImgBase()
{
	QHash<QString,ImgRef*>::iterator it;
	for (it = base.begin(); it != base.end(); it++) {
		ImgRef* ref = it.value();
		for (int i = 0; i < imageAngles; i++)
			if (ref->images[i])
				SDL_FreeSurface(ref->images[i]);
		delete[] ref->images;
		delete ref;
	}
	base.clear();
}

SDL_Surface* ImgBase::getImage(QString name, int frame, bool rotates)
{
	QHash<QString,ImgRef*>::iterator img;

	// finally, just get image
	img = base.find(name);
	if (img != base.end()) {
		ImgRef* ir = img.value();
		return ir->images[frame];
	}

	return lookForImg(name, frame, rotates);
}

SDL_Surface* ImgBase::lookForImg(QString search, int frame, bool rotates)
{
	SDL_Surface* sur;

	DIR* dir = opendir("gfx");
	if (dir == NULL) {
		fprintf(stderr, "Couldn't open directory 'gfx'\n");
		Quit(1);
	}

	dirent* dent = NULL;
	QString fileName;
	while ((dent = readdir(dir))) {
		QString fname(dent->d_name);
		if (fname.startsWith(search)) {
			fileName = fname;
			break;
		}
	}

	closedir(dir);

	if (fileName.isEmpty()) {
		fprintf(stderr, "Couldn't find the '%s' graphic\n",
				search.toUtf8().data());
		throw;
	}

	QString fnbuf = QString("gfx/%s").arg(fileName);
	sur = IMG_Load(fnbuf.toUtf8().data());

	DWORD flags = IB_PUT;
	if (rotates)
		flags |= IB_IMAGE_ROTATES;
	put(search, sur, flags);

	if (rotates)
		return getImage(search, frame);
	else
		return sur;
}


void ImgBase::put(QString name, SDL_Surface* img, DWORD flags)
{
	ImgRef* ref = new ImgRef;
	ref->images = new SDL_Surface*[imageAngles];
	ref->name = name;

	bool rotate = (flags & IB_IMAGE_ROTATES) != 0;

	// Add and pre-render rotations
	if (rotate) {
		ref->images[0] = img;
		for (int i = 1; i < imageAngles; i++) {
			SDL_Surface* sur = rotozoomSurface(img,
					360/imageAngles*i, 1, SMOOTHING_ON);
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
	base[name] = ref;
}

ImgBase* getImgBase()
{
	return images;
}

