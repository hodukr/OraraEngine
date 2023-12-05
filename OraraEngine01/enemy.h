#pragma once
#include "model.h"
#include "gameObject.h"


class Enemy : public GameObject
{
private:
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

    class CollisionShape* m_Collision;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

    void ResetPos(){
        m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }
};
