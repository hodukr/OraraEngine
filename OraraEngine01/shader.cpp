#include "main.h"
#include "renderer.h"
#include "shader.h"

void ShaderDate::Load(const std::string file)
{
	if (m_VertexLayout || m_VertexShader || m_PixelShader)return;
	m_File = file;
	std::string vs = "shader\\" + file + "VS.cso";
	std::string ps = "shader\\" + file + "PS.cso";
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, vs.c_str());
	Renderer::CreatePixelShader(&m_PixelShader, ps.c_str());
}

void ShaderDate::SetShader()
{
	//入力レイアウト 
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
}

void ShaderDate::Uninit()
{
	if(m_VertexLayout)m_VertexLayout->Release();
	if (m_VertexShader)m_VertexShader->Release();
	if (m_PixelShader)m_PixelShader->Release();
}


