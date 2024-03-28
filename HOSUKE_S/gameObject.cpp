#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "scene.h"
#include "timeProvider.h"

void GameObject::SetDestroy(float time)
{
    m_Destroy = true;
    if (time > 0.0f)
    {
        m_Timer = new ElapsedTimeTracker;
        m_DeletTime = time;
    }
}

bool GameObject::Destroy()
{
    if (m_Destroy)
    {
        if (m_Timer)
        {
            if (m_DeletTime <= m_Timer->GetElapsedTimeInSeconds())
            {
                Uninit();
            }
            else
            {
                return false;
            }
        }
        Uninit();
        //delete this;
        return true;
    }
    else
    {
        m_Component.remove_if([](const unique_ptr<Component>& component) {return component->Destroy(); });//ラムダ式
        return false;
    }
}

void GameObject::SetName(string name)
{
    //ゲームオブジェクト名がかぶってないかを調べる 
    bool flg = true;
    int num = 1;
    Scene* scene = Manager::GetScene();

    m_ObjctName = name;
    while (flg)
    {
        flg = false;
        for (size_t i = 0; i < 3; i++)
        {
            for (auto& obj : scene->GetList()[i])
            {
                string objnum = "(" + to_string(num) + ")";
                if (name == obj->GetName() && obj.get() != this)
                {
                    name = m_ObjctName + "(" + to_string(num) + ")";
                    flg = true;
                    num++;
                    break;
                }
            }
        }

    };
    m_ObjctName = name;
}
