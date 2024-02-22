#include "main.h"
#include "renderer.h"
#include "modelManager.h"

std::unordered_map<int, Model> ModelManager::m_Models;
int ModelManager::m_Index = 0;

int ModelManager::Load(const char* name)
{
	for (int i = 0; i < m_Index; i++)
	{
		if (m_Models[i].GetFile() == name)
		{
			return i;
		}
	}
	m_Models[m_Index].Load(name);
	
    return m_Index++;
}

Model* ModelManager::GetModel(int index)
{
	return &m_Models[index];
}

void ModelManager::Uninit()
{
	for (int i = 0; i < m_Index; i++)
	{
		m_Models[i].Unload();
	}
	m_Models.clear();
}
