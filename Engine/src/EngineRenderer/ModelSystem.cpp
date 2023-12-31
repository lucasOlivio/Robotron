#include "EngineRenderer/ModelSystem.h"
#include "components/iComponent.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "components/Animation.h"
#include "common/opengl.h"
#include "common/utilsMat.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

ModelSystem::ModelSystem(iShaderInfo* pShaderInfo, SceneView* pSceneView)
{
    this->m_pShaderInfo = pShaderInfo;
    this->m_pSceneView = pSceneView;
    this->m_pVAOManager = new VAOManager(this->m_pShaderInfo);
}

ModelSystem::~ModelSystem()
{
    delete this->m_pVAOManager;
}

bool ModelSystem::LoadModels(int shaderID)
{
    for (this->m_pSceneView->First("model"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
    {
        ModelComponent* pModel = this->m_pSceneView->CurrentValue<ModelComponent>();
        // For now only debug objects will need to be dynamic
        pModel->pMeshInfo = this->m_pVAOManager->LoadModelIntoVAO(pModel->name, shaderID, false);
        if (!pModel->pMeshInfo)
        {
            return false;
        }

        if (pModel->collisionName != "")
        {
            pModel->pCollisionMesh = this->m_pVAOManager->LoadModelIntoVAO(pModel->collisionName, shaderID, false);
            if (!pModel->pCollisionMesh)
            {
                return false;
            }
        }
    }


    for (this->m_pSceneView->First("animation"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
    {
        AnimationComponent* pAnimation = this->m_pSceneView->CurrentValue<AnimationComponent>();
        // For now only debug objects will need to be dynamic

        for (int i = 0; i < pAnimation->models.size(); i++)
        {
            sMesh* pMesh = this->m_pVAOManager->LoadModelIntoVAO(pAnimation->models[i], shaderID, false);
            if (!pMesh)
            {
                return false;
            }
            pAnimation->SetMesh(i, pMesh);
        }
    }


    return true;
}

bool ModelSystem::LoadModels(std::string basePath, int shaderID)
{
    this->m_pVAOManager->SetBasePath(basePath);

    this->LoadModels(shaderID);

    return true;
}

void ModelSystem::Destroy()
{
    for (this->m_pSceneView->First("model"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
    {
        ModelComponent* pModel = this->m_pSceneView->CurrentValue<ModelComponent>();
        this->m_pVAOManager->DestroyVBO(pModel->name);
    }
}
