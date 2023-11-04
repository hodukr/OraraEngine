#pragma once
//循環参照になるので前方宣言
//class Scene;
//前方宣言はポインタで使う時のみ
//継承には使用不可

//#include "scene.h"

class EngineManager
{
private:
    //Manegerはインスタンスしないので静的に作る
    //static Scene* m_Scene;

public:
    static void Init();
    static void Uninit();
    static void Update();
    static void Draw();

    //static Scene* GetScene(void) { return m_Scene; }

    //template<typename T>
    /*static void SetScene()
    {
        if (m_Scene)
        {
            m_Scene->Uninit();
            delete m_Scene;
        }

        m_Scene = new T();
        m_Scene->Init();
    }*/

};
