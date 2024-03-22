#pragma once
#include "guiManager.h"
#include "gameObject.h"
#include <typeinfo>
#include "textureManager.h"
#include "modelManager.h"
#include <algorithm>

namespace fs = filesystem;
class Scene
{
protected:
    string m_Name;
	list<unique_ptr<GameObject>> m_GameObject[3];//レイヤー有のSTLのリスト構造
	int m_FileVersion = NOWVERSION;
public:
    Scene(string name = "NewScene")
	{
		SetName(name);
	}
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
			unique_ptr<Material> material = make_unique<Material>("unlitTexture");
			material->Init();
			sky->SetMaterial(move(material));
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

	virtual void EditorDraw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto& gameObject : m_GameObject[i])
			{
				gameObject->EditorDraw(); //ポリモーフィズム
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
			m_GameObject[i].remove_if([](const unique_ptr<GameObject>& object) {return object->Destroy(); });//ラムダ式
		}
	}


	GameObject* AddGameObject(int Layer)
	{
		return SetGameObject(make_unique<GameObject>());
	}

	GameObject* SetGameObject(unique_ptr<GameObject>  gameObject)
	{
		gameObject->Init();
		gameObject->SetName(gameObject->GetName());
		int drawLayer = 1;
		if (gameObject->GetDrawLayer() > 0)drawLayer = gameObject->GetDrawLayer();
		m_GameObject[drawLayer].push_back(move(gameObject));

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

	vector<GameObject* >GetGameObjectsToTag(const char* name)
	{
		vector<GameObject*> objects; //STLの配列
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


    list<unique_ptr<GameObject>>* GetList()
    {
        return m_GameObject;
    }

    void SetName(string name) {
		string comparisonName = name;
		int index = 0;
		string folderPath = "asset\\scene";
		bool roop = true;
		while (roop)
		{
			roop = false;
			// 指定されたフォルダ内のファイルをイテレート 
			for (const auto& entry : fs::directory_iterator(folderPath)) {
				// ファイル名をベクターに追加 
				if (entry.path().filename().string() == comparisonName + ".json")//フォルダ内にNewScene.jsonがあれば読み込む
				{
					++index;
					comparisonName = name + "(" + to_string(index) + ")";
					roop = true;
				}
			}
		}
        fs::path oldFilePath = "asset/scene/" + m_Name + "json";

        // 新しいファイル名
        string newFileName = comparisonName + ".json";

        // 新しいファイルのパスを構築
        fs::path newFilePath = oldFilePath.parent_path() / newFileName;

        // ファイル名を変更
        try {
            fs::rename(oldFilePath, newFilePath);
            cout << "File renamed successfully." << endl;
        }
        catch (const filesystem::filesystem_error& e) {
            cerr << "Error renaming file: " << e.what() << endl;
        }
        m_Name = comparisonName;
    }
    string GetName() { return m_Name; }


	void MoveLayer(list<unique_ptr<GameObject>>& listA, list<unique_ptr<GameObject>>& listB, GameObject* movedate)
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
