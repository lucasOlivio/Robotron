#include "components/Animation.h"
#include "common/utilsMat.h"
#include "common/opengl.h"

const int FRAME_RATE = 15;  // Frames per second
const double FRAME_DURATION = 1.0 / FRAME_RATE;

AnimationComponent::AnimationComponent()
{
    this->m_currFrame = 0;
    this->m_elapsedTime = 0;
    this->m_pMeshes.clear();
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::m_UpdateFrames(double deltaTime)
{
    this->m_elapsedTime += deltaTime;

    if (this->m_elapsedTime < FRAME_DURATION)
    {
        return;
    }

    // Update the animation index
    this->m_currFrame++;
    if (this->m_currFrame >= this->m_pMeshes.size()) {
        this->m_currFrame = 0;  // Loop the animation if needed
    }

    // Reset the elapsed time
    this->m_elapsedTime = 0.0;
}

void AnimationComponent::m_UpdateUniforms(uint shaderID, iShaderInfo* pShaderInfo)
{
    if (this->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Set if the mesh should calculate lightning in shader
    GLint doNotLightUL = pShaderInfo->GetUL(shaderID, "doNotLight");
    glUniform1f(doNotLightUL, this->doNotLight);
}

void AnimationComponent::SetMesh(uint index, sMesh* pMesh)
{
    if (index > this->m_pMeshes.size())
    {
        return;
    }

    this->m_pMeshes[index] = pMesh;
}

std::vector<sMesh*> AnimationComponent::GetMeshes()
{
    return this->m_pMeshes;
}

void AnimationComponent::AddMesh(sMesh* pMesh)
{
    this->m_pMeshes.push_back(pMesh);
}

sMesh* AnimationComponent::GetMesh(uint index)
{
    assert(index < this->m_pMeshes.size());
    return this->m_pMeshes[index];
}

void AnimationComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "animation";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("isActive", "bool", this->m_isActive, compInfoOut);
    this->AddCompParInfo("models", "vecStr", this->models, compInfoOut);
    this->AddCompParInfo("friendlyName", "string", this->friendlyName, compInfoOut);
    this->AddCompParInfo("isWireframe", "bool", this->isWireframe, compInfoOut);
    this->AddCompParInfo("doNotLight", "bool", this->doNotLight, compInfoOut);
}

void AnimationComponent::SetParameter(sParameterInfo& parameterIn)
{
    using namespace myutils;

    if (parameterIn.parameterName == "isActive") {
        this->m_isActive = parameterIn.parameterBoolValue;
    }
    else if (parameterIn.parameterName == "models") {
        this->m_currFrame = 0;
        this->models = parameterIn.parameterVecStrValue;
        this->m_pMeshes.resize(this->models.size(), nullptr);
    }
    else if (parameterIn.parameterName == "friendlyName") {
        this->friendlyName = parameterIn.parameterStrValue;
    }
    else if (parameterIn.parameterName == "isWireframe") {
        this->isWireframe = parameterIn.parameterBoolValue;
    }
    else if (parameterIn.parameterName == "doNotLight") {
        this->doNotLight = parameterIn.parameterBoolValue;
    }

    return;
}

void AnimationComponent::Update(double deltaTime, uint shaderID, iShaderInfo* pShaderInfo)
{
    this->m_UpdateUniforms(shaderID, pShaderInfo);

    if (!this->m_isActive)
    {
        return;
    }

    this->m_UpdateFrames(deltaTime);

}

void AnimationComponent::SetActive(bool isActive)
{
    this->m_isActive = isActive;
}

void AnimationComponent::Render()
{
    if (this->m_pMeshes.size() == 0)
    {
        return;
    }
    // Bind to the VAO and call opengl to draw all vertices
    sMesh* pCurrMesh = this->m_pMeshes[this->m_currFrame];
    glBindVertexArray(pCurrMesh->VAO_ID); //  enable VAO (and everything else)
    glDrawElements(GL_TRIANGLES,
        pCurrMesh->numberOfIndices,
        GL_UNSIGNED_INT,
        0);
    glBindVertexArray(0); 			  // disable VAO (and everything else)
}
