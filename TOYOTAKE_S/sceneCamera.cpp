#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "sceneCamera.h"
#include "com_transform.h"
#include "input.h"
#include "imgui/imgui.h"


void SceneCamera::Init()
{
    m_Position = Vector3(0.0f, 5.0f, -10.0f);

}

void SceneCamera::Uninit()
{
    //Renderer::SetViewMatrix(new D3DXMATRIX);
}

void SceneCamera::Update()
{
    //マウスが右クリックされていたら
    if (Input::Instance().GetMouseInput(MOUSE_DOWN_RIGHT))
    {

        Vector3 vel = Vector3(0.0f, 0.0f, 0.0f);
        if (Input::Instance().GetKeyPress('W'))
        {
            vel.z = 1.0f;
        }
        if (Input::Instance().GetKeyPress('S'))
        {
            vel.z = -1.0f;
        }
        if (Input::Instance().GetKeyPress('D'))
        {
            vel.x = 1.0f;
        }
        if (Input::Instance().GetKeyPress('A'))
        {
            vel.x = -1.0f;
        }
        if (Input::Instance().GetKeyPress('Q'))
        {
            vel.y = 1.0f;
        }
        if (Input::Instance().GetKeyPress('E'))
        {
            vel.y = -1.0f;
        }
        vel.NormalizThis();
        m_Position += vel;
    }
    if (m_IsMouseUp)
    {
        //左クリックが押された瞬間のポジションとカメラの回転角度を取得
        if (Input::Instance().GetMouseInput(MOUSE_TRIGGER_LEFT))
        {
            m_LeftClickMousePos = Input::Instance().GetMousePos();
            m_LeftClickCameraRot = m_Rotation;
            m_IsMouseUp = true;
        }

        //左クリックされている間マウスの移動距離分カメラを回転させる
        if (Input::Instance().GetMouseInput(MOUSE_DOWN_LEFT))
        {
            Vector2 vec{};
            vec = Input::Instance().GetMousePos() - m_LeftClickMousePos;
            m_Rotation = Vector3(m_LeftClickCameraRot + (Vector3().Right() * ((vec.y / SCREEN_HEIGHT) * D3DX_PI / 2.0f) +
                Vector3().Up() * ((vec.x / SCREEN_WIDTH * 1.0f) * D3DX_PI / 2.0f)));
            m_IsMouseUp = true;
        }
    }
    else
    {
        m_LeftClickMousePos = Input::Instance().GetMousePos();
        m_LeftClickCameraRot = m_Rotation;
    }

    m_Target = GetForward() + m_Position.dx();
   
    Vector3 pos = GetWorldMousePosition(Input::Instance().GetMousePos().x, Input::Instance().GetMousePos().y);
    //ImGui::Text("X:%f Y:%f Z:%f",pos.x,pos.y,pos.z);
}

void SceneCamera::Draw()
{
    //ビューマトリクス設定 
    D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 position = m_Position.dx();
    D3DXVECTOR3 target = m_Target.dx();

    D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

    Renderer::SetViewMatrix(&m_ViewMatrix);


    //プロジェクションマトリクス設定 
    D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    Renderer::SetProjectionMatrix(&m_ProjectionMatrix);

    Renderer::SetCameraPosition(position);
}

// スクリーン座標をワールド座標に変換
Vector3 SceneCamera::GetWorldMousePosition(float x, float y)
{
    // 各行列の逆行列を算出
    D3DXMATRIX invView, invPrj, vp, invViewport;
    D3DXMatrixInverse(&invView, NULL, &m_ViewMatrix);
    D3DXMatrixInverse(&invPrj, NULL, &m_ProjectionMatrix);
    D3DXMatrixIdentity(&vp);
    vp._11 = SCREEN_WIDTH / 2.0f; vp._22 = -SCREEN_HEIGHT / 2.0f;
    vp._41 = SCREEN_WIDTH / 2.0f; vp._42 = SCREEN_HEIGHT / 2.0f;
    D3DXMatrixInverse(&invViewport, NULL, &vp);

    // 逆変換
    D3DXMATRIX tmp = invViewport * invPrj * invView;
    D3DXVECTOR3 mouPos = D3DXVECTOR3(x, y, 0.0f);
    Vector3 pOut{};
    D3DXVec3TransformCoord(pOut, &mouPos, &tmp);

    return pOut;
}

// XZ平面とスクリーン座標の交点算出関数
//Vector3* SceneCamera::CalcScreenToXZ(Vector3* pout,int x,int y)
//{
//    Vector3 nearpos;
//    Vector3 farpos;
//    Vector3 ray;
//    GetWorldMousePosition(&nearpos, x, y, 0.0f);
//    GetWorldMousePosition(&farpos, x, y, 1.0f);
//    ray = farpos.dx() - nearpos.dx();
//    D3DXVec3Normalize(ray, ray);
//
//    // 床との交差が起きている場合は交点を
//    // 起きていない場合は遠くの壁との交点を出力
//    if (ray.y <= 0)
//    {
//        // 床交点
//        Vector3 up = Vector3(0, 1, 0);
//        float Lray = D3DXVec3Dot(ray, up);
//        float LP0 = D3DXVec3Dot(-nearpos, up);
//        *pout = nearpos + (LP0 / Lray) * ray.dx();
//    }
//    else
//    {
//        *pout = farpos;
//    }
//
//    return pout;
//}