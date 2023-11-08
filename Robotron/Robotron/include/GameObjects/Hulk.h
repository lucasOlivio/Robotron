#pragma once

#include "NpcRandomWalk.h"

class Hulk : public NpcRandomWalk
{
public:
	Hulk() {};
	virtual ~Hulk() {};

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
