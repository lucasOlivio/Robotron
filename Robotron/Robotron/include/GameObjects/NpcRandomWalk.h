#pragma once

#include "GameCharacter.h"

class NpcRandomWalk : public GameCharacter
{
private:
	// Time it will go in a direction before adjusting to players position
	float m_timeWalking;
	float m_timeElapsed;

public:
	NpcRandomWalk() {};
	virtual ~NpcRandomWalk() {};

	virtual void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	// Every "m_timeWalking" time search for player and set new direction
	void Move();

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
