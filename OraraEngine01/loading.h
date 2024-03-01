#pragma once
#include "scene.h"
#include <mutex>

#define PATTERN_X (1.0f / 3.0f)
#define PATTERN_Y (1.0f / 4.0f)

class Loading :public Scene
{
private:
	ID3D11Buffer* m_VertexBuffer{};

	
	int m_ShaderNum = -1;
	int m_TextureNum = -1;

	bool m_Loaded = false;
	std::mutex m_LoadedMutex{};
	Scene* m_NextScene = nullptr;

	int m_Count = 0;
	void AsyncLoad();

public:

	Loading() {}
	Loading(Scene* next_scene) : m_NextScene(next_scene) {}
	void Init()override;
	void Update()override;
	void Uninit()override;
	void Draw()override;


	void SetLockFlag(bool Load)
	{
		m_Loaded = true;
	}

	bool GetLockFlag()
	{
		return m_Loaded;
	}
};