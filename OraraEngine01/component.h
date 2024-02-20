#pragma once
#include <string>
#include <variant>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>

#define SET_COMPONENT_CLASS(Classname)\
CEREAL_REGISTER_TYPE(Classname)\
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Classname)

//前方宣言
class Vector3;

//lsitに追加したい型をここに書く
#define VARIATDATE int*, float*,std::string*, bool*,Vector3*,struct D3DXCOLOR*,FolderPass*,CustomVector3*,CustomFloat*
#define SETDATE(T) SetDateList(#T,&T)

enum VariableDate {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VECTOR3,
    TYPE_D3DXCOLOR,
    TYPE_FOLDERPASS,
    TYPE_CUSTOMVECTOR3,
    TYPE_CUSTOMFLOAT
};

enum CastomVector3State
{
    CASTOM_VECTOR_STATE＿CORRECTION
};

enum CastomFloatState
{
    CASTOM_FLOAT_STATE_NONE,
    CASTOM_FLOAT_STATE_SLIDER
};

struct CustomFloat 
{
    float* Date{};
    float Min = 0.0f;
    float Max = 1.0f;
    CastomFloatState State;
    
};

//フォルダ内のファイルを使用する場合はこっちを使う
struct FolderPass
{
    std::string Date{};
    std::string Pass{};//フォルダアクセスする場合に指定する
    std::string Extension{'.'};//拡張子の指定無ければ指定しない
    bool IsSet = false;

};

struct CustomVector3
{
    Vector3* Vector3date;
    CastomVector3State State;
};

struct TypeDate
{
    std::string Name;//メンバ変数の名前
    std::variant<VARIATDATE> MemberDate;//メンバ変数のデータ
};

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

    std::vector<TypeDate> GetDateList()
    {
        return m_DataList;
    }

    void SetDateList(std::string name, std::variant<VARIATDATE> date)
    {
        name = name.substr(name.find('_') + 1);
        TypeDate typedate;
        typedate.Name = name;
        typedate.MemberDate = date;
        m_DataList.push_back(typedate);
    }

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
