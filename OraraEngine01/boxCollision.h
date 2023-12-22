#pragma once

#include"collisionShape.h"

class BoxCollision : public CollisionShape
{
private:
    ID3D11Buffer* m_VertexBuffer{};

    D3DXVECTOR3 m_Size{};

    void SetVertex(VERTEX_3D* vertex);
public:
    BoxCollision() { m_Shape = SHAPE_BOX; }

    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    void SetSize(D3DXVECTOR3 size) { m_Size = size; }
    D3DXVECTOR3 GetSize() { return m_Size; }

    bool CheckCollision(CollisionShape* other) override { return other->CollideWith(this); }
    bool CollideWith(BoxCollision* other) override;
    bool CollideWith(SphereCollision* other) override;
};
