#pragma once

#include "iGameObject.h"
#include "iGameMediator.h"
#include "components/Tag.h"
#include "components/Script.h"

class GameManager : public iGameObject,
					public iGameMediator
{
private:
	SceneView* m_pSceneView;
	iEvent* m_pCollisionEvents;

	EntityID m_entityID;
	TagComponent* m_tag;
	ScriptComponent* m_script;

	// Child game objects
	//-------------------
	iGameObject* m_pPlayer;
	std::vector<iGameObject*> m_vecGameObjects; // All kinds of game objects used for the gameplay

	// Script custom
	//----------------
	// Number of enemies to place
	int m_nGrunts;
	int m_nHulks;
	int m_nBrains;
	int m_nSphereoids;
	int m_nTanks;

	// Number of humans to place
	int m_nDad;
	int m_nMom;
	int m_nMikey;

	int m_maxHeight; // Total height of arena
	int m_maxWidth; // Total width of arena
	int m_minHeight; // min height of arena
	int m_minWidth; // min width of arena

public:
	virtual ~GameManager();

	virtual void GameOver();

	// Game object
	//----------------
	virtual bool Load(EntityID entityID, SceneView* pScene);

	virtual void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime);

	virtual void OnExit(iEvent* pEvent);

	// TODO: Game manager could be a layer not a gameobject? so we wouldn`t need these
	virtual void SetPosition(glm::vec3 value) {};
	virtual void SetDirection(glm::vec3 value) {};
	virtual void SetMediator(iGameMediator* pMediator) {};

	virtual glm::vec3 GetPosition() { return glm::vec3(0); };
	virtual std::string GetTagName();
	virtual std::string GetScriptName();
	virtual iGameMediator* GetMediator() { return nullptr; };

	virtual EntityID GetEntityID();

	virtual void Delete() {};

	virtual bool IsDeleted() { return false; };

	// Game mediator
	//----------------
	virtual glm::vec3 GetPlayerDirection(glm::vec3 objPosition);
	virtual bool GetTargetDirection(std::string targetName, glm::vec3 objPosition, glm::vec3& directionOut);
	virtual void DestroyGameObj(iGameObject* pGameObj);
	virtual void SpawnGameObj(std::string tagName, glm::vec3 position, glm::vec3 direction);
	// Random spawn
	virtual void SpawnGameObj(std::string tagName);

	// Collision listener
	//----------------
	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision) {};
	virtual void Notify(iEvent* pEvent) {};
};
