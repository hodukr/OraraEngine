#include"main.h"
#include"rendererManager.h"
#include"camera.h"
void Camera::Init()
{
    m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
    m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
    //Player* player = Manager::GetScene()->GetGameObject<Player>();

    ////トップビュー
    //m_Target = player->GetPosition();
    //m_Position = m_Target + D3DXVECTOR3(0.0f, 3.0f, -5.0f);

    //サードパーソンビュー
    //m_Target = player->GetPosition();
    //m_Position = m_Target - player->GetForward() * 5.0f + D3DXVECTOR3(0.0f, 3.0f, 0.0f);

    //ファーストパーソンビュー
    //m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
    //m_Target = m_Position + player->GetForward();
}





void Camera::Draw()
{
    D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

    RendererManager::Instance().SetViewMatrix(&m_ViewMatrix);

    //プロジェクションマトリックス設定
    D3DXMATRIX projectionMatrix;
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    RendererManager::Instance().SetProjectionMatrix(&projectionMatrix);
}
