#pragma once

#include "iGameObject.h"
#include "components/Tag.h"
#include "components/Transform.h"
#include "components/Force.h"
#include "components/Collision.h"
#include "components/Animation.h"
#include "components/Script.h"
#include "events/iCollisionListener.h"

// TODO: This whole game objects things should be rebuilt as a component design pattern

class GameCharacter : public iGameObject
{
protected:
	EntityID m_entityID;
	TagComponent* m_tag;
	ScriptComponent* m_script;
	TransformComponent* m_transform;
	ForceComponent* m_force;
	CollisionComponent* m_collision;
	AnimationComponent* m_animation;

	iGameMediator* m_pMediator;

	glm::vec3 m_currDirection;

	bool m_isDeleted;

	// Script custom
	//----------------
	float m_velocity;

public:
	GameCharacter() {};
	virtual ~GameCharacter() {};

	// Game object
	//----------------
	virtual bool Load(EntityID entityID, SceneView* pScene);

	virtual void OnStart(iEvent* pEvent);

	virtual void Update(double deltaTime) {};

	virtual void OnExit(iEvent* pEvent);

	virtual EntityID GetEntityID();

	virtual void SetMediator(iGameMediator* pMediator);
	virtual iGameMediator* GetMediator();

	virtual void SetPosition(glm::vec3 value);
	virtual void SetDirection(glm::vec3 value);

	virtual glm::vec3 GetPosition();
	virtual std::string GetTagName();
	virtual std::string GetScriptName();

	// Change the force velocity to be m_velocity * orientation
	virtual void Move(glm::vec3 orientation);

	virtual void Delete();
	virtual bool IsDeleted();

	// Collision listener
	//----------------
	virtual void Notify(iEvent* pEvent, sCollisionData* pCollision) {};
	virtual void Notify(iEvent* pEvent) {};
};
