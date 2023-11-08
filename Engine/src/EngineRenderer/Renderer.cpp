#include "EngineRenderer/Renderer.h"
#include "common/utils.h"
#include "components/Animation.h"
#include "components/Model.h"
#include "components/Transform.h"

Renderer::Renderer()
{
	this->m_isRunning = false;
	this->m_isInitialized = false;
	
	this->m_currShaderID = -1;
	this->m_pShaderManager = nullptr;

	this->m_pCameraSystem = nullptr;
	this->m_pModelSystem = nullptr;

	this->m_pSceneView = nullptr;
}

Renderer::~Renderer()
{
	delete this->m_pCameraSystem;
	delete this->m_pModelSystem;
}

bool Renderer::Initialize(std::string baseModelsPath,
							ShaderManager* pShaderManager,
							uint currShaderID,
							SceneView* pSceneView)
{
	if (this->m_isInitialized)
	{
		return true;
	}

	this->m_pSceneView = pSceneView;
	this->m_pShaderManager = pShaderManager;
	this->m_currShaderID = currShaderID;

	printf("Initializing engine renderer...\n");
	// All systems that update info in shaders must have the shader info
	// To set their respectives uniforms and attributes
	this->m_pCameraSystem = new CameraSystem(this->m_pShaderManager, pSceneView);
	this->m_pModelSystem = new ModelSystem(this->m_pShaderManager, pSceneView);

	this->m_pCameraSystem->Initialize();

	// Loading models into VAO and getting IDs
	this->LoadScene(baseModelsPath);

	this->m_isInitialized = true;
	this->m_isRunning = true;

    return true;
}

void Renderer::Destroy()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_isInitialized = false;

	return;
}

void Renderer::LoadScene(std::string baseModelsPath)
{
	this->m_pCameraSystem->Initialize();
	this->m_pModelSystem->LoadModels(baseModelsPath, this->m_currShaderID);
}

void Renderer::NewFrame()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_pShaderManager->UseShaderProgram(this->m_currShaderID);

	return;
}

void Renderer::DrawModel(double deltaTime)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_pCameraSystem->UpdateUL(this->m_currShaderID);

	// TODO: Remove code duplication
	for (this->m_pSceneView->First("model"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
	{
		EntityID entityID = this->m_pSceneView->CurrentKey();
		TransformComponent* pTransform = this->m_pSceneView->GetComponent<TransformComponent>(entityID, "transform");
		ModelComponent* pModel = this->m_pSceneView->GetComponent<ModelComponent>(entityID, "model");

		pTransform->Update(this->m_currShaderID, this->m_pShaderManager);
		pModel->Update(deltaTime, this->m_currShaderID, this->m_pShaderManager);
		pModel->Render();
	}

	for (this->m_pSceneView->First("animation"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
	{
		EntityID entityID = this->m_pSceneView->CurrentKey();
		TransformComponent* pTransform = this->m_pSceneView->GetComponent<TransformComponent>(entityID, "transform");
		AnimationComponent* pAnimation = this->m_pSceneView->GetComponent<AnimationComponent>(entityID, "animation");

		pTransform->Update(this->m_currShaderID, this->m_pShaderManager);
		pAnimation->Update(deltaTime, this->m_currShaderID, this->m_pShaderManager);
		pAnimation->Render();
	}

	return;
}

bool Renderer::IsRunning()
{
	if (this->m_isRunning)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CameraSystem* Renderer::GetCamera()
{
	return this->m_pCameraSystem;
}
