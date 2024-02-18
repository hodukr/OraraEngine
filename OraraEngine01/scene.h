#pragma once
#include "main.h"
#include "guiManager.h"
#include "gameManager.h"
#include "gameObject.h"
#include <list>
#include <typeinfo>
#include <vector>
#include "textureManager.h"
#include "modelManager.h"
#include <algorithm>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <filesystem>

namespace fs = std::filesystem;
class Scene
{
protected:
    std::string m_Name;
	std::list<std::unique_ptr<GameObject>> m_GameObject[3];//レイヤー有のSTLのリスト構造
	int m_FileVersion = NOWVERSION;
public:
    Scene(std::string name = "NewScene"):m_Name(name){}
	virtual void Init()
    {
		bool newflg = true;
        for (int i = 0; i < 3; i++)
        {
            for (auto& gameobject : m_GameObject[i])
            {
                gameobject.get()->Init();
				newflg = false;
            }
        }
		if (newflg) 
		{
			GameObject* obj = AddGameObject(1);
			//obj->SetName("MainCamera");
			//obj->AddComponent<Camera>();
			Mesh* mesh = obj->AddComponent<Mesh>();
			obj->SetName("Sky");
			mesh->SetModel("sky.obj");
			obj->m_Transform->SetScale(80.0f,80.0f,80.0f);
			std::unique_ptr<Material> material = std::make_unique<Material>("unlitTexture");
			material->Init();
			obj->SetMaterial(std::move(material));
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
		ModelManager::Uninit();
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
		for (int i = 0; i < 3; i++)
		{
			for (auto& gameObject : m_GameObject[i])
			{
				gameObject->Draw(); //ポリモーフィズム
			}
		}
	}

	GameObject* AddGameObject(int Layer)
	{
		std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
		gameObject->Init();
       
        //std::string name;
        //name = "GameObject";
        //gameObject->SetName(name);
        m_GameObject[Layer].push_back(std::move(gameObject));

		return m_GameObject[Layer].back().get();
	}

	GameObject* GetGameObject(const char* name)
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto& object : m_GameObject[i])
			{

				if (object.get()->GetName() == name)//型を調べる(RTTI動的型情報)
				{
					return object.get();
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

    void SetName(std::string name) {
        fs::path oldFilePath = "asset/scene/" + m_Name + "json";

        // 新しいファイル名
        std::string newFileName = name + ".json";

        // 新しいファイルのパスを構築
        fs::path newFilePath = oldFilePath.parent_path() / newFileName;

        // ファイル名を変更
        try {
            fs::rename(oldFilePath, newFilePath);
            std::cout << "File renamed successfully." << std::endl;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error renaming file: " << e.what() << std::endl;
        }
        m_Name = name;
    }
    std::string GetName() { return m_Name; }
    template<class Archive>
    void serialize(Archive& archive)
    {
		archive(CEREAL_NVP(m_FileVersion),
			CEREAL_NVP(m_GameObject[0]),
			CEREAL_NVP(m_GameObject[1]),
			CEREAL_NVP(m_GameObject[2]), 
			CEREAL_NVP(m_Name));

		for (int i = 0; i < 3; i++)
		{
			for (auto& gameobj : m_GameObject[i])
			{
				gameobj.get()->SetVersion(m_FileVersion);
			}
		}
    }
};
