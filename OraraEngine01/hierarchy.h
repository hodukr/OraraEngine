#pragma once
#include <list>

class Hierarchy
{
private:
	class Scene* m_Scene;
	std::list<class GameObject*> m_GameObjects;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetScene(Scene* scene) { m_Scene = scene; }
};
