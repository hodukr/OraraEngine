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
		SetTexture(m_TextureName,m_Extension);
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

void Material::SetShader(string file)
{
	m_ShaderName = file;
	m_ShaderNum = ShaderManager::Instance().LoadShader(m_ShaderName);
}

void Material::SetTexture(string name,string extension)
{
	m_TextureName = name;
	m_Extension = extension;
	string pass = "asset\\texture\\"+ name + m_Extension;
	m_TextureNum = TextureManager::LoadTexture(pass);
}



