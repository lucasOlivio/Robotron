#pragma once

#include "GameCharacter.h"

// TODO: Research a better approach to add common gameplay to characters without need of inheritance
class Shooter : public GameCharacter
{
protected:
	// Time it can shoot again
	float m_timeFireRate = 0;
	float m_timeLastShot = 0;

	std::string m_projectileName;

public:
	virtual ~Shooter() {};

	virtual void Update(double deltaTime);

	virtual void OnStart(iEvent* pEvent);

	virtual glm::vec3 GetProjectileDirection() = 0;

	virtual void Shoot();
};
