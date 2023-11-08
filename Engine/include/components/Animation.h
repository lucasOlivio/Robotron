#pragma once

#include "Component.h"
#include "common/drawInfo.h"
#include "EngineRenderer/iShaderInfo.h"
#include <vector>

class AnimationComponent : public Component
{
private:
	bool m_isActive;
	int m_currFrame;
	double m_elapsedTime;

	std::vector <sMesh*> m_pMeshes;

	void m_UpdateFrames(double deltaTime);
	void m_UpdateUniforms(uint shaderID, iShaderInfo* pShaderInfo);

public:
	AnimationComponent();
	virtual ~AnimationComponent();

	std::vector<std::string> models;
	std::string friendlyName;
	bool isWireframe;
	bool doNotLight;

	void SetMesh(uint index, sMesh* pMesh);
	std::vector <sMesh*> GetMeshes();

	void AddMesh(sMesh* pMesh);
	sMesh* GetMesh(uint index);

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);

	void Update(double deltaTime, uint shaderID, iShaderInfo* pShaderInfo);

	void SetActive(bool isActive);

	void Render();
};