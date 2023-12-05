#include"main.h"
#include"renderer.h"
#include "material.h"

void Material::Init()
{
    m_NameVS = "shader\\vertexLightingVS.cso";
    m_NamePS = "shader\\vertexLightingPS.cso";
    m_Shader = "vertexLighting";
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_NameVS.c_str());

    Renderer::CreatePixelShader(&m_PixelShader, m_NamePS.c_str());
}

void Material::Init(const char* VS = "shader\\vertexLightingVS.cso", const char* PS = "shader\\vertexLightingPS.cso")
{
	//m_NameVS = VS;
	//m_NamePS = PS;

    m_NameVS = VS;
    m_NamePS = PS;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,VS);

	Renderer::CreatePixelShader(&m_PixelShader, PS);
}

void Material::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Material::Update()
{

}

void Material::Draw()
{ 
	//入力レイアウト 
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マテリアル設定 
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = false;
	Renderer::SetMaterial(material);
}

void Material::SetShader(std::string Shader)
{
    m_Shader = Shader;
    m_NameVS = "shader\\" + Shader + "VS.cso";
    m_NamePS = "shader\\" + Shader + "PS.cso";
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_NameVS.c_str());
    Renderer::CreatePixelShader(&m_PixelShader, m_NamePS.c_str());

}

