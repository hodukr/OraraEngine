#include "main.h"
#include "renderer.h"
#include "post.h"
#include "pass_postPass.h"
#include "shaderManager.h"
#include "textureManager.h"

void Post::Init()
{
    VERTEX_3D vertex[4];

    vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

    vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

    vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

    vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
    vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


    // 頂点バッファ生成
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

    m_DefaultShader = ShaderManager::Instance().LoadShader("unlitTexture");
    m_PostShader = ShaderManager::Instance().LoadShader("postNoise");

    //コンスタントバッファに送るパラメーターの初期化  
    ZeroMemory(&m_Water, sizeof(WATER));
    //波紋用ステータス 
    m_Water.WaveAmplitude = 0.05f;                   //波の感覚：0.00～0.10   
    m_Water.WaveFrequency = 50.0f;                  // 波の数多いほど細かく：0.0～100.0   
    m_Water.Pos = D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
    m_Water.Speed = 0.01f;                          //中心から広がっていく速度：0.0～1.0

    ZeroMemory(&m_Param, sizeof(PARAMETER));
    m_Param.dissolveThreshold = 0.0f;
    m_Param.dissolveRange = 0.1f;
}


void Post::Uninit()
{
    m_VertexBuffer->Release();
}


void Post::Update()
{
    if (ShaderManager::Instance().GetShader(m_PostShader)->GetFile() == "waterSurface")
        m_Water.Time++;
    else if (ShaderManager::Instance().GetShader(m_PostShader)->GetFile() == "wipe")
    {
        m_Param.dissolveThreshold += m_Delta;

        if (m_Param.dissolveThreshold >= 1.1f || m_Param.dissolveThreshold <= -0.1f)
        {
            m_Delta *= -1;
        }
    }

}


void Post::Draw()
{
    if(m_IsPost)
     ShaderManager::Instance().GetShader(m_PostShader)->SetShader();
    else
     ShaderManager::Instance().GetShader(m_DefaultShader)->SetShader();

    // マトリクス設定 
    Renderer::SetWorldViewProjection2D();

    // 頂点バッファ設定 
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // マテリアル設定 
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    Renderer::SetMaterial(material);

    //レンダリングテクスチャを取得
    PostPass* post = ShaderManager::Instance().GetPass<PostPass>(SHADER_POST);
    
    //レンダリングテクスチャを0番にセット  
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, post->GetTexture());

    // プリミティブトポロジ設定 
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    if (ShaderManager::Instance().GetShader(m_PostShader)->GetFile() == "waterSurface")
        Renderer::SetWater(m_Water);
    else if (ShaderManager::Instance().GetShader(m_PostShader)->GetFile() == "wipe")
    {
        if (m_TextureNum > -1)
            Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, TextureManager::GetTexture(m_TextureNum));

        Renderer::SetParameter(m_Param);
    }

    // ポリゴン描画 
    Renderer::GetDeviceContext()->Draw(4, 0);
}
