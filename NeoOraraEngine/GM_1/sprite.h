#pragma once
#include "component.h"

class Sprite:public Component
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
	D3DXVECTOR2 m_UV;
	D3DXVECTOR2 m_UVlen;
	VERTEX_3D m_vertex[4];

	D3DXCOLOR m_Color;
public:
	void SetUV(D3DXVECTOR2 uv)
	{
		m_UV = uv;
	}
	void SetUVlen(D3DXVECTOR2 uv)
	{
		m_UVlen = uv;
	}
	void Init(D3DXVECTOR2 position, D3DXVECTOR2 size, const char* textureName);
	void Init(float x, float y, float wid,float hei, const char* textureName);
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR color)
	{
		m_Color = color;
	}

};	
