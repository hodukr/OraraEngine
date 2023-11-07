#pragma once
#include "gameObject.h"

class Sprite;

class Fade:public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	bool InOutflg;
	Sprite* m_Sprite;
	D3DXCOLOR m_Color;

public:
	void Init(char* texture);
	void Uninit();
	void Update();
	void Draw();
};