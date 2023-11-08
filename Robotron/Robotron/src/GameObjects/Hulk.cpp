#include "GameObjects/Hulk.h"

void Hulk::Notify(iEvent* pEvent, sCollisionData* pCollision)
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
		// Slow down by 1% each shot
		this->m_force->SetVelocity(this->m_force->GetVelocity() * 0.99f);
		this->m_transform->SetOldPosition(10);
	}
}
