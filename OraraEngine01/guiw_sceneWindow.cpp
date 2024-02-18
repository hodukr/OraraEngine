#include "main.h"
#include "manager.h"
#include "guiw_sceneWindow.h"
#include "scene.h"
#include "renderer.h"
#include "pass_postPass.h"
#include "shaderManager.h"
#include "guiManager.h"
#include "guiw_accessFolder.h"
#include <filesystem>
#include "scene.h"
#include "sceneCamera.h"
#include "guiw_hierarchy.h"

void SceneWindow::Update()
{
    if (ImGui::IsKeyPressed(ImGuiKey_T))
        m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_E))
        m_CurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
        m_CurrentGizmoOperation = ImGuizmo::SCALE;
}

void SceneWindow::SetWindowConfig()
{
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH * 0.512f, SCREEN_HEIGHT * 0.56f));
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.25f, 0.0f));
}

void SceneWindow::Draw()
{
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

    if (ImGui::BeginMenuBar())
    {
        static std::string str = "Editor";
        const char* menuName = str.c_str();
        if (ImGui::BeginMenu(menuName))
        {
            if (ImGui::MenuItem("Editor"))
            {
                str = "Editor";
                if (Manager::GetSceneState() != SCENESTATE_SCENE) Manager::SetNextSceneState(SCENESTATE_SCENE);
            }
            if (ImGui::MenuItem("Game"))
            {
                str = "Game";
                if (Manager::GetSceneState() != SCENESTATE_GAME) Manager::SetNextSceneState(SCENESTATE_GAME);
            }
           
            ImGui::EndMenu();
        }
        static bool isPlay = false;
        bool newplay = isPlay;
        if (ImGui::MenuItem("Play",0,&isPlay))
        {
            if (!newplay)
            {
                Manager::SetNextGameState(GAMESTATE_PLAY);
                if (Manager::GetSceneState() != SCENESTATE_GAME) Manager::SetNextSceneState(SCENESTATE_GAME);
                str = "Game";
            }
            else
            {
                isPlay = false;
                Manager::SetNextGameState(GAMESTATE_STOP);
            }
            
        }

        if (ImGui::MenuItem("Stop"))
        {
            isPlay = false;
            Manager::SetNextGameState(GAMESTATE_STOP);
        }
        /*ImGui::Button("Play");
        ImGui::Button("Stop");*/

        ImGui::EndMenuBar();
    }
    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = io.DisplaySize.x;
    float viewManipulateTop = 0;

    ImGuizmo::SetDrawlist();
    ////グリッド線の範囲
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
    viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
    viewManipulateTop = ImGui::GetWindowPos().y;


    //Windowに対してのマウスの状態
    m_IsChildWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
    m_IsMouseHoveringChildWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);



    ImVec2 windowSize(SCREEN_WIDTH * 0.50f, SCREEN_HEIGHT * 0.50f);
    if (m_SceneTexture == nullptr)
    {
        //レンダリングテクスチャを取得
        PostPass* post = ShaderManager::Instance().GetPass<PostPass>(SHADER_POST);
        ImGui::Image((ImTextureID)*post->GetPPTexture(), windowSize);
    }
    else
    {
        ImGui::Image((ImTextureID)*m_SceneTexture, windowSize);
    }

    Scene* scene = Manager::GetScene();
    Hierarchy* hierarchy = GuiManager::Instance().GetGuiWindow<Hierarchy>();

    //ドラック＆ドロップでシーンにオブジェクトを出す
    // ドラッグターゲットを設定
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_PAYLOAD_TYPE"))
        {
            //ドラッグ先の要素に対する処理
            GameObject* gameObj = scene->AddGameObject(2);
            AccessFolder* acFolder = GuiManager::Instance().GetGuiWindow <AccessFolder>();
            gameObj->SetName(acFolder->GetDragName());
            Mesh* mesh = gameObj->AddComponent<Mesh>();
            fs::path folderPath(acFolder->GetDragName());
            std::string folderName = folderPath.filename().string();
            mesh->SetModel(folderName);
            hierarchy->SetSelectGameObject(gameObj);
        }
        ImGui::EndDragDropTarget();
    }

    if (Manager::GetSceneState() == SCENESTATE_SCENE)
    {
        //ゲームオブジェクトの移動、回転、スケールの変更など

            //直行投影モードから透視投影に変換
        ImGuizmo::SetOrthographic(false);
        EditorCamera* camera = ShaderManager::Instance().GetSceneCamera();
        //グリッド線表示
        //ImGuizmo::DrawGrid(camera->GetViewMatrix(), camera->GetProjectionMatrix(), m_IdentityMatrix, 100.0f);
        //右上の四角(どこを見ているかのUI)
        ImGuizmo::ViewManipulate(camera->GetViewMatrix(), 8.0f, ImVec2(viewManipulateRight - 100.0f, viewManipulateTop), ImVec2(100.0f, 100.0f), 0x10101010);
        if (hierarchy->GetSelectGameObject() != nullptr)
        {
            D3DXMATRIX matrix = hierarchy->GetSelectGameObject()->m_Transform->GetMatrix();

            //オブジェクトの操作
            ImGuizmo::Manipulate(camera->GetViewMatrix(), camera->GetProjectionMatrix(), m_CurrentGizmoOperation, ImGuizmo::MODE(ImGuizmo::LOCAL), matrix, NULL);

            //マニピュレートを使っていか
            if (ImGuizmo::IsUsing())
            {
                hierarchy->GetSelectGameObject()->m_Transform->SetMatrix(matrix);
                D3DXVECTOR3 pos, scale, rot;
                //D3DXQUATERNION rot = D3DXQUATERNION(0.0f, 0.0, 0.0f, 1.0f);
                D3DXMATRIX tempMatrix = hierarchy->GetSelectGameObject()->m_Transform->GetMatrix();

                //マトリクスからposition,rotation,scaleを抽出
                ImGuizmo::DecomposeMatrixToComponents(tempMatrix, pos, rot, scale);

                //ローテーションだけ直接抽出数値をバグるので個別処理
                switch (m_CurrentGizmoOperation)
                {
                case ImGuizmo::TRANSLATE:
                    hierarchy->GetSelectGameObject()->m_Transform->SetPosition(pos);
                    break;
                case ImGuizmo::ROTATE:
                {
                    //マトリクスからクオータニオンを抽出してセット
                    D3DXQUATERNION tempQuaternion;
                    D3DXQuaternionRotationMatrix(&tempQuaternion, &tempMatrix);
                    hierarchy->GetSelectGameObject()->m_Transform->SetQuaternion(tempQuaternion);
                }
                break;
                case ImGuizmo::SCALE:
                    hierarchy->GetSelectGameObject()->m_Transform->SetScale(scale);
                    break;
                }
            }
        }
    }
    ImGui::End();
}