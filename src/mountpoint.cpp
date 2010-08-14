#include "mountpoint.h"

MountPoint::MountPoint(Sprite* m, Vector off)
	: mount(m), offset(off)
{
}

Orientation MountPoint::getOrientation() const
{
	return mount->getOrientation() + offset;
}

