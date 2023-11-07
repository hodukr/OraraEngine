#pragma once
#include <list>
#include "component.h"
class GameObject
{
protected:
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	std::list<Component*> m_Components;

public:
	void SetDestroy() { m_Destroy = true; }

	void SetPosition(D3DXVECTOR3 pos){m_Position = pos;}
	D3DXVECTOR3 GetPosition(){ return m_Position; }

	void SetScale(D3DXVECTOR3 sca) { m_Scale = sca; }
	D3DXVECTOR3 GetScale() { return m_Scale; }



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

	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}



	virtual void Init()
	{
	
	}
	virtual void Uninit()
	{
		for (Component* component : m_Components)
		{
			component->Uninit();
		}
	}
	virtual void Update()
	{
		for (Component* component : m_Components)
		{
			component->Update();
		}
	}
	virtual void Draw()
	{
		for (Component* component : m_Components)
		{
			component->Draw();
		}
	}

	template<typename T>
	T* AddComponent(std::string name = "none")
	{
		Component* component = new T();
		component->SetName(name);
		m_Components.push_back(component);
		component->Init();

		return (T*)component;
	}

	template<typename T>
	T* GetComponent(std::string name = "none")
	{
		for (Component* component : m_Components)
		{
			if (typeid(*component) == typeid(T))//Œ¨‚ð’²‚×‚é(RTTI“®“IŒ^î•ñ)
			{
				if(name == component->GetName())
				return (T*)component;
			}
		}
		return nullptr;
	}
};