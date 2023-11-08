#include "GameObjects/Player.h"
#include "common/Input.h"
#include "common/utilsMat.h"
#include "common/opengl.h" // TODO: Remove this necessity, abstract key buttons values

void Player::Update(double deltaTime)
{
	this->Shooter::Update(deltaTime);

	this->Move();

	if (Input::IsKeyPressed(GLFW_KEY_SPACE))
	{
		this->Shoot();
	}
}

void Player::Move()
{
	// Vertical movement
	if (Input::IsKeyPressed(GLFW_KEY_W))
	{
		this->m_currDirection.y = 1;
	}
	else if (Input::IsKeyPressed(GLFW_KEY_S))
	{
		this->m_currDirection.y = -1;
	}

	// Horizontal movement
	if (Input::IsKeyPressed(GLFW_KEY_D))
	{
		this->m_currDirection.x = 1;
	}
	else if (Input::IsKeyPressed(GLFW_KEY_A))
	{
		this->m_currDirection.x = -1;
	}

	GameCharacter::Move(this->m_currDirection);

	this->m_currDirection = glm::vec3(0);
}

glm::vec3 Player::GetProjectileDirection()
{
	using namespace glm;
	using namespace myutils;

	vec2 mousePosition = Input::MousePosRelative();

	// Get direction
	return GetCardinalDirection({mousePosition, 0.0f}, this->GetPosition());
}

void Player::Notify(iEvent* pEvent, sCollisionData* pCollision)
{
	using namespace std;

	string collidedName = pCollision->tagA;
	if (pCollision->entityA == this->GetEntityID())
	{
		// This is the collider A so we get the other object it collided
		collidedName = pCollision->tagB;
	}

	// Check if its colliding with non-enemies
	if (collidedName == "wall" || 
		collidedName == "dad" ||
		collidedName == "mom" ||
		collidedName == "mikey" ||
		collidedName == "playerbullet")
	{
		return;
	}

	// If colliding with enemy, then just die and game over
	this->m_pMediator->DestroyGameObj(this);
}
