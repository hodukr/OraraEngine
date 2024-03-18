#include "main.h"
#include "com_shakeCamera.h"
#include "gameObject.h"
void ShakeCamera::Init()
{
	camera = m_GameObject->GetComponent<Camera>();
}
void ShakeCamera::Uninit()
{

}
void ShakeCamera::EditorUpdate()
{

}
void ShakeCamera::Update()
{
	if (m_IsShake)
	{
		if (m_ShakeAmplitude <= 0.001f)
		{
			m_IsShake = false;
			m_ShakeAmplitude = 1.0f;
			if (camera)camera->SetOffset(Vector3::Zero());
			m_Offset = Vector3::Zero();
		}
		else 
		{
			m_Offset.x = sinf(m_ShakeTime) * m_ShakeVectol.x * m_ShakeAmplitude;
			m_Offset.y = sinf(m_ShakeTime) * m_ShakeVectol.y * m_ShakeAmplitude;
			m_Offset.z = sinf(m_ShakeTime) * m_ShakeVectol.z * m_ShakeAmplitude;
			m_ShakeTime+= m_ShakeSpeed;
			m_ShakeAmplitude *= m_AttenuationRate;
			if (camera)camera->SetOffset(m_Offset);
		}
	}


}

