#include"main.h"
#include"renderer.h"
#include "material.h"

void Material::Init()
{
    std::string name = "shader\\" + m_NameVS;
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, name.c_str());
    name = "shader\\" + m_NamePS;
    Renderer::CreatePixelShader(&m_PixelShader, name.c_str());
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

void Material::SetShaderVS(std::string Shader)
{
    m_NameVS = Shader;
    if (m_VertexLayout)
        m_VertexLayout->Release();
    if (m_VertexShader)
        m_VertexShader->Release();

    std::string name = "shader\\" + m_NameVS;

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, name.c_str());
}

void Material::SetShaderPS(std::string Shader)
{
    m_NamePS = Shader;
    if(m_PixelShader)
    m_PixelShader->Release();

    std::string name = "shader\\" + m_NamePS;


    Renderer::CreatePixelShader(&m_PixelShader, name.c_str());
}



