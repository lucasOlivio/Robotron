#include "GameObjects/Projectile.h"

void Projectile::OnStart(iEvent* pEvent)
{
	this->GameCharacter::OnStart(pEvent);

	glm::vec3 velocity = this->m_velocity * this->m_currDirection;

	this->m_force->SetVelocity(velocity);
}

void Projectile::Update(double deltaTime)
{
	this->GameCharacter::Update(deltaTime);

	// Just making sure the bullets are destroied so they don`t accumulate
	if (this->GetPosition().x > 1000 ||
		this->GetPosition().x < 0 ||
		this->GetPosition().y > 800 ||
		this->GetPosition().y < 100)
	{
		this->m_pMediator->DestroyGameObj(this);
	}
}

void Projectile::SetDirection(glm::vec3 value)
{
	this->GameCharacter::SetDirection(value);

	glm::vec3 velocity = this->m_velocity * this->m_currDirection;

	this->m_force->SetVelocity(velocity);
}

void Projectile::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
	using namespace std;

	string collidedName = pCollision->tagA;
	if (pCollision->entityA == this->GetEntityID())
	{
		// This is the collider A so we get the other object it collided
		collidedName = pCollision->tagB;
	}

	// TODO: should not collide with its original shooter, without need of checking all
	if (collidedName == "player" || collidedName == "enforcer" || collidedName == "tank")
	{
		return;
	}

	this->m_pMediator->DestroyGameObj(this);
}
