#pragma once
#include "imgui/imgui.h"
#include "imGuiWindow.h"
struct Node {
    int id{};
    ImVec2 Position{};
    ImVec2 PositionCorrection{};
    ImVec2 Size{};
    ImVec4 Color{};
};

struct Connection {
    int StartNodeId{};
    int EndNodeId{};
};

// ノードエディターの状態 
struct NodeEditor {
    std::vector<Node*> Nodes;
    std::vector<Connection> Connections;
    int nextNodeId = 1;
};


class NodeEditorManager:public GuiWindowBase
{
private:
    NodeEditor m_Editor{};
    Node* m_GetNode = nullptr;
    ImVec2 m_MouseDownlen{};
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    void AddNode(NodeEditor& editor, const ImVec2& position, const ImVec2& size, const ImVec4& color);
    void AddConnection(NodeEditor& editor, int startNodeId, int endNodeId);
    void DrawNodeEditor(NodeEditor& editor);
    void DrawBackGround();
};
