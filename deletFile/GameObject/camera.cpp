#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"

void Camera::Init()
{
	m_Transform->SetPosition(0.0f, 5.0f, -10.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	m_flag = false;
}

void Camera::Update()
{ 
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//トップビュー(3Dマリオ)
	//Playerを中心に追従するカメラ
	/*m_Target = player->GetPosition();
	m_Position = m_Target + D3DXVECTOR3(0.0f, 5.0f, 0.0f);

	static float rot = 0.0f;
	m_Position += D3DXVECTOR3(cosf(rot), 0.0f, sinf(rot)) * 5.0f;
	rot+=0.01;*/

	//サードパーソンビュー(フォートナイト)
	
	m_Target = player->m_Transform->GetPosition();
	Vector3 position  = m_Target - player->m_Transform->GetForward() * 10.0f + Vector3::Up() * 10.0f;
    m_Transform->SetPosition(position);


	//ファーストパーソンビュー
	//m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	//m_Target = m_Position + player->GetForward();

	//自分
	/*static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Target = player->GetPosition() + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	pos = m_Target - player->GetForward() * 5.0f + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 dir = pos - m_Position;
	if (D3DXVec3Length(&dir) > 0.4f)
	{
		if (D3DXVec3Length(&dir) >= 10.0f)
			m_flag = true;

		if (m_flag == true)
		{
			D3DXVec3Normalize(&dir, &dir);
			m_Position += dir * 0.4f;
		}
		else
		{
			D3DXVec3Normalize(&dir, &dir);
			m_Position += dir * 0.1f;
		}
	}
	else
	{
		m_flag = false;
	}*/
}

void Camera::Draw()
{
    //ビューマトリクス設定
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 position = m_Transform->GetPosition().dx();
    D3DXVECTOR3 target = m_Target.dx();

	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	//プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

