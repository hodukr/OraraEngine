#pragma once
#include "main.h"
#include "guiManager.h"
#include "gameObject.h"
#include <list>
#include <typeinfo>
#include <vector>
#include "camera.h"
#include "textureManager.h"
#include <algorithm>
#include <cereal/types/list.hpp>

class Scene
{
protected:
    //GameObject* m_GameObject[4]{};
    std::string m_Name;
	std::list<std::unique_ptr<GameObject>> m_GameObject[3];//レイヤー有のSTLのリスト構造

public:
    Scene(std::string name = "NewScene"):m_Name(name){}
	virtual void Init()
    {
        for (int i = 0; i < 3; i++)
        {
            for (auto& gameobject : m_GameObject[i])
            {
                gameobject.get()->Init();
            }
        }
    }

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto& gameObject : m_GameObject[i])//範囲forループ
			{
				gameObject->Uninit();
			}

			m_GameObject[i].clear();
		}
		TextureManager::Uninit();
        GuiManager::Instance().Uninit();

	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto& gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}

		}
        for (int i = 0; i < 3; i++)
        {
            for (auto& gameObject : m_GameObject[i])
            {

            }
			 m_GameObject[i].remove_if([](const std::unique_ptr<GameObject>& object) {return object->Destroy(); });//ラムダ式
        }

	}

	virtual void Draw()
	{
		//Camera* camera = GetGameObject<Camera>();
		for (int i = 0; i < 3; i++)
		{
			
			
			for (auto& gameObject : m_GameObject[i])
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
		std::unique_ptr<GameObject> gameObject = std::make_unique<T>();
		gameObject->Init();
        //std::string name;
        //name = "GameObject";
        //gameObject->SetName(name);
        m_GameObject[Layer].push_back(std::move(gameObject));

		return dynamic_cast<T*>(m_GameObject[Layer].back().get());
	}

	template<typename T>//テンプレート関数
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto& object : m_GameObject[i])
			{
				//メモリをくうのであまり使わないほうがいい
				if (typeid(*object) == typeid(T))//型を調べる(RTTI動的型情報)
				{
					return dynamic_cast<T*>(object.get());
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
			for (auto& object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))//型を調べる(RTTI動的型情報)
				{
					objects.push_back(dynamic_cast<T*>(object.get()));
				}
			}
		}
		return objects;
	}

    std::list<std::unique_ptr<GameObject>>* GetList()
    {
        return m_GameObject;
    }

    void SetName(std::string name) { m_Name = name; }
    std::string GetName() { return m_Name; }

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_GameObject[0]), CEREAL_NVP(m_GameObject[1]), CEREAL_NVP(m_GameObject[2]));
    }
};
