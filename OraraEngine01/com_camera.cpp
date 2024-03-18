#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "gameObject.h"
#include "com_camera.h"
#include "com_transform.h"
Camera::Camera()
{
    m_DrawLayer = GAME_OBJECT_DRAW_LAYER_CAMERA;
}
void Camera::Init()
{
    m_Target = m_GameObject->m_Transform->GetForward() + m_GameObject->m_Transform->GetPosition().dx();;
}

void Camera::Uninit()
{
    Renderer::SetViewMatrix(new D3DXMATRIX);

}

void Camera::Update()
{
}

void Camera::Draw()
{
    m_Target = m_GameObject->m_Transform->GetForward() + m_GameObject->m_Transform->GetPosition().dx();
    if (Manager::GetSceneState() == SCENESTATE_SCENE)return;

    //ビューマトリクス設定 
    D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 position = m_GameObject->m_Transform->GetPosition().dx() + m_Offset.dx();
    D3DXVECTOR3 target = m_Target.dx();

    D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

    Renderer::SetViewMatrix(&m_ViewMatrix);


    //プロジェクションマトリクス設定 
   
    D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    Renderer::SetProjectionMatrix(&m_ProjectionMatrix);

    Renderer::SetCameraPosition(position);
}
