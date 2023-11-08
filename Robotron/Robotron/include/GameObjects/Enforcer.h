#pragma once

#include "Shooter.h"

class Enforcer : public Shooter
{
public:
	Enforcer() {};
	virtual ~Enforcer() {};

	virtual void Update(double deltaTime);

	void Move();

	// Shooter
	//------------
	virtual glm::vec3 GetProjectileDirection();

	// iCollider
	//------------
	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
