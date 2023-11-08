#include "GameObjects/BouncingProjectile.h"
#include "common/utilsMat.h"

void BouncingProjectile::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
	using namespace std;
	using namespace glm;
	using namespace myutils;

	string collidedName = pCollision->tagA;
	vec3 colNormal = pCollision->collisionNormalB;
	vec2 velocity = this->m_velocity * this->m_currDirection;
	if (pCollision->entityA == this->GetEntityID())
	{
		// This is the collider A so we get the other object it collided
		collidedName = pCollision->tagB;
		colNormal = pCollision->collisionNormalA;
	}

	if (collidedName == "playerbullet" || collidedName == "player")
	{
		this->m_pMediator->DestroyGameObj(this);
	}
}
