#pragma once
#include "gameObject.h"

class MouseHit:public GameObject
{
private:
	POINT m_MousePos;
	class Model* m_Model;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
};