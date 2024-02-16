#include"main.h"
#include"renderer.h"
#include "material.h"
#include "shaderManager.h"

void Material::Init()
{
	SetShader(m_ShaderName);
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
}

void Material::SetShader(std::string file)
{
	m_ShaderName = file;
	m_ShaderNum = ShaderManager::Instance().LoadShader(m_ShaderName);
}



