#include "GameObjects/GameObjectFactory.h"
#include "GameObjects/GameManager.h"
#include "GameObjects/Player.h"
#include "GameObjects/Grunt.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Human.h"
#include "GameObjects/Prog.h"
#include "GameObjects/Hulk.h"
#include "GameObjects/Sphereoid.h"
#include "GameObjects/Enforcer.h"
#include "GameObjects/TrackingProjectile.h"
#include "GameObjects/Brain.h"
#include "GameObjects/Tank.h"
#include "GameObjects/BouncingProjectile.h"

GameObjectFactory::GameObjectFactory(SceneView* pScene)
{
	this->m_pScene = pScene;
}

iGameObject* GameObjectFactory::CreateGameObject(std::string gameObjName, bool isCopy)
{
	iGameObject* pGameObj;

	EntityID entityID = this->m_pScene->GetEntity(gameObjName);

	// Should we use the entity itself or just a copy of it?
	if (isCopy)
	{
		entityID = this->m_pScene->CreateEntity(entityID);
	}

	ScriptComponent* pScript = this->m_pScene->GetComponent<ScriptComponent>(entityID, "script");
	std::string scriptName = pScript->GetScriptName();

	if (scriptName == "GameManager")
	{
		pGameObj = new GameManager();
	}
	else if (scriptName == "Player")
	{
		pGameObj = new Player();
	}
	else if (scriptName == "Grunt")
	{
		pGameObj = new Grunt();
	}
	else if (scriptName == "Projectile")
	{
		pGameObj = new Projectile();
	}
	else if (scriptName == "Human")
	{
		pGameObj = new Human();
	}
	else if (scriptName == "Prog")
	{
		pGameObj = new Prog();
	}
	else if (scriptName == "Hulk")
	{
		pGameObj = new Hulk();
	}
	else if (scriptName == "Sphereoid")
	{
		pGameObj = new Sphereoid();
	}
	else if (scriptName == "Enforcer")
	{
		pGameObj = new Enforcer();
	}
	else if (scriptName == "TrackingProjectile")
	{
		pGameObj = new TrackingProjectile();
	}
	else if (scriptName == "Brain")
	{
		pGameObj = new Brain();
	}
	else if (scriptName == "Tank")
	{
		pGameObj = new Tank();
	}
	else if (scriptName == "BouncingProjectile")
	{
		pGameObj = new BouncingProjectile();
	}
	else
	{
		// Should not return null
		assert(nullptr);
		return nullptr;
	}

	bool isLoaded = pGameObj->Load(entityID, this->m_pScene);
	if (!isLoaded)
	{
		delete pGameObj;
		return nullptr;
	}

	return pGameObj;
}
