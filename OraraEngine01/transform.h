#pragma once
#include "component.h"
#include "vector.h"

class Transform :public Component
{
private:
	Vector3 m_Position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_OldPosition = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_Scale = Vector3(1.0f, 1.0f, 1.0f);
public:
    Transform()
    {
        m_Position = Vector3(0.0f, 0.0f, 0.0f);
        m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
        m_Scale = Vector3(1.0f, 1.0f, 1.0f);
    }
    void SetPosition(Vector3 pos) { m_Position = pos; }
    void SetPosition(float x,float y,float z) { m_Position.x = x; m_Position.y = y; m_Position.z = z;}
    void SetPositionX(float x) { m_Position.x = x; }
    void SetPositionY(float y) { m_Position.y = y; }
    void SetPositionZ(float z) { m_Position.z = z; }

	Vector3 GetPosition() { return m_Position; }

    void SetOldPosition(Vector3 pos) { m_OldPosition = pos; }
    void SetOldPosition(float x, float y, float z) { m_OldPosition.x = x; m_OldPosition.y = y; m_OldPosition.z = z; }
    void SetOldPositionX(float x) { m_OldPosition.x = x; }
    void SetOldPositionY(float y) { m_OldPosition.y = y; }
    void SetOldPositionZ(float z) { m_OldPosition.z = z; }

    Vector3 GetOldePosition() { return m_OldPosition; }
   
    void SetRotation(Vector3 rot) { m_Rotation = rot; }
    void SetRotation(float x, float y, float z) { m_Rotation.x = x; m_Rotation.y = y; m_Rotation.z = z; }
    void SetRotationX(float x) { m_Rotation.x = x; }
    void SetRotationY(float y) { m_Rotation.y = y; }
    void SetRotationZ(float z) { m_Rotation.z = z; }


	Vector3 GetRotation() { return m_Rotation; }

    void SetScale(Vector3 sca) { m_Scale = sca; }
    void SetScale(float x, float y, float z) { m_Scale.x = x; m_Scale.y = y; m_Scale.z = z; }
    void SetScaleX(float x) { m_Scale.x = x; }
    void SetScaleY(float y) { m_Scale.y = y; }
    void SetScaleZ(float z) { m_Scale.z = z; }

    Vector3 GetScale() { return m_Scale; }






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
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_Position), CEREAL_NVP(m_Rotation), CEREAL_NVP(m_Scale));
    }
};
CEREAL_REGISTER_TYPE(Transform);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component,Transform)
