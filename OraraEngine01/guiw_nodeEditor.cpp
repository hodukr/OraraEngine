#include "main.h"
#include "guiw_nodeEditor.h"

void NodeEditorManager::Init()
{
    AddNode(m_Editor, ImVec2(100, 100), ImVec2(100, 100), ImVec4(0.1f, 0.1f, 0.1f, 0.3f));
    AddNode(m_Editor, ImVec2(300, 300), ImVec2(100, 50), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    AddConnection(m_Editor, 1, 2);
    m_GetNode = nullptr;
}
void NodeEditorManager::Uninit()
{

}

void NodeEditorManager::Update()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

    if (m_GetNode)
    {
        m_GetNode->Position = ImGui::GetMousePos() - m_MouseDownlen;
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            m_GetNode = nullptr;
        }
    }
    for (auto& node : m_Editor.Nodes)
    {
        node->PositionCorrection = node->Position + ImGui::GetWindowPos();

        if (!m_GetNode)
        {
            ImVec2 min = node->PositionCorrection;
            ImVec2 max = node->PositionCorrection + node->Size * 1.0f;
            if (ImGui::IsMouseHoveringRect(min, max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                m_GetNode = node;
                m_MouseDownlen = ImGui::GetMousePos() - node->Position;
                break;
            }
        }
    }
    ImGui::PopStyleColor();

}

void NodeEditorManager::Draw()
{
    ImGui::Begin("NodeEditor",&m_IsShowWindow);
    DrawNodeEditor(m_Editor);
    ImGui::End();
}

void NodeEditorManager::AddNode(NodeEditor& editor, const ImVec2& position, const ImVec2& size, const ImVec4& color)
{
    Node* node = new Node;
    node->id = editor.nextNodeId++;
    node->Position = position;
    node->Size = size;
    node->Color = color;
    editor.Nodes.push_back(node);
}

void NodeEditorManager::AddConnection(NodeEditor& editor, int startNodeId, int endNodeId)
{
    Connection connection;
    connection.StartNodeId = startNodeId;
    connection.EndNodeId = endNodeId;
    editor.Connections.push_back(connection);
}

void NodeEditorManager::DrawNodeEditor(NodeEditor& editor)
{

    ImGui::Text("mousepos %f %f", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

    //背景を描画 
    DrawBackGround();


    // ノードを描画 
    for (const auto& node : editor.Nodes) {
        ImGui::SetCursorScreenPos(node->PositionCorrection);
        ImGui::PushID(node->id);
        ImGui::BeginGroup();
        ImGui::Text("Node %f %f", node->PositionCorrection.x, node->PositionCorrection.y);
        ImGui::ColorButton("Color", node->Color, ImGuiColorEditFlags_NoBorder, node->Size);

        ImGui::EndGroup();
        ImGui::PopID();
    }

    // 接続を描画 
    for (const auto& connection : editor.Connections) {
        Node& startNode = *editor.Nodes[connection.StartNodeId - 1];
        Node& endNode = *editor.Nodes[connection.EndNodeId - 1];
        ImVec2 start = startNode.PositionCorrection + startNode.Size * 0.5f;
        ImVec2 end = endNode.PositionCorrection + endNode.Size * 0.5f;
        ImGui::GetWindowDrawList()->AddLine(start, end, IM_COL32(255, 255, 255, 255), 2.0f);
    }

}

void NodeEditorManager::DrawBackGround()
{
    ImVec2 windowpos = ImGui::GetWindowPos();
    ImVec2 windowsize = ImGui::GetWindowSize();
    float linewid = 50;
    //横線  
    for (float i = 0.0f; i < 10.0f; i++)
    {
        ImVec2 State = windowpos;
        State.x -= 50.0f;
        State.y += i * linewid;
        ImVec2 end = windowpos;
        end.x += windowsize.x;
        end.y += i * linewid;
        ImGui::GetWindowDrawList()->AddLine(State,
            end,
            IM_COL32(0, 0, 0, 255), 1.0f);
    }
    //縦線  
    for (float i = 0.0f; i < 20.0f; i++)
    {
        ImVec2 State = windowpos;
        State.x += i * linewid;
        State.y -= 50.0f;
        ImVec2 end = windowpos;
        end.x += i * linewid;
        end.y += windowsize.x;
        ImGui::GetWindowDrawList()->AddLine(State,
            end,
            IM_COL32(0, 0, 0, 255), 1.0f);
    }


}

