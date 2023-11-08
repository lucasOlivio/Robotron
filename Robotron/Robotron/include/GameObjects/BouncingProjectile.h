#pragma once

#include "Projectile.h"

class BouncingProjectile : public Projectile
{
public:
	virtual ~BouncingProjectile() {};

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
