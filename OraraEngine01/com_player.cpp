#include "main.h"
#include "manager.h"
#include "shaderManager.h"
#include "gameObject.h"
#include "input.h"
#include "com_player.h"
#include "post.h"
void Player::Init()
{
    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    GameObject* goal = Manager::GetScene()->GetGameObjectToTag("Goal");
    GameObject* gameObject = Manager::GetScene()->GetGameObject("Water");
    if (gameObject)
    {
        m_WaterSurface = gameObject->GetComponent<WaterSurface>();
    }
    m_IsDead = false;
    if (goal)
    {
        m_ChangeScene = goal->GetComponent<ChangeScene>();
    }

    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {

                BoxCollision* boxother = (BoxCollision*)other;
                if (state == COLLISION_ENTER)
                {
                    if (other->GetGameObejct()->GetTag() == "Enemy")
                    {
                        GameObject* water = Manager::CreatePrefab("HitEnemy",m_GameObject->m_Transform->GetPosition());
                        water->SetDestroy(0.5f);
                        Dead();
                        other->GetGameObejct()->GetComponent<Enemy>()->SetEnable(false);
                    }
                    if (boxother->GetHitDirection(m_Collision) == BOXHITDIRECTION_UP)
                    {
                        m_IsGround++;
                        m_Velocity.y = -0.1f;
                        m_IsOldUpHit[boxother] = true;
                    }
                    else if (boxother->GetHitDirection(m_Collision) == BOXHITDIRECTION_DOWN)
                    {
                        m_Velocity.y = -0.1f;
                    }
                }
                else if (state == COLLISION_STAY)
                {
                    if (m_Collision->GetHitDirection(boxother) == BOXHITDIRECTION_DOWN)
                        m_Velocity.y = 0.0f;
                }
                else if (state == COLLISION_EXIT)
                {
                    if (m_IsOldUpHit[boxother])
                    {
                        m_IsGround--;
                        m_IsOldUpHit[boxother] = false;
                    }
                }
            });
    }
}
void Player::Uninit()
{

}
void Player::EditorUpdate()
{

}
void Player::Update()
{

    switch (m_DeadWave)
    {
    case 0:
        if (Input::Instance().GetKeyTrigger(VK_SPACE) && m_IsGround > 0)
        {
            m_JumpPower = 1.0f;
            //m_IsGround--;
            m_Velocity.y = m_JumpPower;
        }
        m_Velocity.y -= 0.1f;
        m_GameObject->m_Transform->Translate(m_Velocity);
        if (m_WaterSurface)
        {
            float groundHeight = m_WaterSurface->GetHeigt(m_GameObject->m_Transform->GetPosition());
            float difference = m_GameObject->m_Transform->GetPosition().y - groundHeight;
            if (difference < -3.0f)
            {
                GameObject* water = Manager::CreatePrefab("waterPraticle", m_GameObject->m_Transform->GetPosition());
                water->SetDestroy(0.5f);
                Dead();
            }
        }
        break;
    case 1:
    {
        m_HItTime++;
        if (m_HItTime >= 30)
        {
            m_DeadWave = 2;
        }
    }
        break;
    case 2:
    {
        m_Velocity.y -= 0.07f;
        m_GameObject->m_Transform->Translate(m_Velocity);
        if (m_WaterSurface)
        {

            float groundHeight = m_WaterSurface->GetHeigt(m_GameObject->m_Transform->GetPosition());
            float difference = m_GameObject->m_Transform->GetPosition().y - groundHeight;
            if (difference < -3.0f)
            {
                m_DeadWave = 3;
            }
        }
    }
        break;
    case 3:
    {
        m_DeadWave = 4;
        string newscene = Manager::GetScene()->GetName();
        Post* post = ShaderManager::Instance().GetPost();
        post->SetIsWipe(true);
        post->SetThreshold(0.0f);
        post->SetWipeSpeed(0.05f);
        if (!m_ChangeScene)return;
        m_ChangeScene->SetScene(newscene);
        m_ChangeScene->SetIsFadeOut(true);
    }
        break;
    default:
        break;
    }
}
void Player::Draw()
{
}

void Player::Dead()
{
    m_DeadWave = 1;
    m_DeadPosition = m_GameObject->m_Transform->GetPosition();
    InputSystem* is = m_GameObject->GetComponent<InputSystem>();
    if (is)is->SetEnable(false);
    
    BoxCollision* bc = m_GameObject->GetComponent<BoxCollision>();
    if (bc)bc->SetEnable(false);

    m_Velocity = Vector3::Zero();
    m_Velocity.y = 1.0f;
    GameObject* camera = Manager::GetScene()->GetGameObjectToTag("Camera");
    if (camera)
    {
        ShakeCamera* sc = camera->GetComponent<ShakeCamera>();
        if (sc)sc->SetShake(true);
    }
    

}



