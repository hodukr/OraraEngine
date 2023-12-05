#pragma once

#include <list>
#include "component.h"


class GameObject
{
protected:
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 m_OldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	float       m_radius{};

	std::list<Component*> m_Component;

public:
	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetPosition(D3DXVECTOR3 position) { m_Position = position;}
	void SetRotation(D3DXVECTOR3 rotation) { m_Rotation = rotation;}
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }
    void SetOldPosition(D3DXVECTOR3 position) { m_OldPosition = position; }
	D3DXVECTOR3 GetPosition(void) { return m_Position; }
	D3DXVECTOR3 GetOldPosition(void) { return m_OldPosition; }
	D3DXVECTOR3 GetRotation(void) { return m_Rotation; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }


	D3DXVECTOR3 GetRight(void) //�E�����x�N�g���擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}
	D3DXVECTOR3 GetUp(void) //������x�N�g���擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._21;
		forward.y = rot._22;
		forward.z = rot._23;

		return forward;
	}
	D3DXVECTOR3 GetForward(void) //�O�����x�N�g���擾
	{ 
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward; 
	}
	
	virtual void Init() {}; //���z�֐�
	//virtual void Init() = 0; �������z�֐��@��΂ɂ����Ă΂Ȃ��Ⴂ���Ȃ��Ȃ�
	virtual void Uninit() 
	{
		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
	    }

		m_Component.clear();
	}; 

 	virtual void Update() 
	{
		for (Component* component : m_Component)
		{
			component->Update();
		}
	};

	virtual void Draw() 
	{
		for (Component* component : m_Component)
		{
			component->Draw();
		}
	};

	template<typename T>//�e���v���[�g�֐�
	T* AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init();

		//�X�^�e�B�b�N�L���X�g��int�Ȃǂł�
		//             �g���邪�_�C�i�~�b�N�L���X�g���댯
		//return static_cast<T*>(component);
		//�_�C�i�~�b�N�L���X�g�͎Q�Ƃ܂��̓|�C���^�̂�
		return dynamic_cast<T*>(component);
	}
}; 
