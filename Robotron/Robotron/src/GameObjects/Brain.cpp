#include "GameObjects/Brain.h"
#include "common/utilsMat.h"

void Brain::OnStart(iEvent* pEvent)
{
	Shooter::OnStart(pEvent);

	this->m_timeSeeking = std::stof(this->m_script->GetVariable("timeSeeking"));
	this->m_targetScriptName = this->m_script->GetVariable("targetScriptName");
}

void Brain::Update(double deltaTime)
{
	this->Shooter::Update(deltaTime);

	this->m_timeElapsed += (float)deltaTime;

	this->Move();

	this->Shoot();
}

void Brain::Move()
{
	using namespace glm;

	if (this->m_timeElapsed < this->m_timeSeeking)
	{
		return;
	}

	this->m_timeElapsed = 0;

	vec3 direction;
		
	bool targetFound = this->m_pMediator->GetTargetDirection(this->m_targetScriptName, 
															 this->GetPosition(), 
															 direction);

	if (!targetFound)
	{
		// No target left, so seek player
		direction = this->m_pMediator->GetPlayerDirection(this->GetPosition());
	}

	Shooter::Move(direction);
	return;
}

glm::vec3 Brain::GetProjectileDirection()
{
	using namespace glm;
	using namespace myutils;

	// Get direction
	return this->m_pMediator->GetPlayerDirection(this->GetPosition());
}

void Brain::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
	using namespace std;

	string collidedName = pCollision->tagA;
	if (pCollision->entityA == this->GetEntityID())
	{
		// This is the collider A so we get the other object it collided
		collidedName = pCollision->tagB;
	}

	// Check if its colliding with non-enemies
	if (collidedName == "playerbullet")
	{
		// If colliding with bullet, just die
		this->m_pMediator->DestroyGameObj(this);
	}
}
