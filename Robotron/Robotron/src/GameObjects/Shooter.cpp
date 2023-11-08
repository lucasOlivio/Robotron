#include "GameObjects/Shooter.h"

void Shooter::Update(double deltaTime)
{
	this->GameCharacter::Update(deltaTime);

	this->m_timeLastShot += (float)deltaTime;
}

void Shooter::OnStart(iEvent* pEvent)
{
	this->GameCharacter::OnStart(pEvent);

	// Script custom
	//----------------
	this->m_timeFireRate = std::stof(this->m_script->GetVariable("timeFireRate"));
	this->m_projectileName = this->m_script->GetVariable("projectileName");

	// Don`t start shooting
	this->m_timeLastShot = 0;
}

void Shooter::Shoot()
{
	using namespace glm;

	if (this->m_timeLastShot < this->m_timeFireRate)
	{
		return;
	}

	vec3 direction = this->GetProjectileDirection();

	this->GetMediator()->SpawnGameObj(this->m_projectileName, this->GetPosition(), direction);

	this->m_timeLastShot = 0;
}
