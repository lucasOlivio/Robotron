#pragma once

#include "NpcRandomWalk.h"

class Human : public NpcRandomWalk
{
public:
	Human() {};
	virtual ~Human() {};

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
