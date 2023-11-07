#pragma once
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
	std::list<GameObject*> m_GameObject[3];//���C���[�L��STL�̃��X�g�\��

public:
	virtual void Init(){}

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])//�͈�for���[�v
			{
				gameObject->Uninit();
				delete gameObject;
			}

			m_GameObject[i].clear();
		}
		TextuereManager::Uninit();
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}

			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });//�����_��
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
				gameObject->Draw(); //�|�����[�t�B�Y��
			}
		}
	}

	//�^�̕������֐��������
	//���܂�g��Ȃ���������
	template<typename T>//�e���v���[�g�֐�
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template<typename T>//�e���v���[�g�֐�
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				//�������������̂ł��܂�g��Ȃ��ق�������
				if (typeid(*object) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
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
		std::vector<T*> objects; //STL�̔z��
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};