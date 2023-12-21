#pragma once
#include "singleton.h"
#include <string>

class GuiManager
{
private:
    Singleton(GuiManager);
    static std::string m_Text;
    class NodeEditorManager* m_NodeEditor;

public:
    void SetUp();
    void Init();
    void Uninit();
    void Update();
    void Draw();
    static void SetText(std::string text) { m_Text = text; }
};
