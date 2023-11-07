#pragma once
#include "gameObject.h"
#include "model.h"

class Child : public GameObject
{
private:
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	class Player* m_Parent;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetParent(Player* Parent) { m_Parent = Parent; }
};
