#pragma once
#include <list>
#include "imGuiWindow.h"
#include "singleton.h"
class Hierarchy :public GuiWindowBase
{
private:
	class Scene* m_Scene = nullptr;
	bool m_OpenTree{};
	list<class GameObject*> m_GameObjects;
    GameObject* m_SelectGameObject = nullptr;
    GameObject* m_ConfigGameObject = nullptr;
	GameObject* m_CopyGameObject = nullptr;

public:
	void Init();
	void Uninit();
	void Update();
	void SetWindowConfig();
	void Draw();

	void SetScene(Scene* scene) { m_Scene = scene; }
	void SetSelectGameObject(GameObject* objct) { m_SelectGameObject = objct; }
	
	GameObject* GetSelectGameObject() { return m_SelectGameObject; }
private:
	void DrawCreateObject();
	void CleatePrefab();

public:
	//template<class Archive>
	//void serialize(Archive& archive)
	//{
	//	archive(CEREAL_NVP(m_OpenTree));
	//}
};
