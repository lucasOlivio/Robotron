#pragma once

#include "GameCharacter.h"
#include "Shooter.h"

class Player : public Shooter
{
public:
	Player() {};
	virtual ~Player() {};

	virtual void Update(double deltaTime);

	void Move();

	// Shooter
	//------------
	virtual glm::vec3 GetProjectileDirection();

	// iCollider
	//------------
	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
