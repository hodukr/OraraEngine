#pragma once

#include"com_collisionShape.h"
enum BoxHitDirection
{
    BOXHITDIRECTION_NONE,
    BOXHITDIRECTION_FORWARD,
    BOXHITDIRECTION_BACK,
    BOXHITDIRECTION_RIGHT,
    BOXHITDIRECTION_LEFT,
    BOXHITDIRECTION_UP,
    BOXHITDIRECTION_DOWN,
};

class BoxCollision : public CollisionShape
{
private:
    ID3D11Buffer* m_VertexBuffer{};

    Vector3 m_Size{1.0f,1.0f,1.0f};
    Vector3 m_CorrectionSize{ 1.0f,1.0f,1.0f };
    std::unordered_map<BoxCollision*, BoxHitDirection> m_Directions;
    void SetVertex(struct VERTEX_3D* vertex);
public:
    BoxCollision() { 
        m_Shape = SHAPE_BOX; 
    }

    void DrawInspector()
    {
        CollisionShape::DrawInspector();
        SET_DATE(m_Size);
    }
    void Init() override;
    void Uninit() override;
    void EditorUpdate()override;
    void Update() override;
    void Draw() override;

    void SetSize(Vector3 size) { m_Size = size; }
    Vector3 GetSize() { return m_Size; }

    bool CheckCollision(CollisionShape* other) override { return other->CollideWith(this); }
    bool CollideWith(BoxCollision* other) override;
    bool CollideWith(SphereCollision* other) override;

    void SetHitDirection(BoxCollision* boxCollision,BoxHitDirection direction) { m_Directions[boxCollision] = direction; }
    BoxHitDirection GetHitDirection(BoxCollision* boxCollision) { return m_Directions[boxCollision]; }
    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_Dynamic), CEREAL_NVP(m_Offset), CEREAL_NVP(m_Size));
        }
        catch (const std::exception&)
        {

        }
    }
};
