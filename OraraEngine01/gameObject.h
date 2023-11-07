#pragma once

#include <list>
#include "component.h"
#include "sprite.h"


class GameObject
{
protected:
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position;}
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation;}
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	D3DXVECTOR3 GetPosition(void) { return m_Position; }
	D3DXVECTOR3 GetRotation(void) { return m_Rotation; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }

	D3DXVECTOR3 GetRight(void) //右方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}
	D3DXVECTOR3 GetUp(void) //上方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._21;
		forward.y = rot._22;
		forward.z = rot._23;

		return forward;
	}
	D3DXVECTOR3 GetForward(void) //前方向ベクトル取得
	{ 
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward; 
	}
	
	virtual void Init() {}; //仮想関数
	//virtual void Init() = 0; 純粋仮想関数　絶対にこれを呼ばなきゃいけなくなる
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

	template<typename T>//テンプレート関数
	T* AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init();

		//スタティックキャストはintなどでも
		//             使えるがダイナミックキャストより危険
		//return static_cast<T*>(component);
		//ダイナミックキャストは参照またはポインタのみ
		return dynamic_cast<T*>(component);
	}
}; 
