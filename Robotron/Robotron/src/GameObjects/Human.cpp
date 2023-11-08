#include "GameObjects/Human.h"
#include "common/utilsMat.h"

void Human::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
	using namespace std;

	string collidedName = pCollision->tagA;
	if (pCollision->entityA == this->GetEntityID())
	{
		// This is the collider A so we get the other object it collided
		collidedName = pCollision->tagB;
	}

	// bullets and walls shouldn't kill human
	if (collidedName != "player" &&
		collidedName != "brain")
	{
		return;
	}

	this->m_pMediator->DestroyGameObj(this);

	if (collidedName != "brain")
	{
		return;
	}

	string progName = "prog" + this->GetTagName();
	this->m_pMediator->SpawnGameObj(progName, this->GetPosition(), this->m_currDirection);
}
