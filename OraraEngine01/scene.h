#pragma once
#include "main.h"
#include "gameObject.h"
#include <list>
#include <typeinfo>
#include <vector>

#include "camera.h"
#include "textureManager.h"
#include <algorithm>

class Scene
{
protected:
    //GameObject* m_GameObject[4]{};
	std::list<GameObject*> m_GameObject[3];//レイヤー有のSTLのリスト構造

public:
	virtual void Init(){}

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])//範囲forループ
			{
				gameObject->Uninit();
				delete gameObject;
			}

			m_GameObject[i].clear();
		}
		TextureManager::Uninit();
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}

			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });//ラムダ式
		}
	}

	virtual void Draw()
	{
		//Camera* camera = GetGameObject<Camera>();
		for (int i = 0; i < 3; i++)
		{
			
			
			for (GameObject* gameObject : m_GameObject[i])
			{
				//sort(m_GameObject->begin(), m_GameObject->end(), [camera,gameObject](float maxLen) {
				//	D3DXVECTOR3 vec = gameObject->GetPosition() - camera->GetPosition();
				//	float len = D3DXVec3Length(&vec);
				//	len > maxLen;
				//	//max
				//	});
				gameObject->Draw(); //ポリモーフィズム
			}
		}
	}

	//型の分だけ関数が作られる
	//あまり使わない方がいい
	template<typename T>//テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template<typename T>//テンプレート関数
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				//メモリをくうのであまり使わないほうがいい
				if (typeid(*object) == typeid(T))//型を調べる(RTTI動的型情報)
				{
					return(T*)object;
				}
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects; //STLの配列
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))//型を調べる(RTTI動的型情報)
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};
