#include "scene/SceneView.h"

SceneView::SceneView()
{
	this->m_map.clear();
	this->m_currIterator = this->m_map.begin();
}

SceneView::~SceneView()
{
}

void SceneView::First(std::string componentName)
{
	bool hasComponent = this->GetMapComponents(componentName, this->m_map);
	if (hasComponent)
	{
		this->m_currIterator = this->m_map.begin();
	}
	else
	{
		this->m_currIterator = this->m_map.end();
	}
}

void SceneView::Next()
{
	this->m_currIterator++;
}

void SceneView::End()
{
	this->m_currIterator = this->m_map.end();
}

bool SceneView::IsDone()
{
	if (this->m_currIterator == this->m_map.end())
	{
		return true;
	}
	return false;
}

EntityID SceneView::CurrentKey()
{
	return this->m_currIterator->first;
}

iComponent* SceneView::GetComponent(EntityID entityID, std::string componentName)
{
	auto entity = this->m_map.find(entityID);
	if (entity == this->m_map.end())
	{
		return nullptr;
	}

	return entity->second;
}
