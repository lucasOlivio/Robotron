#pragma once

#include "GameCharacter.h"

class Projectile : public GameCharacter
{
public:
	virtual ~Projectile() {};

	virtual void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	virtual void SetDirection(glm::vec3 value);

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
