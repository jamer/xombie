#ifndef _MOUNT_POINT_H_
#define _MOUNT_POINT_H_

/*
 * A mount point is a point in space that is at a relative offset to another,
 * often mobile, object.
 *
 * E.g.: A weapon is mounted on a character's hand.
 */

#include "orientation.h"
#include "sprite.h"
#include "point.h"

class MountPoint
{
public:
	MountPoint(Sprite* mount, Point offset);
	~MountPoint();

	Orientation getOrientation() const;

private:
	Sprite* mount;
	Point offset;
};

#endif
