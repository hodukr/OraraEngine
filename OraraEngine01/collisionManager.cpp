#include "main.h"
#include "manager.h"
#include "collisionManager.h"
#include "gameObject.h"
#include "imgui/imgui.h"

std::list<CollisionShape*>  CollisionManager::m_Shape{};
std::list<CollisionShape*>  CollisionManager::m_NextShape{};

void CollisionManager::Init()
{
    //m_Shape = m_NextShape;
    //m_NextShape.clear();
}

void CollisionManager::Uninit()
{
    m_Shape.clear();
}

void CollisionManager::Update()
{
    for (auto& shape : m_Shape)
    {
        if (shape->GetDynamic())
            shape->Update();
    }
    if(Manager::GetGameState() == GAMESTATE_PLAY){

        // すべての形状のペアに対して当たり判定を行う 
        for (auto it1 = m_Shape.begin(); it1 != m_Shape.end(); ++it1)
        {
            for (auto it2 = std::next(it1); it2 != m_Shape.end(); ++it2)
            {

                if (!(*it1)->GetStateMap().empty())
                {
                    auto stateMap1 = (*it1)->GetStateMap();
                    auto stateMap2 = (*it2)->GetStateMap();

                    auto i1 = stateMap1.find((*it2));
                    auto i2 = stateMap2.find((*it1));

                    if (i1 == stateMap1.end())
                    {
                        (*it1)->SetStateMap((*it2), COLLISION_NONE);
                    }
                    if (i2 == stateMap2.end())
                    {
                        (*it2)->SetStateMap((*it1), COLLISION_NONE);
                    }

                    bool hit = (*it1)->CheckCollision(*it2);

                    // 当たり判定状態の更新 
                    if (hit)
                    {
                        // 衝突が発生した場合 
                        if ((*it1)->GetState((*it2)) == COLLISION_NONE)
                        {
                            (*it1)->SetStateMap((*it2), COLLISION_ENTER);
                        }
                        else if ((*it1)->GetState((*it2)) == COLLISION_ENTER)
                        {
                            (*it1)->SetStateMap((*it2), COLLISION_STAY);
                        }

                        if ((*it2)->GetState((*it1)) == COLLISION_NONE)
                        {
                            (*it2)->SetStateMap((*it1), COLLISION_ENTER);
                        }
                        else if ((*it2)->GetState((*it1)) == COLLISION_ENTER)
                        {
                            (*it2)->SetStateMap((*it1), COLLISION_STAY);
                        }
                    }
                    else
                    {
                        // 衝突が発生していない場合 
                        if ((*it1)->GetState((*it2)) == COLLISION_STAY || (*it1)->GetState((*it2)) == COLLISION_ENTER)
                        {
                            (*it1)->SetStateMap((*it2), COLLISION_EXIT);
                        }
                        else
                        {
                            (*it1)->SetStateMap((*it2), COLLISION_NONE);
                        }

                        if ((*it2)->GetState((*it1)) == COLLISION_STAY || (*it2)->GetState((*it1)) == COLLISION_ENTER)
                        {
                            (*it2)->SetStateMap((*it1), COLLISION_EXIT);
                        }
                        else
                        {
                            (*it2)->SetStateMap((*it1), COLLISION_NONE);
                        }
                    }
                    ImGui::Text("%d", (*it1)->GetState((*it2)));
                }
                else
                {
                    (*it1)->SetStateMap((*it2), COLLISION_NONE);
                    if (!(*it2)->GetStateMap().empty())
                        (*it2)->SetStateMap((*it1), COLLISION_NONE);
                }

                if ((*it1)->GetCollisionCallback())
                {
                    (*it1)->GetCollisionCallback()((*it1)->GetState((*it2)), (*it2));
                }
                if ((*it2)->GetCollisionCallback())
                {
                    (*it2)->GetCollisionCallback()((*it2)->GetState((*it1)), (*it1));
                }
            }
            (*it1)->SetOldPosition((*it1)->GetPosition());
            (*it1)->GetGameObejct()->m_Transform->SetOldPosition((*it1)->GetGameObejct()->m_Transform->GetPosition());
        }
    }
}
