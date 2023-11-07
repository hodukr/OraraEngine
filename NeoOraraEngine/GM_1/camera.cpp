#include"main.h"
#include "manager.h"
#include"renderer.h"
#include "scene.h"
#include"camera.h"
#include "player.h"
void Camera::Init() 
{
	m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	Player* player = Manager::GetScene()->GetGameObject<Player>();

	////�g�b�v�r���[
	//m_Target = player->GetPosition();
	//m_Position = m_Target + D3DXVECTOR3(0.0f, 3.0f, -5.0f);

	//�T�[�h�p�[�\���r���[
	m_Target = player->GetPosition();
	m_Position = m_Target - player->GetForward() *5.0f + D3DXVECTOR3(0.0f, 3.0f, 0.0f);

	//�t�@�[�X�g�p�[�\���r���[
	//m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	//m_Target = m_Position + player->GetForward();
}





void Camera::Draw()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����}�g���b�N�X�ݒ�
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}