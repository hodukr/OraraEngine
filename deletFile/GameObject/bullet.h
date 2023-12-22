#pragma once
#include "model.h"
#include "gameObject.h"

class Bullet :public GameObject
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

    Vector3 m_Velocity{};
public:
	static void Load();
	static void Unload();


	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetBullet(Vector3 Position, Vector3 Velosity) {
		m_Velocity = Velosity;
		m_Transform->SetPosition(Position);
	}
};
