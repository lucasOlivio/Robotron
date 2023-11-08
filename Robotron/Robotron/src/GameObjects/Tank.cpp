#include "GameObjects/Tank.h"
#include "common/Input.h"
#include "common/utilsMat.h"
#include "common/opengl.h" // TODO: Remove this necessity, abstract key buttons values

void Tank::Update(double deltaTime)
{
	this->Shooter::Update(deltaTime);

	this->Move();

	this->Shoot();
}

void Tank::Move()
{
	GameCharacter::Move(this->m_pMediator->GetPlayerDirection(this->GetPosition()));
}

glm::vec3 Tank::GetProjectileDirection()
{
	using namespace glm;
	using namespace myutils;

	// Get direction
	return this->m_pMediator->GetPlayerDirection(this->GetPosition());
}

void Tank::Notify(iEvent* pEvent, sCollisionData* pCollision)
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
