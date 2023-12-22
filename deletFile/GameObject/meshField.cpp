
#include "main.h"
#include "renderer.h"
#include "meshField.h"

float g_FieldHeight[NUM_VERTEX][NUM_VERTEX] =
{
	{3.0f,4.0f,3.0f,4.0f,4.0f,3.0f,2.0f,3.0f,6.0f,3.0f,4.0f,5.0f,4.0f,5.0f,4.0f,3.0f,2.0f,5.0f,2.0f,3.0f,2.0f},
	{4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{5.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,0.0f,0.0f,0.0f,3.0f,4.0f},
	{2.0f,3.0f,0.0f,3.0f,0.0f,0.0f,3.0f,0.0f,0.0f,-3.0f,-3.0f,0.0f,0.0f,0.0f,4.0f,0.0f,0.0f,0.0f,3.0f,0.0f,5.0f},
	{3.0f,4.0f,0.0f,0.0f,2.0f,0.0f,0.0f,0.0f,0.0f,-4.0f,-4.0f,0.0f,4.0f,5.0f,5.0f,0.0f,0.0f,0.0f,4.0f,0.0f,4.0f},
	{4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-3.0f,-3.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,3.0f},
	{5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,3.0f,5.0f,6.0f,4.0f,2.0f,2.0f},
	{4.0f,0.0f,2.0f,3.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,3.0f,1.0f,4.0f},
	{3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,5.0f},
	{2.0f,0.0f,0.0f,0.0f,2.0f,4.0f,0.0f,0.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f},
	{4.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,-4.0f,-4.0f,0.0f,0.0f,1.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,3.0f,0.0f,3.0f,0.0f,0.0f,4.0f},
	{4.0f,0.0f,0.0f,0.0f,-5.0f,-5.0f,0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,6.0f},
	{5.0f,6.0f,0.0f,0.0f,-4.0f,-4.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,5.0f,0.0f,0.0f,3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f},
	{2.0f,6.0f,0.0f,0.0f,6.0f,4.0f,0.0f,5.0f,1.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,0.0f,0.0f,4.0f},
	{4.0f,7.0f,0.0f,0.0f,6.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,3.0f},
	{4.0f,7.0f,0.0f,0.0f,3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,0.0f,0.0f,2.0f,5.0f},
	{5.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,2.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,0.0f,4.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,3.0f},
	{4.0f,2.0f,3.0f,4.0f,5.0f,3.0f,4.0f,3.0f,3.0f,2.0f,5.0f,4.0f,4.0f,4.0f,3.0f,4.0f,5.0f,4.0f,3.0f,4.0f,3.0f},
};


void MeshField::Init()
{

	// 頂点バッファ生成
	{
		for (int x = 0; x <= NUM_VERTEX - 1; x++)
		{
			for (int z = 0; z <= NUM_VERTEX - 1; z++)
			{
				m_Vertex[x][z].Position = D3DXVECTOR3((x - 10) * 5.0f, g_FieldHeight[z][x], (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル
				m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}

		//法線ベクトル算出
		for (int x = 1; x <= NUM_VERTEX - 2; x++)
		{
			for (int z = 1; z <= NUM_VERTEX - 2; z++)
			{
				D3DXVECTOR3 vx, vz, vn;
				vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
				vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

				D3DXVec3Cross(&vn, &vz, &vx);//外積
				D3DXVec3Normalize(&vn, &vn);//正規化
				m_Vertex[x][z].Normal = vn;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX * NUM_VERTEX;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}




	// インデックスバッファ生成
	{
		unsigned int index[((NUM_VERTEX + 1) * 2) * (NUM_VERTEX - 1) - 2 ];

		int i = 0;
		for (int x = 0; x < (NUM_VERTEX - 1); x++)
		{
			for (int z = 0; z < NUM_VERTEX; z++)
			{
				index[i] = x * NUM_VERTEX + z;
				i++;

				index[i] = (x + 1) * NUM_VERTEX + z;
				i++;
			}

			if (x == NUM_VERTEX - 2)
				break;

			//縮退ポリゴン
			index[i] = (x + 1) * NUM_VERTEX + (NUM_VERTEX - 1);
			i++;

			index[i] = (x + 1) * NUM_VERTEX;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (((NUM_VERTEX + 1) * 2) * (NUM_VERTEX - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}




	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
											"asset/texture/field.jpg",
											NULL,
											NULL,
											&m_Texture,
											NULL);
	assert( m_Texture );


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");


}


void MeshField::Uninit()
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}


void MeshField::Update()
{

}


void MeshField::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// マトリクス設定
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_Transform->GetScale().dx();
    D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_Transform->GetPosition().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// マテリアル設定
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	material.TextureEnable = true;
	Renderer::SetMaterial( material );

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(((NUM_VERTEX + 1) * 2) * (NUM_VERTEX - 1) - 2, 0, 0);

}

float MeshField::GetHeight(D3DXVECTOR3 position)
{
	int x, z;

	//ブロック番号算出
	x = position.x / 5.0f + 10.0f;
    z = position.z / -5.0f + 10.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;
	
	D3DXVECTOR3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;
	if (c.y > 0.0f)
	{
		//左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		//右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//高さ取得
	py = -((position.x - pos1.x) * n.x 
		     + (position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

