#pragma once
#include <list>
#include "imGuiWindow.h"
#include "singleton.h"
class Hierarchy :public GuiWindowBase
{
private:
	class Scene* m_Scene;
    bool m_OpenTree;
	std::list<class GameObject*> m_GameObjects;
    GameObject* m_SelectGameObject = nullptr;
    GameObject* m_ConfigGameObject = nullptr;

public:
	void Init();
	void Uninit();
	void Update();
	void SetWindowConfig();
	void Draw();

	void SetScene(Scene* scene) { m_Scene = scene; }
	void SetSelectGameObject(GameObject* objct) { m_SelectGameObject = objct; }
	GameObject* GetSelectGameObject() { return m_SelectGameObject; }
};
