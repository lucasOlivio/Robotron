#pragma once

#include "Component.h"
#include "common/drawInfo.h"
#include "EngineRenderer/iShaderInfo.h"
#include <string>

class ModelComponent : public Component
{
private:
	bool m_isActive;
public:
	ModelComponent();
	virtual ~ModelComponent() {};

	std::string name;
	std::string collisionName;
	std::string friendlyName;
	bool isWireframe;
	bool doNotLight;

	sMesh* pMeshInfo;
	sMesh* pCollisionMesh;

	void SetActive(bool isActive);
	bool IsActive();

	void Update(double deltaTime, uint shaderID, iShaderInfo* pShaderInfo);
	void Render();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};