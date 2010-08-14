#include "mountpoint.h"

MountPoint::MountPoint(Sprite* m, Vector off)
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

