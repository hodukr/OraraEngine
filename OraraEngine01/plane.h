#pragma once
#include "component.h"

class Plane : public Component
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

public:
    void Init()override;
	void Init(float x, float y, float width, float height, const  char* texture);
	void Uninit();
	void Update();
	void Draw();
};

