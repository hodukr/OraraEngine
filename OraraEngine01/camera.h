#pragma once
#include "gameObject.h"

class Camera :public GameObject
{
private:

	Vector3 m_Target{};
	D3DXMATRIX m_ViewMatrix{};
	bool m_flag;

public:
	void Init()override;
	void Update()override;
	void Draw()override;

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
};

