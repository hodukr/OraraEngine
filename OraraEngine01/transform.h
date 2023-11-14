#pragma once
#include "component.h"
#include "vector.h"

class Transform :public Component
{
private:
	Vector3 m_Position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_Scale = Vector3(1.0f, 1.0f, 1.0f);
	int a;
public:
	
	void SetWorldPosition(Vector3 pos) { m_Position = pos; }
	Vector3 GetWorldPosition() { return m_Position; }
	void SetWorldRotation(Vector3 rot) { m_Rotation = rot; }
	Vector3 GetWorldScale() { return m_Scale; }
	void SetWorldScale(Vector3 sca) { m_Scale = sca; }
	Vector3 GetWorldRotation() { return m_Rotation; }

	void Translate(Vector3 moveVec) { m_Position += moveVec; }
	void Rotate(Vector3 rotVec) { m_Rotation += rotVec; }
	void Scale(Vector3 scaVec) { m_Scale += scaVec; }
	
	Vector3 GetRight();
	Vector3 GetForward();
	Vector3 GetUp();

	void Revolution(Vector3 target, Vector3 axis, float rot, bool isObjRot = false);

	void Init()override;
	void Uninit()override;
	void Update()override;


#ifdef _DEBUG
	Vector3* GetPositionPointer() { return &m_Position; }
	Vector3* GetRotationPointer() { return &m_Rotation; }
	Vector3* GetScalePointer() { return &m_Scale; }
#endif // _DEBUG

//オブジェクトの親子関係できたら使う鴨
//public:
//	Vector3 m_LocalPosition = Vector3(0.0f, 0.0f, 0.0f);
//	Vector3 m_LocalRotation = Vector3(0.0f, 0.0f, 0.0f);
//	Vector3 m_LocalScale = Vector3(1.0f, 1.0f, 1.0f);
//	Transform* m_Parent;
//	int m_childCount;
//	void AddParent(Transform* parent);



//ファイルへの読み書き

};
