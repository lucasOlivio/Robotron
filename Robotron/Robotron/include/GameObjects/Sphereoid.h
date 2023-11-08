#pragma once

#include "NpcRandomWalk.h"

class Sphereoid : public NpcRandomWalk
{
private:
	double m_timeLastSpawn = 0;
	double m_timeSpawnRate;

	double m_timeTillDisappear;
	double m_timeElapsed = 0;

	std::string m_enforcerName;

public:
	Sphereoid() {};
	virtual ~Sphereoid() {};

	virtual void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision);
};
