#pragma once
#include <unordered_map>
#include "material.h"
#include "model.h"
#include "singleton.h"

class ModelManager
{
private:
    Singleton(ModelManager);
	std::unordered_map<int, Model> m_Models;
	int m_Index;
public:
	int Load(char* name);
	Model* GetModel(int index);
	void Uninit();
};
