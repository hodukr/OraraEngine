#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "gameObject.h"
#include "textureManager.h"
#include "com_spriteAnimation.h"



void SpriteAnimation::Init()
{
	SetPattern();

	VERTEX_3D vertex[4];
	Vector3 size = m_GameObject->m_Transform->GetScale();

	vertex[0].Position = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(m_Pattern.x, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, m_Pattern.y);

	vertex[3].Position = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(m_Pattern.x, m_Pattern.y);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void SpriteAnimation::Uninit()
{
	m_VertexBuffer->Release();
	//m_Texture->Release();

}

void SpriteAnimation::Update()
{
	if (m_AnimFinish == false)
	{
		if (m_Loop == 0)
		{
			if (m_Count >= m_AnimSpeed)
			{
				if (m_AnimCount >= m_NumTexture)
					m_AnimCount = 0;

				m_AnimCount++;
				m_Count = 0;
			}
		}
		else
		{

			if (m_Count >= m_AnimSpeed)
			{
				if (m_AnimCount >= m_NumTexture)
				{
					m_Index++;
					if (m_Index >= m_Loop)
					{
						m_AnimFinish = true;
						return;
					}
					m_AnimCount = 0;
				}

				m_AnimCount++;
				m_Count = 0;
			}
		}
		m_Count++;
	}
}

void SpriteAnimation::Draw()
{
	if (m_AnimFinish == false)
	{
		//テクスチャ座標書き換え
		float u = m_AnimCount % (int)m_Divide.x * m_Pattern.x;
		float v = m_AnimCount / (int)m_Divide.x * m_Pattern.y;

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
		Vector3 size = m_GameObject->m_Transform->GetScale();
		vertex[0].Position = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(u, v);

		vertex[1].Position = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(u + m_Pattern.x, v);

		vertex[2].Position = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(u, v + m_Pattern.y);

		vertex[3].Position = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(u + m_Pattern.x, v + m_Pattern.y);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//カメラのビューマトリクス取得
		Scene* scene = Manager::GetScene();
		Camera* camera = scene->GetGameObject("MainCamera")->GetComponent<Camera>();
		D3DXMATRIX view = camera->GetViewMatrix();

		//ビューの逆行列
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);
		invView._41 = 0.0f;
		invView._42 = 0.0f;
		invView._43 = 0.0f;

		//マトリクス設定
		D3DXMATRIX world, scale, rot, trans;
		Vector3 gScale = m_GameObject->m_Transform->GetScale();
		Vector3 gPosition = m_GameObject->m_Transform->GetPosition();
		D3DXMatrixScaling(&scale, gScale.x, gScale.y, gScale.z);
		D3DXMatrixTranslation(&trans, gPosition.x, gPosition.y, gPosition.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(&world);

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}

void SpriteAnimation::SetPattern()
{
	m_Pattern.x = (1.0f / m_Divide.x);
	m_Pattern.y = (1.0f / m_Divide.y);
}

