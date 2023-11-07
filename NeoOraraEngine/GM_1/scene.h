#pragma once
#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>

 
class Scene
{
protected:
	std::list<GameObject*> m_GameObject[3];//���C���[�L��STL

public:
	virtual void Init(){}
	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Uninit();
				delete gameObject;
			}
			m_GameObject[i].clear();//���X�g�\���̏���
		}
		

	}
	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}
			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });

		}
	}
	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Draw();
			}
		}
		
	}


	template<typename T>
	T* AddGameObject(int layer)
	{
		GameObject* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		gameObject->Init();

		return (T*)gameObject;
	}

	template<typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))//���𒲂ׂ�(RTTI���I�^���)
				{
					return (T*)object;
				}
			}
		}
		return nullptr;

	}

	template<typename T>
	std::vector<T*>GetGameObjects()
	{
		std::vector<T*>objects;
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};