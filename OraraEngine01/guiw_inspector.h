﻿#pragma once
#include "singleton.h"
#include "imGuiWindow.h"


class Inspector:public GuiWindowBase
{
private:
    class GameObject* m_GameObject;

    class Component* m_PopupComponent{};
    std::unordered_map<int,bool> m_IsRockVector{};
    int m_NumVector{};
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
private:
    void DrawComponent(class Component* component);
    void DrawMaterial();
    void Drawvariable(struct TypeDate& vardate);
    std::vector<std::string> AccessFolder(const char* folderPass);//フォルダ内のファイル名の取得
    void CreatComponentFile(std::string comname);
    void AddFileToProject(const std::string& project_file, const std::string& file_path, bool is_header);
};
