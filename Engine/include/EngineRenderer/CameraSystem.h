#pragma once

#include "scene/SceneView.h"
#include "iShaderInfo.h"
#include "components/Camera.h"
#include <glm/mat4x4.hpp>

class CameraSystem
{
private:
	iShaderInfo* m_pShaderInfo;
	SceneView* m_pSceneView;

	CameraComponent* m_pCamera;
public:
	// ctors & dtors
	CameraSystem(iShaderInfo* pShaderInfo, SceneView* pSceneView);
	~CameraSystem();

	void Initialize();

	glm::mat4 GetViewMat();

	// Called each frame
	void UpdateUL(int shaderID);
};