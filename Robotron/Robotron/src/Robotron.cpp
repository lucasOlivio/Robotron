#include "Robotron.h"
#include "GameObjects/GameObjectFactory.h"

Robotron::Robotron()
	: Engine::Engine()
{
	this->m_pGM = nullptr;
}

Robotron::~Robotron()
{
	delete this->m_pGM;
}

void Robotron::LoadGM()
{
	GameObjectFactory gameObjFactory(this->m_pScene);
	this->m_pGM = gameObjFactory.CreateGameObject("GM", false);

	if (this->m_pGM)
	{
		return;
	}
	else
	{
		CheckEngineError("Error loading Game manager object");
		return;
	}
}

void Robotron::Update(double fixedDeltaTime)
{
	Engine::Update(fixedDeltaTime);

	if (this->m_pScene->IsPlaying())
	{
		this->m_pGM->Update(fixedDeltaTime);
	}
}

void Robotron::ChangeMode()
{
	this->Engine::ChangeMode();

	if (this->m_pScene->IsPlaying())
	{
		this->LoadGM();
		this->m_pGM->OnStart(this->m_pCollisionEvent);
	}
	else
	{
		this->m_pGM->OnExit(this->m_pCollisionEvent);
		delete this->m_pGM;
		this->m_pGM = NULL;
	}
}
