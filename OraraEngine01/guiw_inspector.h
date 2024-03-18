#pragma once
#include "singleton.h"
#include "imGuiWindow.h"
#include <variant>

#define VARIATDATE int*, float*,string*, bool*,Vector3*,struct D3DXCOLOR*
#define SET_DATE(T) GuiManager::Instance().GetGuiWindow<Inspector>()->SetDate(#T, &T)
#define SET_NAME_DATE(NAME,T) GuiManager::Instance().GetGuiWindow<Inspector>()->SetDate(NAME, &T)
#define SET_DATE_STATE(T,S) GuiManager::Instance().GetGuiWindow<Inspector>()->SetDate(#T, &T,S)
#define SET_NAME_DATE_STATE(NAME,T,S) GuiManager::Instance().GetGuiWindow<Inspector>()->SetDate(NAME, &T,S)
#define SET_NEXT_SLIDER(MIN,MAX) GuiManager::Instance().GetGuiWindow<Inspector>()->SetSlider(MIN,MAX)
#define SET_NEXT_FOLDER(PASS,EXTENSION)GuiManager::Instance().GetGuiWindow<Inspector>()->SetFolder(PASS,EXTENSION)

class Vector3;

enum VariableDate {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VECTOR3,
    TYPE_D3DXCOLOR
};

enum CastomItemState
{// CASTOMDRAWSTATE_型名_ステータス
    CASTOMDRAWSTATE_NONE_NONE,
    CASTOMDRAWSTATE_FLOAT_SLIDER,//スライド
    CASTOMDRAWSTATE_STRING_FOLDER,//file参照
    CASTOMDRAWSTATE_STRING_GAMEOBJECT,//ゲームオブジェクト参照
    CASTOMDRAWSTATE_STRING_TAG,//tag参照
    CASTOMDRAWSTATE_VECTOR3_CORRECTION,//度に変換
};

struct TypeDate
{
    string Name{};//メンバ変数の名前
    CastomItemState State{};
    variant<VARIATDATE> MemberDate{};//メンバ変数のデータ
};

class Inspector:public GuiWindowBase
{
private:
    class GameObject* m_GameObject = nullptr;

    class Component* m_PopupComponent{};
    unordered_map<int,bool> m_IsRockVector{};
    int m_NumVector{};
    list<string> m_Taglist{};
    string m_DeletTag{};
    //vector<TypeDate> m_DataList;//Componentのメンバ変数を格納する

//component表示に必要な変数
    bool m_IsSet = false;//データが変更されたかを監視する
    //float
    // CASTOMDRAWSTATE_FLOAT_SLIDER 
    float m_SliderMin = 0.0f;
    float m_SliderMax = 1.0f;

    //string
    //CASTOMDRAWSTATE_STRING_FOLDER
    string m_AccessPass{};//アクセスするパス
    string m_Extension{};//型指定子
public:
    void Init();
    void Uninit();
    void Update();
    void SetWindowConfig()override;
    void Draw();
    void SetGameObejct(GameObject* obj){
        m_GameObject = obj;
        m_IsShowWindow = true;
    }
    const GameObject* GetGameObject(){return m_GameObject;}

    //コンポーネント表示
    bool SetDate(string name, variant<VARIATDATE> date, CastomItemState state = CASTOMDRAWSTATE_NONE_NONE)
    {
        BegenItemDraw();
        TypeDate typeDate;
        typeDate.MemberDate = date;
        if (name.find("m_") != string::npos)
        {
            name = name.substr(name.find("m_") + 2);
        }
        typeDate.Name = name;
        typeDate.State = state;
        Drawvariable(typeDate);
        return m_IsSet;
    }
    void SetSlider(float min, float max) { m_SliderMin = min; m_SliderMax = max; }
    void SetFolder(string pass, string extension) { m_AccessPass = pass; m_Extension = extension;}
private:
    void DrawComponent(class Component* component);
    void DrawMaterial();
    void DrawSetPass();
    void Drawvariable(struct TypeDate& vardate);
    vector<string> AccessFolder(const char* folderPass);//フォルダ内のファイル名の取得
    void CreatComponentFile(string comname);
    void AddFileToProject(const string& project_file, const string& file_path, bool is_header);
    void DeletTag(string tag);

    void DrawItemInt(TypeDate& date);
    void DrawItemFloat(TypeDate& date);
    void DrawItemString(TypeDate& date);
    void DrawItemBool(TypeDate& date);
    void DrawItemVector3(TypeDate& date);
    void DrawItemD3dxcolor(TypeDate& date);

    void BegenItemDraw()
    {
        m_IsSet = false;
    }
    void EndItemDraw()
    {
        m_SliderMin = 0.0f;
        m_SliderMax = 1.0f;

        m_AccessPass = "";
        m_Extension = "";
    }

};
