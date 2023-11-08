#include "GameObjects/Enforcer.h"
#include "common/Input.h"
#include "common/utilsMat.h"
#include "common/opengl.h" // TODO: Remove this necessity, abstract key buttons values

void Enforcer::Update(double deltaTime)
{
	this->Shooter::Update(deltaTime);

	this->Move();

	this->Shoot();
}

void Enforcer::Move()
{
	GameCharacter::Move(this->m_pMediator->GetPlayerDirection(this->GetPosition()));
}

glm::vec3 Enforcer::GetProjectileDirection()
{
	using namespace glm;
	using namespace myutils;

	// Get direction
	return this->m_pMediator->GetPlayerDirection(this->GetPosition());
}

void Enforcer::Notify(iEvent* pEvent, sCollisionData* pCollision)
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
		this->m_pMediator->DestroyGameObj(this);
	}
}
