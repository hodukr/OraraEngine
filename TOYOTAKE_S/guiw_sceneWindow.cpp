#include "main.h"
#include "manager.h"
#include "guiw_sceneWindow.h"
#include "scene.h"
#include "renderer.h"
#include "pass_postPass.h"
#include "shaderManager.h"
#include "guiManager.h"
#include "guiw_accessFolder.h"
#include "scene.h"
#include "sceneCamera.h"
#include "guiw_hierarchy.h"
#include "pass_createTexture.h"
#include "com_uiTexture.h"
#include <cereal/archives/json.hpp>

void SceneWindow::Update()
{
    if (ImGui::IsKeyPressed(ImGuiKey_1))
        m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_2))
        m_CurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_3)) // r Key
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
        static string str = "Editor";
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
            Manager::SetNextSceneState(SCENESTATE_SCENE);
            str = "Editor";
        }

        ImGui::EndMenuBar();
    }
    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = io.DisplaySize.x;
    float viewManipulateTop = 0;

    ImGuizmo::SetDrawlist();

    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
    viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
    viewManipulateTop = ImGui::GetWindowPos().y;

    //Windowに対してのマウスの状態
    m_IsChildWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
    m_IsMouseHoveringChildWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);

    ImVec2 windowSize(SCREEN_WIDTH * 0.50f, SCREEN_HEIGHT * 0.50f);   
    //ここでシーン画面を出力
    CreateTexture* cTex = ShaderManager::Instance().GetPass<CreateTexture>(SHADER_CREATETEXTURE);
    ImGui::Image((ImTextureID)*cTex->GetTexture(), windowSize);
   
    Scene* scene = Manager::GetScene();
    Hierarchy* hierarchy = GuiManager::Instance().GetGuiWindow<Hierarchy>();

    //ドラック＆ドロップでシーンにオブジェクトを出す
    // ドラッグターゲットを設定
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_PAYLOAD_TYPE"))
        {
            //ドラッグ先の要素に対する処理
            AccessFolder* acFolder = GuiManager::Instance().GetGuiWindow <AccessFolder>();
            fs::path folderPath(acFolder->GetDragName());
            string folderName = folderPath.filename().string();
            // ドットの位置を検索
            size_t dotPosition = folderName.find_last_of(".");
            // ドットの前の部分文字列を取得
            string beforeDot = folderName.substr(0, dotPosition);
            // ドットの後の部分文字列を取得
            string afterDot = folderName.substr(dotPosition + 1);

            GameObject* gameObj = nullptr;
            if (afterDot == "png" || afterDot == "jpg" || afterDot == "dds")
            {
                gameObj = scene->AddGameObject(1);
                Texture* texture = gameObj->AddComponent<Texture>();
                texture->SetTexture(folderName);
                gameObj->GetMaterial()->SetShader("unlitTexture");
                scene->MoveLayer(scene->GetList()[1], scene->GetList()[2], gameObj);
                hierarchy->SetSelectGameObject(gameObj);
            }
            else if (afterDot == "obj")
            {
                gameObj = scene->AddGameObject(1);
                Mesh* mesh = gameObj->AddComponent<Mesh>();
                mesh->SetModel(folderName);
                hierarchy->SetSelectGameObject(gameObj);
            }
            else if (afterDot == "prefab")
            {
                try
                {
                    ifstream inputFile("asset/prefab/" + folderName);
                    cereal::JSONInputArchive archive(inputFile);
                    unique_ptr<GameObject> obj = make_unique<GameObject>();
                    archive(*obj);
                    GameObject* object = scene->SetGameObject(move(obj));
                    object->m_Transform->SetPosition(0.0f,0.0f,0.0f);
                    hierarchy->SetSelectGameObject(object);
                }
                catch (const exception&)
                {

                }
            }

            if (gameObj) 
                gameObj->SetName(beforeDot);
        }
        ImGui::EndDragDropTarget();
    }

    if (Manager::GetSceneState() == SCENESTATE_SCENE)
    {
        //ゲームオブジェクトの移動、回転、スケールの変更など

        //直行投影モードから透視投影に変換
        ImGuizmo::SetOrthographic(false);
        EditorCamera* camera = ShaderManager::Instance().GetSceneCamera();

        //右上の四角(どこを見ているかのUI)
        ImGuizmo::ViewManipulate(camera->GetViewMatrix(), 8.0f, ImVec2(viewManipulateRight - 100.0f, viewManipulateTop), ImVec2(100.0f, 100.0f), 0x10101010);
        if (hierarchy->GetSelectGameObject() != nullptr && hierarchy->GetSelectGameObject()->GetDrawLayer() != GAME_OBJECT_DRAW_LAYER_2D)
        {
            D3DXMATRIX matrix = hierarchy->GetSelectGameObject()->m_Transform->GetMatrix();

            //オブジェクトの操作
            ImGuizmo::Manipulate(camera->GetViewMatrix(), camera->GetProjectionMatrix(), m_CurrentGizmoOperation, ImGuizmo::MODE(ImGuizmo::LOCAL), matrix, NULL);

            //マニピュレートを使っていか
            if (ImGuizmo::IsUsing())
            {
                hierarchy->GetSelectGameObject()->m_Transform->SetMatrix(matrix);
                D3DXVECTOR3 pos, scale,rot;
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
                    D3DXMATRIX rotationMatrix;
                    D3DXVECTOR3 mat0 = D3DXVECTOR3(tempMatrix._11, tempMatrix._12, tempMatrix._13);
                    D3DXVECTOR3 mat1 = D3DXVECTOR3(tempMatrix._21, tempMatrix._22, tempMatrix._23);
                    D3DXVECTOR3 mat2 = D3DXVECTOR3(tempMatrix._31, tempMatrix._32, tempMatrix._33);

                    // スケール成分を除去した正規化された回転行列を作成
                    D3DXVec3Normalize(&mat0, &mat0);
                    D3DXVec3Normalize(&mat1, &mat1);
                    D3DXVec3Normalize(&mat2, &mat2);

                    rotationMatrix._11 = mat0.x; rotationMatrix._12 = mat0.y; rotationMatrix._13 = mat0.z;
                    rotationMatrix._21 = mat1.x; rotationMatrix._22 = mat1.y; rotationMatrix._23 = mat1.z;
                    rotationMatrix._31 = mat2.x; rotationMatrix._32 = mat2.y; rotationMatrix._33 = mat2.z;

                    // 回転をクオータニオンに変換
                    D3DXQUATERNION tempQuaternion;
                    D3DXQuaternionRotationMatrix(&tempQuaternion, &rotationMatrix);

                    // 回転をセット
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