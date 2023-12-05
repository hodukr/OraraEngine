#pragma once
#include <list>
#include "singleton.h"
class Hierarchy
{
private:
    Singleton(Hierarchy);
	class Scene* m_Scene;
    bool m_OpenTree;
	std::list<class GameObject*> m_GameObjects;
    GameObject* m_SelectGameObject = nullptr;
    GameObject* m_ConfigGameObject = nullptr;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetScene(Scene* scene) { m_Scene = scene; }
};
