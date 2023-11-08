#pragma once

#include "ShaderManager.h"
#include "WindowSystem.h"
#include "CameraSystem.h"
#include "ModelSystem.h"
#include "scene/SceneView.h"

class Renderer
{
private:
	bool m_isRunning;
	bool m_isInitialized;

	ShaderManager* m_pShaderManager;
	uint m_currShaderID;

	CameraSystem* m_pCameraSystem;
	ModelSystem* m_pModelSystem;

	SceneView* m_pSceneView;

public:
	// ctors & dtors
	Renderer();
	~Renderer();

	// TODO: The scene should be changed so that the systems can get only the
	// componen they need, to avoid the whole thing been passed everywhere 

	// Initializes all the systems, if anything go wrong we should stop here
	bool Initialize(std::string baseModelsPath,
					ShaderManager* pShaderManager,
					uint currShaderID,
					SceneView* pSceneView);
	void Destroy();

	// Go through the scene loading each model component to the VAO
	void LoadScene(std::string baseModelsPath);

	// Clear frame, set shader and configure glfw for a new frame
	void NewFrame();
	// Update respectives UL and render model
	void DrawModel(double deltaTime);

	bool IsRunning();

	CameraSystem* GetCamera();
};