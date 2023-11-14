#include "main.h"
#include "transform.h"
#include <cmath>
Vector3 Transform::GetRight()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Vector3 right;
	right.x = rot._11;
	right.y = rot._12;
	right.z = rot._13;
	
	return right;
}

Vector3 Transform::GetForward()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Vector3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;

	return forward;	
}

Vector3 Transform::GetUp()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Vector3 up;
	up.x = rot._21;
	up.y = rot._22;
	up.z = rot._23;

	return up;
}

void Transform::Revolution(Vector3 center, Vector3 axis, float angle, bool isObjRot)
{
	Vector3 vector = center - m_Position;

	// ƒ‰ƒWƒAƒ“‚É•ÏŠ·
	float radian = angle * static_cast<float>(D3DX_PI) / 180.0f;

	// ‰ñ“]s—ñ‚Ì—v‘f‚ðŒvŽZ
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	float oneMinusCosTheta = 1.0f - cosTheta;

	// ‰ñ“]s—ñ‚ÌŠe—v‘f‚ðŒvŽZ
	float x = vector.x * (cosTheta + oneMinusCosTheta * axis.x * axis.x) +
		vector.y * (oneMinusCosTheta * axis.x * axis.y - axis.z * sinTheta) +
		vector.z * (oneMinusCosTheta * axis.x * axis.z + axis.y * sinTheta);

	float y = vector.x * (oneMinusCosTheta * axis.x * axis.y + axis.z * sinTheta) +
		vector.y * (cosTheta + oneMinusCosTheta * axis.y * axis.y) +
		vector.z * (oneMinusCosTheta * axis.y * axis.z - axis.x * sinTheta);

	float z = vector.x * (oneMinusCosTheta * axis.x * axis.z - axis.y * sinTheta) +
		vector.y * (oneMinusCosTheta * axis.y * axis.z + axis.x * sinTheta) +
		vector.z * (cosTheta + oneMinusCosTheta * axis.z * axis.z);

	m_Position = Vector3(x, y, z) + center;

  
}



void Transform::Init()
{

}

void Transform::Uninit()
{

}

void Transform::Update()
{

}

//***********************************************************************************
//ˆÈ‰ºeŽqŠÖŒWì‚éŠ›

//void Transform::AddParent(Transform* parent)
//{
//	if (!parent)
//	{
//		m_Parent = parent;
//		m_Position = parent->GetWorldPosition() - GetWorldPosition();
//		m_Rotation = parent->GetWorldRotation() - GetWorldRotation();
//		m_Scale = GetWorldScale() / parent->GetWorldScale();
//	}
//}