#pragma once

#include "GameCharacter.h"

class NpcFollowPlayer : public GameCharacter
{
private:
	// Time it will go in a direction before adjusting to players position
	float m_timeSeeking;
	float m_timeElapsed;

public:
	NpcFollowPlayer() {};
	virtual ~NpcFollowPlayer() {};

	virtual void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	// Every "m_timeSeeking" time search for player and set new direction
	void Move();

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
