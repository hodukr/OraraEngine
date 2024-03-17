﻿#pragma once
#include "main.h"
#include "guiManager.h"
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
			GameObject* camera = AddGameObject(0);
			camera->SetName("MainCamera");
			camera->AddComponent<Camera>();
			GameObject* sky = AddGameObject(1);
			Mesh* mesh = sky->AddComponent<Mesh>();
			sky->SetName("Sky");
			mesh->SetModel("sky.obj");
			sky->m_Transform->SetScale(80.0f,80.0f,80.0f);
			std::unique_ptr<Material> material = std::make_unique<Material>("unlitTexture");
			material->Init();
			sky->SetMaterial(std::move(material));
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

	void EditorUpdate()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto& gameObject : m_GameObject[i])
			{
				gameObject->EditorUpdate();
			}
		}
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

	void Destroy()
	{
		for (int i = 0; i < 3; i++)
		{
			m_GameObject[i].remove_if([](const std::unique_ptr<GameObject>& object) {return object->Destroy(); });//ラムダ式
		}
	}


	GameObject* AddGameObject(int Layer)
	{
		return SetGameObject(std::make_unique<GameObject>());
	}

	GameObject* SetGameObject(std::unique_ptr<GameObject>  gameObject)
	{
		gameObject->Init();
		gameObject->SetName(gameObject->GetName());
		int drawLayer = 1;
		if (gameObject->GetDrawLayer() > 0)drawLayer = gameObject->GetDrawLayer();
		m_GameObject[drawLayer].push_back(std::move(gameObject));

		return m_GameObject[drawLayer].back().get();
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

	GameObject* GetGameObjectToTag(const char* name)
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto& object : m_GameObject[i])
			{

				if (object.get()->GetTag() == name)//型を調べる(RTTI動的型情報)
				{
					return object.get();
				}
			}
		}

		return nullptr;
	}

	std::vector<GameObject* >GetGameObjectsToTag(const char* name)
	{
		std::vector<GameObject*> objects; //STLの配列
		for (int i = 0; i < 3; i++)
		{
			for (auto& object : m_GameObject[i])
			{
				if (object.get()->GetTag() == name)//型を調べる(RTTI動的型情報)
				{
					objects.push_back(object.get());
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


	void MoveLayer(std::list<std::unique_ptr<GameObject>>& listA, std::list<std::unique_ptr<GameObject>>& listB, GameObject* movedate)
	{
		// listAからlistBへデータNと同じ要素を移動
		auto iter = listA.begin();
		while (iter != listA.end()) {
			if ((*iter)->GetName() == movedate->GetName()) {
				listB.splice(listB.end(), listA, iter++);
			}
			else {
				++iter;
			}
		}
	}
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
