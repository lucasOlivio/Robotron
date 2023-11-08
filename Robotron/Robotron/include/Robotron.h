#pragma once

#include "Engine.h"
#include "GameObjects/iGameObject.h"

class Robotron : public Engine
{
private:
	iGameObject* m_pGM;
public:
	Robotron();
	virtual ~Robotron();

	// Instantiate game manager and set it to load all his info
	void LoadGM();

	virtual void Update(double fixedDeltaTime);

	virtual void ChangeMode();
};