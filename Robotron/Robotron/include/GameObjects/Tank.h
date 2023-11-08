#pragma once

#include "Shooter.h"

class Tank : public Shooter
{
public:
	Tank() {};
	virtual ~Tank() {};

	virtual void Update(double deltaTime);

	void Move();

	// Shooter
	//------------
	virtual glm::vec3 GetProjectileDirection();

	// iCollider
	//------------
	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
