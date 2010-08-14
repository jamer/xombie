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
#include "vector.h"

class MountPoint
{
public:
	MountPoint(Sprite* mount, Vector offset);
	Orientation getOrientation() const;

private:
	Sprite* mount;
	Vector offset;
};

#endif // _MOUNT_POINT_H_

