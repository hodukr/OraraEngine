#include "main.h"
#include "rendererManager.h"
#include "modelManager.h"


int ModelManager::Load(char* name)
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
