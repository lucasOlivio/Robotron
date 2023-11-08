#include "GameObjects/TrackingProjectile.h"

void TrackingProjectile::OnStart(iEvent* pEvent)
{
	GameCharacter::OnStart(pEvent);

	this->m_timeSeeking = std::stof(this->m_script->GetVariable("timeSeeking"));
}

void TrackingProjectile::Update(double deltaTime)
{
	this->Projectile::Update(deltaTime);

	this->m_timeElapsed += (float)deltaTime;

	if (this->m_timeElapsed < this->m_timeSeeking)
	{
		return;
	}

	// Bullet follow the player direction always
	this->SetDirection(this->m_pMediator->GetPlayerDirection(this->GetPosition()));
}

void TrackingProjectile::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
	using namespace std;

	string collidedName = pCollision->tagA;
	if (pCollision->entityA == this->GetEntityID())
	{
		// This is the collider A so we get the other object it collided
		collidedName = pCollision->tagB;
	}

	if (collidedName == "playerbullet" || collidedName == "player")
	{
		this->m_pMediator->DestroyGameObj(this);
	}
}
