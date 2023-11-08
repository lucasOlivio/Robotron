#include "GameObjects/Sphereoid.h"

void Sphereoid::OnStart(iEvent* pEvent)
{
	this->NpcRandomWalk::OnStart(pEvent);

	// Script custom
	//----------------
	this->m_timeSpawnRate = stod(this->m_script->GetVariable("timeSpawnRate"));
	this->m_timeTillDisappear = stod(this->m_script->GetVariable("timeTillDisappear"));
	this->m_enforcerName = this->m_script->GetVariable("enforcerName");
}

void Sphereoid::Update(double deltaTime)
{
	// After "m_timeTillDisappear" object is destroyed
	this->m_timeElapsed += (float)deltaTime;

	if (this->m_timeElapsed > this->m_timeTillDisappear)
	{
		this->m_pMediator->DestroyGameObj(this);
		return;
	}

	// Every "m_timeSpawnRate" will spawn an enforcer in this position
	this->m_timeLastSpawn += (float)deltaTime;

	if (this->m_timeLastSpawn < this->m_timeSpawnRate)
	{
		return;
	}

	this->m_pMediator->SpawnGameObj(this->m_enforcerName, this->GetPosition(), glm::vec3(0));

	this->m_timeLastSpawn = 0;
}

void Sphereoid::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
	using namespace std;

	string collidedName = pCollision->tagA;
	if (pCollision->entityA == this->GetEntityID())
	{
		// This is the collider A so we get the other object it collided
		collidedName = pCollision->tagB;
	}

	if (collidedName == "playerbullet")
	{
		this->m_pMediator->DestroyGameObj(this);
	}
}
