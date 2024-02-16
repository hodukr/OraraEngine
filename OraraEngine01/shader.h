#pragma once
#include <string>
class ShaderDate
{
private:
	std::string file{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
public:
	ShaderDate(std::string file) { Load(file); }
	void Load(std::string file);
	void SetShader();
	void Uninit();
	std::string GetFile() { return file; }
};