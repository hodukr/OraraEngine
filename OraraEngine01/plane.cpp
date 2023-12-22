#include "main.h"
#include "renderer.h"
#include "plane.h"
#include "gameObject.h"

void Plane::Init()
{
    Init(-50, -50, 100, 100, "asset\\texture\\field.jpg");
}

void Plane::Init(float x, float z, float width, float depth, const char* texture)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(x, 0.0f, z + depth);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(x + width, 0.0f, z + depth);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(10.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(x, 0.0f, z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 10.0f);

	vertex[3].Position = D3DXVECTOR3(x + width, 0.0f, z);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(10.0f, 10.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		texture,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);
}

void Plane::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}

void Plane::Update()
{
}

void Plane::Draw()
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_GameObject->m_Transform->GetScale().dx();
    D3DXVECTOR3 Rotation = m_GameObject->m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_GameObject->m_Transform->GetPosition().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;
    Renderer::SetWorldMatrix(&world);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
