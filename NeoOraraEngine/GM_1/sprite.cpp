#include "main.h"
#include "renderer.h"
#include "sprite.h"



void Sprite::Init(D3DXVECTOR2 position , D3DXVECTOR2 size,const char* textureName)
{


	m_vertex[0].Position = D3DXVECTOR3(position.x - (size.x/2.0f), position.y - (size.y / 2.0f), 0.0f);
	m_vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	m_vertex[1].Position = D3DXVECTOR3(position.x + (size.x / 2.0f), position.y - (size.y / 2.0f), 0.0f);
	m_vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	
	m_vertex[2].Position = D3DXVECTOR3(position.x - (size.x / 2.0f), position.y + (size.y / 2.0f), 0.0f);
	m_vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	m_vertex[3].Position = D3DXVECTOR3(position.x + (size.x / 2.0f), position.y + (size.y / 2.0f), 0.0f);
	m_vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	VERTEX_3D vertex[4];
	vertex[0] = m_vertex[0];
	vertex[1] = m_vertex[1];
	vertex[2] = m_vertex[2];
	vertex[3] = m_vertex[3];
    
	m_UV = D3DXVECTOR2(0.0f, 0.0f);
	m_UVlen= D3DXVECTOR2(1.0f, 1.0f);
    

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);



	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		textureName,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void Sprite::Init(float x, float y, float wid, float hei, const char * textureName)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(x - (wid / 2.0f), y - (hei / 2.0f), 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(x + (wid / 2.0f), y - (hei / 2.0f), 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(x - (wid / 2.0f), y + (hei / 2.0f), 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(x + (wid / 2.0f), y + (hei / 2.0f), 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);



	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		textureName,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);
}

void Sprite::Uninit()
{

	m_VertexBuffer->Release();
	m_Texture->Release();
}

void Sprite::Update()
{
	m_vertex[0].TexCoord = D3DXVECTOR2(m_UV.x			 , m_UV.y);
	m_vertex[1].TexCoord = D3DXVECTOR2(m_UV.x + m_UVlen.x, m_UV.y);
	m_vertex[2].TexCoord = D3DXVECTOR2(m_UV.x			 , m_UV.y + m_UVlen.y);
	m_vertex[3].TexCoord = D3DXVECTOR2(m_UV.x + m_UVlen.x, m_UV.y + m_UVlen.y);
}

void Sprite::Draw()
{
	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0] = m_vertex[0];
	vertex[1] = m_vertex[1];
	vertex[2] = m_vertex[2];
	vertex[3] = m_vertex[3];

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse =m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
