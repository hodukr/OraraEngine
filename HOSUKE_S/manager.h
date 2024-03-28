#pragma once
//循環参照になるので前方宣言
//class Scene;
//前方宣言はポインタで使う時のみ
//継承には使用不可

#include "scene.h"

class Manager
{
private:
	//Manegerはインスタンスしないので静的に作る
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static Scene* m_LoadScene;
	static SceneState m_SceneState;
	static SceneState m_NextSceneState;
	static GameState m_GameState;
	static GameState m_NextGameState;
	static class CollisionManager* m_CollisionManager;
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static void MTInit();

	static Scene* GetScene(void) { return m_Scene; }

	template<typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}

	static void SetLoadScene(string scene);
	static void SetLoaded(Scene* scene);
	static void SetScene(string scene);

	static void SetNextSceneState(SceneState state) {m_NextSceneState = state;}
	static SceneState GetSceneState() { return m_SceneState; }

	static void SetNextGameState(GameState state) { m_NextGameState = state; }
	static GameState GetGameState() { return m_GameState; }

	static CollisionManager* GetCollisionManager() { return m_CollisionManager; }
	static GameObject* CreatePrefab(std::string name, Vector3 position);
};
