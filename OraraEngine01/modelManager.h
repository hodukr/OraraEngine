#pragma once
#include "model.h"

class ModelManager
{
private:
	static std::unordered_map<int, Model> m_Models;
	static int m_Index;
public:
	static int Load(const char* name);
	static Model* GetModel(int index);
	static void Uninit();
};