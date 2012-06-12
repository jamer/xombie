#include "mountpoint.h"

MountPoint::MountPoint(Sprite* m, Point off)
	: mount(m), offset(off)
{
}

MountPoint::~MountPoint()
{
}

Orientation MountPoint::getOrientation() const
{
	return mount->getOrientation() + offset;
}
