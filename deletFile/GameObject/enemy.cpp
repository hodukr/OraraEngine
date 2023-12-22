#include "main.h"
#include "renderer.h"
#include "boxCollision.h"
#include "sphereCollision.h"
#include "enemy.h"


void Enemy::Init()
{
    m_Model = new Model();
    m_Model->Load("asset\\model\\box.obj");


    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\boxCollisionVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\boxCollisionPS.cso");

    m_Collision = AddComponent<SphereCollision>();
    m_Collision->SetObject(this);
    //m_Collision->SetOffset(D3DXVECTOR3(0.0f, 3.0f, 0.0f));
}

void Enemy::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}
void Enemy::Update()
{
	//static  D3DXVECTOR3 rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	//m_Rotation = D3DXVECTOR3(rot.x, rot.y, rot.z);
	//rot.x += 0.1f;
	//rot.y += 0.1f;
 //   rot.z += 0.1f;
   
    GameObject::Update();
}

void Enemy::Draw()
{
    GameObject::Draw();
	//入力レイアウト設定

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_Transform->GetPosition().dx();
    D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_Transform->GetRotation().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
	
}
