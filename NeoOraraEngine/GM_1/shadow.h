#pragma once
#include "component.h"

class Shadow :public Component
{
private:
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	class Plane* m_plane;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetPos(const D3DXVECTOR3  pos) { m_Position = pos; }

};