#pragma once

#include"collisionShape.h"

class SphereCollision : public CollisionShape
{
private:
    #define VERTEX_NUM  31

    ID3D11Buffer* m_VertexBuffer[3]{};

    float   m_Size;

    void SetVertex(VERTEX_3D* vertex, int index);
public:
    SphereCollision(){ m_Shape = SHAPE_SPHERE; }

    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    void SetSize(float size) { m_Size = size; }
    float GetSize() { return m_Size; }

    bool CheckCollision(CollisionShape* other) override { return other->CollideWith(this); }
    bool CollideWith(BoxCollision* other) override;
    bool CollideWith(SphereCollision* other) override;
};
