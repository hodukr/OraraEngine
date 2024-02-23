#pragma once
#include "component.h"
#include "vector.h"
#include <DirectXMath.h>
using namespace DirectX;

class Transform :public Component
{
private:
	Vector3 m_Position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_OldPosition = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
    D3DXQUATERNION m_Qnaternion = D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f);
	Vector3 m_Scale = Vector3(1.0f, 1.0f, 1.0f);
    D3DXMATRIX m_Matrix{};

public:
    Transform()
    {
        m_Position = Vector3(0.0f, 0.0f, 0.0f);
        m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
        m_Scale = Vector3(1.0f, 1.0f, 1.0f);

    }
    void DrawInspector()override
    {
        SET_DATE(m_Position);
        SET_DATE_STATE(m_Rotation, CASTOMDRAWSTATE_VECTOR3_CORRECTION);
        SET_DATE(m_Scale);
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
    void SetDegreeToRadianRotation(Vector3 rot){ m_Rotation = D3DXToRadian(rot);}
    void SetQuaternionToRadian(D3DXQUATERNION quaternion)
    {// クオータニオンをオイラー角に変換
        float roll, pitch, yaw;
        float x = quaternion.x;
        float y = quaternion.y;
        float z = quaternion.z;
        float w = quaternion.w;

        // Roll (X軸の回転)
        float sinRoll = 2.0f * (w * x + y * z);
        float cosRoll = 1.0f - 2.0f * (x * x + y * y);
        roll = atan2f(sinRoll, cosRoll);

        // Pitch (Y軸の回転)
        float sinPitch = 2.0f * (w * y - z * x);
        if (fabsf(sinPitch) >= 1)
            pitch = copysign(D3DX_PI / 2, sinPitch); // 割り算の誤差を回避
        else
            pitch = asinf(sinPitch);

        // Yaw (Z軸の回転)
        float sinYaw = 2.0f * (w * z + x * y);
        float cosYaw = 1.0f - 2.0f * (y * y + z * z);
        yaw = atan2f(sinYaw, cosYaw);

        // ラジアンに変換
        //roll = XMConvertToRadians(roll);
        //pitch = XMConvertToRadians(pitch);
        //yaw = XMConvertToRadians(yaw);

        m_Rotation = Vector3(pitch, yaw, roll);
    }
    void SetQuaternion(D3DXQUATERNION qnaternion){m_Qnaternion = qnaternion;}
    void SetRotation(float x, float y, float z) { m_Rotation.x = x; m_Rotation.y = y; m_Rotation.z = z; }
    void SetRotationX(float x) { m_Rotation.x = x; }
    void SetRotationY(float y) { m_Rotation.y = y; }
    void SetRotationZ(float z) { m_Rotation.z = z; }

	Vector3 GetRotation() { return m_Rotation; }
    Vector3 GetDegreeRotation() { return D3DXToDegree(m_Rotation); }
    D3DXQUATERNION GetQuaternion() { return m_Qnaternion; }


    void SetScale(Vector3 sca) { m_Scale = sca; }
    void SetScale(float x, float y, float z) { m_Scale.x = x; m_Scale.y = y; m_Scale.z = z; }
    void SetScaleX(float x) { m_Scale.x = x; }
    void SetScaleY(float y) { m_Scale.y = y; }
    void SetScaleZ(float z) { m_Scale.z = z; }

    Vector3 GetScale() { return m_Scale; }



    void Translate(Vector3 moveVec) { m_Position += moveVec; }
    void Translate(float x, float y, float z) { m_Position += Vector3(x,y,z); }
	void Rotate(Vector3 rotVec) { m_Rotation += rotVec; }
	void Scale(Vector3 scaVec) { m_Scale += scaVec; }
	


	Vector3 GetRight();
	Vector3 GetForward();
	Vector3 GetUp();


	void Revolution(Vector3 target, Vector3 axis, float rot, bool isObjRot = false);

    void SetMatrix(D3DXMATRIX matrix) { m_Matrix = matrix; }
    D3DXMATRIX* GetMatrixPtr(){ return &m_Matrix; }
    D3DXMATRIX GetMatrix() { return m_Matrix; }

	void Init()override;
	void Uninit()override;
	void Update()override;
    void Draw()override;


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
        try
        {
            archive(CEREAL_NVP(m_Position), CEREAL_NVP(m_Rotation), CEREAL_NVP(m_Scale));

        }
        catch (const std::exception&)
        {

        }
    }
};

