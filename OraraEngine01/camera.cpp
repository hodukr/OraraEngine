#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "cameracm.h"
#include "transform.h"
Cameracm::Cameracm()
{

}
void Cameracm::Init()
{
    m_GameObject->m_Transform->SetPosition(0.0f, 5.0f, -10.0f);
}

void Cameracm::Uninit()
{
    Renderer::SetViewMatrix(new D3DXMATRIX);

}

void Cameracm::Update()
{
    m_Target = m_GameObject->m_Transform->GetForward() + m_GameObject->m_Transform->GetPosition().dx();
}

void Cameracm::Draw()
{
    //ビューマトリクス設定 
    D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 position = m_GameObject->m_Transform->GetPosition().dx();
    D3DXVECTOR3 target = m_Target.dx();

    D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

    Renderer::SetViewMatrix(&m_ViewMatrix);


    //プロジェクションマトリクス設定 
    D3DXMATRIX projectionMatrix;
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    Renderer::SetProjectionMatrix(&projectionMatrix);

    Renderer::SetCameraPosition(m_GameObject->m_Transform->GetPosition().dx());
}
