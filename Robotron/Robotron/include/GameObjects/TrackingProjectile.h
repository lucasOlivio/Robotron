#pragma once

#include "Projectile.h"

class TrackingProjectile : public Projectile
{
private:
	// Time it will go in a direction before adjusting to players position
	float m_timeSeeking;
	float m_timeElapsed;

public:
	virtual ~TrackingProjectile() {};

	void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
