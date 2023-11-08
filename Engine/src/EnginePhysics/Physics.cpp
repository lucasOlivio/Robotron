#include "EnginePhysics/Physics.h"
#include "common/utilsMat.h"
#include "components/Tag.h"
#include <glm/gtx/string_cast.hpp>

Physics::Physics(SceneView* pSceneView, CollisionEvent* pCollisionEvent)
{
	this->m_isRunning = false;
	this->m_pSceneView = pSceneView;
	this->m_pCollisionEvent = pCollisionEvent;
}

Physics::~Physics()
{

}

void Physics::m_ApplyForce(ForceComponent* pForce, TransformComponent* pTransform, double deltaTime)
{
	// Check if object have mass
	if (pForce->GetInverseMass() <= 0.0f)
	{
		return;
	}

	// Explicit forward Euler "integration step"
	//	NewVelocity = LastVel + (Accel * DeltaTime)
	//	NewPosition = LastPos + (Vel * DeltaTime)	

	// Calculate new velocity this frame based on 
	// delta time, acceleration and current velocity
	glm::vec3 velThisFrame = (pForce->GetAcceleration() * (float)deltaTime) + pForce->GetVelocity();
	pForce->SetVelocity(velThisFrame);
	// New object position
	glm::vec3 deltaPosition = velThisFrame * (float)deltaTime;

	pTransform->Move(deltaPosition);

	return;
}

void Physics::m_CheckCollisions(EntityID entityA, CollisionComponent* pCollA, TransformComponent* pTransformA)
{
	using namespace std;
	using namespace glm;

	bool isIntersecting = false;
	vec3 collisionNormalA = vec3(0);
	vec3 collisionNormalB = vec3(0);

	if (!pCollA->IsActive())
	{
		return;
	}

	if (pCollA->Get_eBodyType() == eBodyType::STATIC)
	{
		this->m_vecCollVisited.push_back(entityA);
		return;
	}

	mat4 transformMatA = pTransformA->GetTransformNoRotation();
	vec3 contactPointA(0);
	vec3 contactPointB(0);

	// Go through list of already visited components, this way we are sure we'll only test 1 time each
	for (EntityID entityB : this->m_vecCollVisited)
	{
		bool isCollision = false;
		CollisionComponent* pCollB = this->m_pSceneView->GetComponent<CollisionComponent>(entityB, "collision");
		TransformComponent* pTransformB = this->m_pSceneView->GetComponent<TransformComponent>(entityB, "transform");
		mat4 transformMatB = pTransformB->GetTransformNoRotation();

		if (pCollA->Get_eShape() == eShape::AABB2D && pCollB->Get_eShape() == eShape::AABB2D)
		{
			sAABB2D* pAABB2D_A = pCollA->GetShape<sAABB2D>();
			sAABB2D* pAABB2D_B = pCollB->GetShape<sAABB2D>();

			isCollision = this->AABBAABB2D_Test(pAABB2D_A, transformMatA, pAABB2D_B, transformMatB, 
												contactPointA, contactPointB, collisionNormalA, collisionNormalB);
		}
		else
		{
			// Collision test not implemented yet
			isCollision = false;
		}

		if (!isCollision)
		{
			continue;
		}

		TagComponent* tagA = this->m_pSceneView->GetComponent<TagComponent>(entityA, "tag");
		TagComponent* tagB = this->m_pSceneView->GetComponent<TagComponent>(entityB, "tag");

		// Set all collision data needed for others to handle it
		sCollisionData* pCollision = new sCollisionData();
		pCollision->entityA = entityA;
		pCollision->entityB = entityB;
		pCollision->tagA = tagA->name;
		pCollision->tagB = tagB->name;
		pCollision->bodyTypeA = pCollA->Get_eBodyType();
		pCollision->bodyTypeB = pCollB->Get_eBodyType();
		pCollision->collisionNormalA = collisionNormalA;
		pCollision->collisionNormalB = collisionNormalB;
		pCollision->contactPointA = contactPointA;
		pCollision->contactPointB = contactPointB;
		pCollision->positionA = pTransformA->GetPosition();
		pCollision->positionB = pTransformB->GetPosition();

		// Add to vector that will be later sent as notification
		this->m_vecCollided.push_back(pCollision);
	}

	this->m_vecCollVisited.push_back(entityA);

	return;
}

// TODO: Entity A will never be static body here right? because we just jump them
void Physics::m_ResolveCollision(sCollisionData* pCollisionEvent, TransformComponent* pTransformA, 
								TransformComponent* pTransformB, ForceComponent* pForceA, ForceComponent* pForceB)
{
	using namespace glm;

	vec3 velocityA = vec3(0);
	vec3 velocityB = vec3(0);

	float inverseMassA = 0;
	float inverseMassB = 0;

	float restitutionA = 0;
	float restitutionB = 0;

	if (pForceA)
	{
		velocityA = pForceA->GetVelocity();
		inverseMassA = pForceA->GetInverseMass();
		restitutionA = pForceA->GetRestitution();
	}

	if (pForceB)
	{
		velocityB = pForceB->GetVelocity();
		inverseMassB = pForceB->GetInverseMass();
		restitutionB = pForceB->GetRestitution();
	}


	if (pCollisionEvent->bodyTypeA == eBodyType::STATIC)
	{
		pTransformB->SetOldPosition(2);
	}
	if (pCollisionEvent->bodyTypeB == eBodyType::STATIC)
	{
		pTransformA->SetOldPosition(2);
	}

	// Recalculate velocity based on inverse mass
	if (pCollisionEvent->bodyTypeA == eBodyType::DYNAMIC && pCollisionEvent->bodyTypeB == eBodyType::STATIC)
	{
		myutils::ResolveVelocity(velocityA, velocityB, pCollisionEvent->collisionNormalA, restitutionA,
			inverseMassA, inverseMassB);

		pForceA->SetVelocity(velocityA);
	}

	if (pCollisionEvent->bodyTypeB == eBodyType::DYNAMIC && pCollisionEvent->bodyTypeA == eBodyType::STATIC)
	{
		myutils::ResolveVelocity(velocityB, velocityA, pCollisionEvent->collisionNormalB, restitutionB,
			inverseMassB, inverseMassA);

		pForceB->SetVelocity(velocityB);
	}
}

void Physics::NewFrame()
{
	for (sCollisionData* pCollision : this->m_vecCollided)
	{
		delete pCollision;
	}

	this->m_vecCollided.clear();
	this->m_vecCollVisited.clear();
}

void Physics::Update(double deltaTime)
{
	if (!this->m_isRunning)
	{
		return;
	}

	// Change position based on the acceleration and velocity
	for (this->m_pSceneView->First("force"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
	{
		EntityID entityID = this->m_pSceneView->CurrentKey();
		ForceComponent* pForce = this->m_pSceneView->CurrentValue<ForceComponent>();
		TransformComponent* pTransform = this->m_pSceneView->GetComponent<TransformComponent>(entityID, "transform");

		this->m_ApplyForce(pForce, pTransform, deltaTime);
	}

	// Check if new position is intersecting with other entity
	for (this->m_pSceneView->First("collision"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
	{
		EntityID entityID = this->m_pSceneView->CurrentKey();
		CollisionComponent* pCollision = this->m_pSceneView->CurrentValue<CollisionComponent>();
		TransformComponent* pTransform = this->m_pSceneView->GetComponent<TransformComponent>(entityID, "transform");

		this->m_CheckCollisions(entityID, pCollision, pTransform);
	}

	// Apply respective response for each collision types
	for (sCollisionData* pCollision : this->m_vecCollided)
	{
		TransformComponent* pTransformA = this->m_pSceneView->GetComponent<TransformComponent>(pCollision->entityA, "transform");
		TransformComponent* pTransformB = this->m_pSceneView->GetComponent<TransformComponent>(pCollision->entityB, "transform");
		ForceComponent* pForceA = this->m_pSceneView->GetComponent<ForceComponent>(pCollision->entityA, "force");

		// Static bodies won`t have force
		ForceComponent* pForceB = nullptr;
		if (pCollision->bodyTypeB != eBodyType::STATIC)
		{
			pForceB = this->m_pSceneView->GetComponent<ForceComponent>(pCollision->entityB, "force");
		}


		this->m_ResolveCollision(pCollision, pTransformA, pTransformB, pForceA, pForceB);
	}

	// Trigger collision event for objects that collided
	this->m_pCollisionEvent->TriggerCollisions(this->m_vecCollided);
}

bool Physics::IsRunning()
{
	return this->m_isRunning;
}

void Physics::SetRunning(bool isRunning)
{
	this->m_isRunning = isRunning;
}

bool Physics::AABBAABB2D_Test(sAABB2D* aabb2dA, glm::mat4 matTransfA,
								sAABB2D* aabb2dB, glm::mat4 matTransfB,
								glm::vec3& contactPointA, glm::vec3& contactPointB,
								glm::vec3& collisionNormalA, glm::vec3& collisionNormalB)
{
	// Transform A in world space
	glm::vec4 AminWorld = (matTransfA * glm::vec4(aabb2dA->minXY, 1.0f, 1.0f));
	glm::vec4 AmaxWorld = (matTransfA * glm::vec4(aabb2dA->maxXY, 1.0f, 1.0f));

	// Transform B in world space
	glm::vec4 BminWorld = (matTransfB * glm::vec4(aabb2dB->minXY, 1.0f, 1.0f));
	glm::vec4 BmaxWorld = (matTransfB * glm::vec4(aabb2dB->maxXY, 1.0f, 1.0f));

	// Check if objects collide
	if (AmaxWorld[0] < BminWorld[0])
	{
		return false;
	}

	if (AminWorld[0] > BmaxWorld[0])
	{
		return false;
	}

	if (AmaxWorld[1] < BminWorld[1])
	{
		return false;
	}

	if (AminWorld[1] > BmaxWorld[1])
	{
		return false;
	}

	// Determine the collision side by calculating the minimum overlap in each direction
	float xOverlap = glm::min(AmaxWorld[0], BmaxWorld[0]) - glm::max(AminWorld[0], BminWorld[0]);
	float yOverlap = glm::min(AmaxWorld[1], BmaxWorld[1]) - glm::max(AminWorld[1], BminWorld[1]);

	// Determine the normal direction based on the collision side
	if (xOverlap < yOverlap) {
		// Horizontal collision
		if (AmaxWorld[0] < BmaxWorld[0]) {
			collisionNormalA = glm::vec3(-1.0f, 0.0f, 0.0f);
			collisionNormalB = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else {
			collisionNormalA = glm::vec3(1.0f, 0.0f, 0.0f);
			collisionNormalB = glm::vec3(-1.0f, 0.0f, 0.0f);
		}
	}
	else {
		// Vertical collision
		if (AmaxWorld[1] < BmaxWorld[1]) {
			collisionNormalA = glm::vec3(0.0f, -1.0f, 0.0f);
			collisionNormalB = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else {
			collisionNormalA = glm::vec3(0.0f, 1.0f, 0.0f);
			collisionNormalB = glm::vec3(0.0f, -1.0f, 0.0f);
		}
	}

	// Calculate the contact points
	contactPointA = glm::vec3(AminWorld[0], AminWorld[1], 0.0f);
	contactPointB = glm::vec3(BminWorld[0], BminWorld[1], 0.0f);

	return true; // Collision detected
}