#pragma once

#include "gameObject.h"

class Score :public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_count{};
public:
	void SetCount(bool upDownflg)
	{
		if (upDownflg && m_count < 9)
			m_count++;
		else if (m_count > 0)
			m_count++;
	}
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
