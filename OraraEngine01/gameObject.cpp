#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "scene.h"

void GameObject::SetName(std::string name)
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
                std::string objnum = "(" + std::to_string(num) + ")";
                if (name == obj->GetName() && obj.get() != this)
                {
                    name = m_ObjctName + "(" + std::to_string(num) + ")";
                    flg = true;
                    num++;
                    break;
                }
            }
        }

    };
    m_ObjctName = name;
}
