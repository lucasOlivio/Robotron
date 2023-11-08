#include "GameObjects/NpcRandomWalk.h"
#include "common/utilsMat.h"

void NpcRandomWalk::OnStart(iEvent* pEvent)
{
	GameCharacter::OnStart(pEvent);

	this->m_timeWalking = std::stof(this->m_script->GetVariable("timeWalking"));
}

void NpcRandomWalk::Update(double deltaTime)
{
	this->m_timeElapsed += (float)deltaTime;

	this->Move();
}

void NpcRandomWalk::Move()
{
	using namespace glm;

	if (this->m_timeElapsed < this->m_timeWalking)
	{
		return;
	}

	this->m_timeElapsed = 0;

	const vec3 directions[8] = {
		vec3(1, 0, 0),
		vec3(1, 1, 0),
		vec3(0, 1, 0),
		vec3(-1, 1, 0),
		vec3(-1, 0, 0),
		vec3(-1, -1, 0),
		vec3(0, -1, 0),
		vec3(1, -1, 0)
	};

	int i = (int)myutils::GetRandFloat(0, 8);
	vec3 direction = directions[i];

	GameCharacter::Move(direction);
}

void NpcRandomWalk::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
}
