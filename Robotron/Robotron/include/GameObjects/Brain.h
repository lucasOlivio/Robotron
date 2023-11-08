#pragma once

#include "Shooter.h"

class Brain : public Shooter
{
private:
	// Time it will go in a direction before adjusting to targets position
	float m_timeSeeking;
	float m_timeElapsed;

	// TODO: Grouping or classification of game objects
	std::string m_targetScriptName;

public:
	Brain() {};
	virtual ~Brain() {};

	virtual void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	// Every "m_timeSeeking" time search for target and set new direction
	// If target not found, doesn`t move and returns false
	void Move();

	// Shooter
	//------------
	virtual glm::vec3 GetProjectileDirection();

	// iCollider
	//------------
	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
