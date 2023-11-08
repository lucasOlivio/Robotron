#include "GameObjects/NpcFollowPlayer.h"
#include "common/utilsMat.h"

void NpcFollowPlayer::OnStart(iEvent* pEvent)
{
	GameCharacter::OnStart(pEvent);

	this->m_timeSeeking = std::stof(this->m_script->GetVariable("timeSeeking"));
}

void NpcFollowPlayer::Update(double deltaTime)
{
	this->m_timeElapsed += (float)deltaTime;

	this->Move();
}

void NpcFollowPlayer::Move()
{
	using namespace glm;

	if (this->m_timeElapsed < this->m_timeSeeking)
	{
		return;
	}

	this->m_timeElapsed = 0;

	vec3 direction = this->m_pMediator->GetPlayerDirection(this->GetPosition());

	GameCharacter::Move(direction);
}

void NpcFollowPlayer::Notify(iEvent* pEvent, sCollisionData* pCollision)
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
