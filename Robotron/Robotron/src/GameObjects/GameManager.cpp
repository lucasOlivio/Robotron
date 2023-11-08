#include "GameObjects/GameManager.h"
#include "GameObjects/GameObjectFactory.h"
#include "common/utils.h"
#include "common/utilsMat.h"
#include "components/Transform.h"

GameManager::~GameManager()
{
}

void GameManager::GameOver()
{
	for (int i = 0; i < this->m_vecGameObjects.size(); i++)
	{
		this->DestroyGameObj(this->m_vecGameObjects[i]);
	}
	glm::vec3 gameOverPosition(500, 500, 0);
	EntityID gameOverID = this->m_pSceneView->GetEntity("gameover");
	TransformComponent* gameOverTransform = this->m_pSceneView->GetComponent<TransformComponent>(gameOverID, "transform");
	gameOverTransform->SetPosition(gameOverPosition);
}

bool GameManager::Load(EntityID entityID, SceneView* pScene)
{
	this->m_pSceneView = pScene;
	this->m_entityID = entityID;

	this->m_tag = pScene->GetComponent<TagComponent>(entityID, "tag");
	this->m_script = pScene->GetComponent<ScriptComponent>(entityID, "script");

	return true;
}

void GameManager::OnStart(iEvent* pEvent)
{
	using namespace std;
	using namespace myutils;
	using namespace glm;

	this->m_pCollisionEvents = pEvent;

	// Script custom
	//----------------
	this->m_nGrunts = stoi(this->m_script->GetVariable("nGrunts"));
	this->m_nHulks = stoi(this->m_script->GetVariable("nHulks"));
	this->m_nBrains = stoi(this->m_script->GetVariable("nBrains"));
	this->m_nSphereoids = stoi(this->m_script->GetVariable("nSphereoids"));
	this->m_nTanks = stoi(this->m_script->GetVariable("nTanks"));
	this->m_nDad = stoi(this->m_script->GetVariable("nDad"));
	this->m_nMom = stoi(this->m_script->GetVariable("nMom"));
	this->m_nMikey = stoi(this->m_script->GetVariable("nMikey"));
	this->m_maxHeight = stoi(this->m_script->GetVariable("maxHeight"));
	this->m_maxWidth = stoi(this->m_script->GetVariable("maxWidth"));
	this->m_minHeight = stoi(this->m_script->GetVariable("minHeight"));
	this->m_minWidth = stoi(this->m_script->GetVariable("minWidth"));

	GameObjectFactory gameObjFactory(this->m_pSceneView);

	// Load player
	//----------------
	this->m_pPlayer = gameObjFactory.CreateGameObject("player", false);
	this->m_pPlayer->SetMediator(this);
	this->m_pPlayer->OnStart(pEvent);

	// Load enemies
	//----------------
	for (int i = 0; i < this->m_nGrunts; i++)
	{
		this->SpawnGameObj("grunt");
	}
	for (int i = 0; i < this->m_nHulks; i++)
	{
		this->SpawnGameObj("hulk");
	}
	for (int i = 0; i < this->m_nBrains; i++)
	{
		this->SpawnGameObj("brain");
	}
	for (int i = 0; i < this->m_nSphereoids; i++)
	{
		this->SpawnGameObj("sphereoid");
	}
	for (int i = 0; i < this->m_nTanks; i++)
	{
		this->SpawnGameObj("tank");
	}

	// Load humans
	//----------------
	for (int i = 0; i < this->m_nDad; i++)
	{
		this->SpawnGameObj("dad");
	}
	for (int i = 0; i < this->m_nMom; i++)
	{
		this->SpawnGameObj("mom");
	}
	for (int i = 0; i < this->m_nMikey; i++)
	{
		this->SpawnGameObj("mikey");
	}
}

void GameManager::Update(double deltaTime)
{
	using namespace std;

	// Update all objects
	this->m_pPlayer->Update(deltaTime);

	for(int i = 0; i < this->m_vecGameObjects.size(); i++)
	{
		iGameObject* pGameObj = this->m_vecGameObjects[i];
		pGameObj->Update(deltaTime);
	}

	// TODO: We shouldnt iterate over the vector twice to delete, trying to delete in the first loop didn't work
	// because we are creating and deleting elements from all over the vector.
	// 
	// Clear deleted from vector
	std::vector<iGameObject*>::iterator it = this->m_vecGameObjects.begin();
	while (it != this->m_vecGameObjects.end())
	{
		iGameObject* pGameObj = (*it);
		if (pGameObj->IsDeleted())
		{
			delete pGameObj;
			it = this->m_vecGameObjects.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameManager::OnExit(iEvent* pEvent)
{
	for (iGameObject* pGameObj : this->m_vecGameObjects)
	{
		pGameObj->OnExit(pEvent);
		delete pGameObj;
	}

	this->m_pPlayer->OnExit(pEvent);
	delete this->m_pPlayer;
}

std::string GameManager::GetTagName()
{
	return this->m_tag->name;
}

std::string GameManager::GetScriptName()
{
	return this->m_script->GetScriptName();
}

EntityID GameManager::GetEntityID()
{
	return this->m_entityID;
}

glm::vec3 GameManager::GetPlayerDirection(glm::vec3 objPosition)
{
	glm::vec3 playerPosition = this->m_pPlayer->GetPosition();
	return myutils::GetCardinalDirection(playerPosition, objPosition);
}

bool GameManager::GetTargetDirection(std::string targetName, glm::vec3 objPosition, glm::vec3& directionOut)
{
	for (iGameObject* pGameObj : this->m_vecGameObjects)
	{
		if (pGameObj->GetScriptName() == targetName)
		{
			glm::vec3 targetPosition = pGameObj->GetPosition();
			directionOut = myutils::GetCardinalDirection(targetPosition, objPosition);
			return true;
		}
	}

	return false;
}

void GameManager::DestroyGameObj(iGameObject* pGameObj)
{
	using namespace std;

	string tagName = pGameObj->GetTagName();

	if (tagName == "player")
	{
		this->GameOver();
	}

	pGameObj->Delete();
	// Clean from scene
	this->m_pSceneView->DeleteEntity(pGameObj->GetEntityID());
	this->m_pSceneView->DeleteListener(pGameObj);
}

void GameManager::SpawnGameObj(std::string tagName, glm::vec3 position, glm::vec3 direction)
{
	using namespace glm;
	using namespace myutils;

	GameObjectFactory gameObjFactory(this->m_pSceneView);

	iGameObject* pGameObj = gameObjFactory.CreateGameObject(tagName, true);

	pGameObj->SetMediator(this);
	pGameObj->OnStart(this->m_pCollisionEvents);

	pGameObj->SetPosition(position);
	pGameObj->SetDirection(direction);

	this->m_vecGameObjects.push_back(pGameObj);
}

void GameManager::SpawnGameObj(std::string tagName)
{
	using namespace glm;
	using namespace myutils;

	// Avoid enemy been placed at player position
	float playerCenterMin = 450;
	float playerCenterMax = 550;

	vec3 position(0);
	position.x = GetRandFloat((float)this->m_minWidth, (float)this->m_maxWidth, playerCenterMin, playerCenterMax);
	position.y = GetRandFloat((float)this->m_minHeight, (float)this->m_maxHeight, playerCenterMin, playerCenterMax);

	this->SpawnGameObj(tagName, position, vec3(0));
}
