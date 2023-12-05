#pragma once

#include "gameObject.h"
#include "component.h"
#include <set>
#include <unordered_map>

enum Shape
{
    SHAPE_NONE = 0,
    SHAPE_BOX,
    SHAPE_SPHERE,
};

enum CollisionState
{
    COLLISION_NONE = 0,
    COLLISION_ENTER,
    COLLISION_STAY,
    COLLISION_EXIT,
};

class CollisionShape :public Component
{
protected:
    bool m_Trigger = false; //trueにすると補正はせず当たり判定だけ取る 
    Shape m_Shape = SHAPE_NONE;
    GameObject* m_Object;

    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};

    D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 m_OldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 m_Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //コールバック 
    using CollisionCallback = std::function<void(CollisionState, CollisionShape*)>;
    CollisionCallback m_CollisionCallback;

    std::unordered_map<CollisionShape*, CollisionState> m_State;
public:
    CollisionShape();
    virtual ~CollisionShape() = default;

    void Init() override {};
    void Uninit() override {};
    void Update() override {};
    void Draw() override {};

    //形が増えたら追加しなきゃいけない　
    virtual bool CheckCollision(CollisionShape* other) = 0;
    virtual bool CollideWith(class BoxCollision* other) = 0;
    virtual bool CollideWith(class SphereCollision* other) = 0;

    void SetTrigger(bool trigger) { m_Trigger = trigger; }
    void SetShape(Shape shape) { m_Shape = shape; }
    void SetPosition(D3DXVECTOR3 position) { m_Position = position; }
    void SetOldPosition(D3DXVECTOR3 position) { m_OldPosition = position; }
    void SetOffset(D3DXVECTOR3 offset) { m_Offset = offset; }
    void SetObject(GameObject* object) { m_Object = object; }
    void SetStateMap(CollisionShape* shape, CollisionState state) { m_State[shape] = state; }

    bool GetTrigger() { return m_Trigger; }
    Shape GetShape() { return m_Shape; }
    D3DXVECTOR3 GetPosition() { return m_Position; }
    D3DXVECTOR3 GetOldPosition() { return m_OldPosition; }
    D3DXVECTOR3 GetOffset() { return m_Offset; }
    GameObject* GetObjct() { return m_Object; }
    CollisionState GetState(CollisionShape* shape) { return m_State[shape]; }
    std::unordered_map<CollisionShape*, CollisionState> GetStateMap(){ return m_State; }

    //コールバック 
    void SetCollisionCallback(const CollisionCallback& callback) { m_CollisionCallback = callback; }
    CollisionCallback GetCollisionCallback() { return m_CollisionCallback; }
};

