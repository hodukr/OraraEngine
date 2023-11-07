#pragma once

#include "component.h"

class Plane :public Component
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

public:
	void Init(D3DXVECTOR3 position, D3DXVECTOR2 size);
	void Init(float x,float y, float z, float wid,float hei);
	void Init(float x, float y, float z, float wid, float hei, char* name);
	void Uninit();
	void Update();
	void Draw();
};
