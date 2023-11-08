#include "EngineRenderer/CameraSystem.h"
#include "common/opengl.h"
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

CameraSystem::CameraSystem(iShaderInfo* pShaderInfo, SceneView* pSceneView)
{
    this->m_pShaderInfo = pShaderInfo;
    this->m_pSceneView = pSceneView;
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Initialize()
{
    this->m_pCamera = this->m_pSceneView->GetComponent<CameraComponent>(0, "camera");
}

glm::mat4 CameraSystem::GetViewMat()
{
    return glm::lookAt(this->m_pCamera->cameraEye,
                        this->m_pCamera->cameraTarget,
                        this->m_pCamera->upVector);
}

void CameraSystem::UpdateUL(int shaderID)
{
    // Update view matrix in shader

    glm::mat4 matView = this->GetViewMat();
    int matViewUL = this->m_pShaderInfo->GetUL(shaderID, "matView");
    glUniformMatrix4fv(matViewUL, 1, GL_FALSE, glm::value_ptr(matView));
}
