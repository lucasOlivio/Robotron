#include "GameObjects/GameCharacter.h"
#include "GameObjects/GameObjectFactory.h"
#include "common/utilsMat.h"

bool GameCharacter::Load(EntityID entityID, SceneView* pScene)
{
	this->m_entityID = entityID;

	this->m_tag = pScene->GetComponent<TagComponent>(entityID, "tag");
	this->m_transform = pScene->GetComponent<TransformComponent>(entityID, "transform");
	this->m_script = pScene->GetComponent<ScriptComponent>(entityID, "script");
	this->m_force = pScene->GetComponent<ForceComponent>(entityID, "force");
	this->m_collision = pScene->GetComponent<CollisionComponent>(entityID, "collision");
	this->m_animation = pScene->GetComponent<AnimationComponent>(entityID, "animation");

    return true;
}

void GameCharacter::OnStart(iEvent* pEvent)
{
	this->m_currDirection = glm::vec3(0);

	// Script custom
	//----------------
	this->m_velocity = std::stof(this->m_script->GetVariable("velocity"));

	this->m_animation->SetActive(true);
	this->m_collision->SetActive(true);

	pEvent->Attach(this);
}

void GameCharacter::OnExit(iEvent* pEvent)
{
	pEvent->Dettach(this);
}

EntityID GameCharacter::GetEntityID()
{
    return this->m_entityID;
}

void GameCharacter::Move(glm::vec3 orientation)
{
	glm::vec3 newVelocity = this->m_velocity * orientation;
	this->m_force->SetVelocity(newVelocity);

	return;
}

void GameCharacter::Delete()
{
	this->m_isDeleted = true;
}

bool GameCharacter::IsDeleted()
{
	if (this->m_isDeleted)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameCharacter::SetMediator(iGameMediator* pMediator)
{
	this->m_pMediator = pMediator;
}

iGameMediator* GameCharacter::GetMediator()
{
	return this->m_pMediator;
}

void GameCharacter::SetPosition(glm::vec3 value)
{
	this->m_transform->SetPosition(value);
}

void GameCharacter::SetDirection(glm::vec3 value)
{
	using namespace glm;
	using namespace myutils;

	vec3 rotation = vec3(0.0f, 0.0f, GetAngleToDirection(value.x, value.y));
	this->m_transform->SetOrientation(rotation);

	this->m_currDirection = value;
}

glm::vec3 GameCharacter::GetPosition()
{
	return this->m_transform->GetPosition();
}

std::string GameCharacter::GetTagName()
{
	return this->m_tag->name;
}

std::string GameCharacter::GetScriptName()
{
	return this->m_script->GetScriptName();
}
