#include"main.h"
#include"renderer.h"
#include "material.h"
#include "shaderManager.h"
#include "textureManager.h"
void Material::Init()
{
	SetShader(m_ShaderName);
	if (m_TextureName != "")
	{
		SetTexture(m_TextureName);
	}
}

void Material::Draw()
{ 

	ShaderManager::Instance().GetShader(m_ShaderNum)->SetShader();

	//マテリアル設定 
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = false;
	Renderer::SetMaterial(material);

	if (m_TextureNum >= 0)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, TextureManager::GetTexture(m_TextureNum));
	}
}

void Material::SetShader(std::string file)
{
	m_ShaderName = file;
	m_ShaderNum = ShaderManager::Instance().LoadShader(m_ShaderName);
}

void Material::SetTexture(std::string name)
{
	m_TextureName = name;
	std::string pass = "asset\\texture\\"+ name + ".png";
	m_TextureNum = TextureManager::LoadTexture(pass);
}



