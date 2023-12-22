#pragma once
#include "gameObject.h"

class Score :public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};
	int m_TexNum{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	void AddCount(int Count) { m_Count += Count; }

};
