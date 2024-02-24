#include"main.h"
#include "manager.h"
#include "renderer.h"
#include <thread>
#include "shaderManager.h"
#include "collisionManager.h" 
#include "scene.h"
#include "loading.h"

void Loading::AsyncLoad()
{
	// ここで m_NextScene へのアクセスをロックする
	{
		std::lock_guard<std::mutex> lock(m_LoadedMutex);
		m_NextScene->Init(); // m_NextScene は nullptr の場合にアクセスしないように修正
		Manager::GetCollisionManager()->Init();
		GuiManager::Instance().Init();
		ShaderManager::Instance().Init();
	}

	m_Loaded = true;
}

void Loading::Init()
{
	Scene::Init();
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 150, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 150, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(PATTERN_X, 0.0f);

	vertex[2].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, PATTERN_Y);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(PATTERN_X, PATTERN_Y);

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

	m_TextureNum = TextureManager::LoadTexture((char*)"asset\\texture\\loading.png");
	m_ShaderNum = ShaderManager::Instance().LoadShader("unlitTexture");

	// バックグラウンドでリソースの非同期読み込みを開始
	 // std::async は返り値を返さない場合、将来的なステータスを取得する方法がないため使わない方が良いです。
	 // 代わりに std::thread を使用します。
	std::thread loadingThread(&Loading::AsyncLoad, this);
	loadingThread.detach(); // メインスレッドと分離
	//loadingThread.join(); // ロード完了まで待機

	// 初期化処理はバックグラウンドで実行されたので、ロードフラグは false のままです
}

void Loading::Uninit()
{

	// 次のシーンのリソースを解放し、クリーンアップ
	//if (m_NextScene != nullptr)
	//{
	//    delete m_NextScene;
	//    m_NextScene = nullptr;
	//}
	//Scene::Uninit();

	m_VertexBuffer->Release();

	// 読み込み済みフラグをリセット
	m_Loaded = false;
}

void Loading::Update()
{
	//Scene::Update();

	if (m_Loaded)
	{
		//ロード完了
		//m_NextScene->Draw();
		Manager::SetLoaded(m_NextScene);
	}
	else
	{
		static int count = 0;
		count++;

		if (count >= 10)
		{
			if (m_Count >= 12)
				m_Count = 0;

			m_Count++;
			count = 0;
		}
	}
}

void Loading::Draw()
{
	//Scene::Draw();

	//テクスチャ座標書き換え
	float u = m_Count % 3 * PATTERN_X;
	float v = m_Count / 3 * PATTERN_Y;

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 150, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(u, v);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 150, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(u + PATTERN_X, v);

	vertex[2].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(u, v + PATTERN_Y);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(u + PATTERN_X, v + PATTERN_Y);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	ShaderManager::Instance().GetShader(m_ShaderNum)->SetShader();

	Renderer::SetWorldViewProjection2D();

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, TextureManager::GetTexture(m_TextureNum));

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}


