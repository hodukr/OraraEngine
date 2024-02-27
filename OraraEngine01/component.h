#pragma once
#include <string>
#include <variant>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include "guiManager.h"
#include "guiw_inspector.h"

#define SET_COMPONENT_CLASS(Classname)\
CEREAL_REGISTER_TYPE(Classname)\
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Classname)

//前方宣言
class Vector3;

enum DrawLayer//描画されるレイヤーの指定
{
    GAME_OBJECT_DRAW_LAYER_NONE = -1,
    GAME_OBJECT_DRAW_LAYER_CAMERA,
    GAME_OBJECT_DRAW_LAYER_3D,
    GAME_OBJECT_DRAW_LAYER_2D,

};

class Component
{
protected:
    std::string m_ObjectName;
    DrawLayer m_DrawLayer{ GAME_OBJECT_DRAW_LAYER_NONE };
    class GameObject* m_GameObject;
    bool m_Destroy = false;
    std::vector<TypeDate> m_DataList;//メンバ変数を格納する
public:
    Component() {};
	virtual void Init() {};
	virtual void Uninit() {};
    virtual void EditorUpdate(){};
	virtual void Update() {};
	virtual void Draw() {};

    void SetObjectName(std::string name) { m_ObjectName = name; };
    void SetGameObejct(GameObject* gameobj) { m_GameObject = gameobj; }

    GameObject* GetGameObejct() {return m_GameObject; }
    std::string GettName() { return m_ObjectName; }
    const DrawLayer& GetDrawLayer() const { return m_DrawLayer; }

    void SetDestroy() { m_Destroy = true; }

    bool Destroy()
    {
        if (m_Destroy)
        {
            Uninit();
            return true;
        }
        else
        {
            return false;
        }
    }

    //std::vector<TypeDate> GetDateList()
    //{
    //    return m_DataList;
    //}

    //void SetDateList(std::string name, std::variant<VARIATDATE> date)
    //{
    //    name = name.substr(name.find('_') + 1);
    //    TypeDate typedate;
    //    typedate.Name = name;
    //    typedate.MemberDate = date;
    //    m_DataList.push_back(typedate);
    //}

    virtual void DrawInspector(){}

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_ObjectName));
        }
        catch (const std::exception&)
        {

        }
    }
};
