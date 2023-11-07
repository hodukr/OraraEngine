#pragma once
#include "scene.h"

class Manager
{
private:
	static Scene* m_NowScene;
	static Scene* m_NextScene;
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_NowScene; }
	template<typename T>
	static void SetScene()
	{
		m_NextScene = new T();


	}

	static void ChangeScene()
	{
		if (m_NextScene != m_NowScene)
		{
			if (m_NowScene)
			{
				m_NowScene->Uninit();
				delete m_NowScene;
			}
			m_NowScene = m_NextScene;
			m_NowScene->Init();
		}
	}

};