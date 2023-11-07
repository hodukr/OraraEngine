#include "main.h"
#include "manager.h"
#include "scene.h"
#include "MouseHIt.h"
#include "renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "camera.h"
#include "model.h"
void MouseHit::Init()
{
    GameObject::Init();
    m_Model = new Model();
    m_Model->Load("asset\\model\\torus.obj");
    m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Scale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\vertexLightingVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\vertexLightingPS.cso");
}

void MouseHit::UnInit()
{
    GameObject::Uninit();
    m_Model->Unload();
    delete m_Model;

    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

}

void MouseHit::Update()
{
    Scene* scene = Manager::GetScene();
    Camera* camera = scene->GetGameObject<Camera>();
    GameObject::Update();

	GetCursorPos(&m_MousePos);
	ScreenToClient(GetWindow(), &m_MousePos);



    //スクリーン座標からビューポート座標への変換
    D3DXVECTOR3 screenSpace(m_MousePos.x, m_MousePos.y, 0.0f); // マウスのスクリーン座標
    D3DXVECTOR3 viewportSpace;
    viewportSpace.x = (2.0f * screenSpace.x / SCREEN_WIDTH) - 1.0f;
    viewportSpace.y = 1.0f - (2.0f * screenSpace.y / SCREEN_HEIGHT);
    viewportSpace.z = 0.0f; // スクリーン座標からの深度値（通常は0.0fを使用）

    //ビューポート座標からワールド座標への変換
    D3DXMATRIX projectionMatrix, viewMatrix;

    // プロジェクション行列、ビュー行列
    viewMatrix = camera->GetViewMatrix();

    // スクリーンのアスペクト比（幅 / 高さ）
    float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    // 画角（FOVY）、アスペクト比、近クリップ面、遠クリップ面を指定してプロジェクション行列を作成
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    D3DXMATRIX Matrix = viewMatrix * projectionMatrix;

    D3DXMATRIX inverseViewProjection; // ビュー行列とプロジェクション行列の逆行列を掛けた行列
    D3DXMatrixInverse(&inverseViewProjection, NULL, &Matrix);



    D3DXVECTOR3 worldSpace;
    D3DXVec3TransformCoord(&worldSpace, &viewportSpace, &inverseViewProjection);
    m_Position = worldSpace;
}

void MouseHit::Draw()
{
    ////入力レイアウト設定
    //Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    ////シェーダー設定
    //Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    //Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    ////マトリクス設定
    //D3DXMATRIX world, scale, rot, trans;

    //D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
    //D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
    //D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
    //world = scale * rot * trans;

    //Renderer::SetWorldMatrix(&world);

    //m_Model->Draw();
}

