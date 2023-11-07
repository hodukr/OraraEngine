#pragma once
#include "gameObject.h"

class Bullet : public GameObject
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
public:
	void SetVelocity(D3DXVECTOR3 vel) { m_Velocity = vel; }

	static void Load();
	static void Unroad();

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};