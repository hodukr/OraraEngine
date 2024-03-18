#pragma once
#include <string>
class ShaderDate
{
private:
	string m_File{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
public:
	ShaderDate(string file) { Load(file); }
	void Load(string file);
	void SetShader();
	void Uninit();
	string GetFile() { return m_File; }
};