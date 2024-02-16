#include "main.h"
#include "manager.h"
#include "guiw_sceneWindow.h"
#include "scene.h"
#include "renderer.h"
#include "pass_postPass.h"
#include "shaderManager.h"
#include "guiManager.h"
#include "guiw_accessFolder.h"
#include "imgui/imgui_internal.h"
#include <filesystem>
#include "scene.h"
#include "input.h"
#include "sceneCamera.h"
#include "guiw_hierarchy.h"

float objectMatrix[4][16] = {
  { 1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f },

  { 1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  2.f, 0.f, 0.f, 1.f },

  { 1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  2.f, 0.f, 2.f, 1.f },

  { 1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  0.f, 0.f, 2.f, 1.f }
};

static const float identityMatrix[16] =
{ 1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f };

bool isPerspective = true;
float fov = 27.f;
float viewWidth = 10.f; // for orthographic
float camYAngle = 165.f / 180.f * 3.14159f;
float camXAngle = 32.f / 180.f * 3.14159f;

bool firstFrame = true;
int lastUsing = 0;

void SceneWindow::Update()
{

    if (ImGui::IsKeyPressed(ImGuiKey_T))
        m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_E))
        m_CurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
        m_CurrentGizmoOperation = ImGuizmo::SCALE;

    //ImGuiIO& io = ImGui::GetIO();
   
    //ImGui::SetNextWindowPos(ImVec2(1024, 100), ImGuiCond_Appearing);
    //ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_Appearing);

    //// create a window and insert the inspector
    //ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
    //ImGui::SetNextWindowSize(ImVec2(320, 340), ImGuiCond_Appearing);
    //ImGui::Begin("Editor");
    //
    //

    //if (ImGui::RadioButton("Full view", !m_UseWindow)) m_UseWindow = false;
    //ImGui::SameLine();
    //if (ImGui::RadioButton("Window", m_UseWindow)) m_UseWindow = true;

    //ImGui::Text("Camera");
    //bool viewDirty = false;
    //if (ImGui::RadioButton("Perspective", isPerspective)) isPerspective = true;
    //ImGui::SameLine();
    //if (ImGui::RadioButton("Orthographic", !isPerspective)) isPerspective = false;
    //if (isPerspective)
    //{
    //    ImGui::SliderFloat("Fov", &fov, 20.0f, 110.0f);
    //}
    //else
    //{
    //    ImGui::SliderFloat("Ortho width", &viewWidth, 1, 20);
    //}
    //viewDirty |= ImGui::SliderFloat("Distance", &m_CamDistance, 1.f, 10.f);
    //ImGui::SliderInt("Gizmo count", &m_GizmoCount, 1, 4);

    //if (viewDirty || firstFrame)
    //{
    //    float eye[] = { cosf(camYAngle) * cosf(camXAngle) * m_CamDistance, sinf(camXAngle) * m_CamDistance, sinf(camYAngle) * cosf(camXAngle) * m_CamDistance };
    //    float at[] = { 0.f, 0.f, 0.f };
    //    float up[] = { 0.f, 1.f, 0.f };
    //    //LookAt(eye, at, up, cameraView);
    //    firstFrame = false;
    //}

    //ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
    //if (ImGuizmo::IsUsing())
    //{
    //    ImGui::Text("Using gizmo");
    //}
    //else
    //{
    //    ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
    //    ImGui::SameLine();
    //    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
    //    ImGui::SameLine();
    //    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
    //    ImGui::SameLine();
    //    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
    //}
    //ImGui::Separator();

    //ImGui::End();
}

void SceneWindow::Draw()
{
    ImVec2 WindowSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
    ImVec2 WindowSizewindow(SCREEN_WIDTH * 0.512f, SCREEN_HEIGHT * 0.56f);
    ImGui::SetNextWindowSize(WindowSizewindow);
    ImVec2 windowPos((SCREEN_WIDTH / 2.0f) - (SCREEN_WIDTH / 4.0f), 0);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 viewportPos = viewport->Pos;
    ImVec2 viewportSize = viewport->Size;
    int windowWidth = static_cast<int>(viewportSize.x);
    int windowHeight = static_cast<int>(viewportSize.y);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(windowWidth);
    io.DisplaySize.y = static_cast<float>(windowHeight);

    //ImVec2 WindowSize(windowWidth * 0.5f, windowHeight * 0.5f);
    //ImVec2 windowPos((windowWidth / 2.0f) - (windowWidth / 4.0f), 0);

    if (m_SceneTexture == nullptr)
    {
        //レンダリングテクスチャを取得
        PostPass* post = ShaderManager::Instance().GetPass<PostPass>(SHADER_POST);
        ImGui::Image((ImTextureID)*post->GetPPTexture(), WindowSize);
    }
    else
    {
        ImGui::Image((ImTextureID)*m_SceneTexture, WindowSize);
    }

    Scene* scene = Manager::GetScene();

    // ドラッグターゲットを設定
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_PAYLOAD_TYPE"))
        {
            //const char* draggedItemName = *(const char**)payload->Data;

            //ドラッグ先の要素に対する処理
            GameObject* gameObj = scene->AddGameObject(2);
            AccessFolder* acFolder = GuiManager::Instance().GetGuiWindow <AccessFolder>();
            gameObj->SetName(acFolder->GetDragName());
            Mesh* mesh = new Mesh;
            gameObj->AddComponent(mesh);
            fs::path folderPath(acFolder->GetDragName());
            std::string folderName = folderPath.filename().string();
            mesh->SetModel(folderName);

            //ImGui::Text("Dropped item: %s", draggedItemName);
        }


        ImGui::EndDragDropTarget();
    }


    //直行投影モードから透視投影に変換
    ImGuizmo::SetOrthographic(false);

    Hierarchy* hierarchy = GuiManager::Instance().GetGuiWindow<Hierarchy>();
    if (hierarchy->GetSelectGameObject() != nullptr)
    {
        SceneCamera* camera = ShaderManager::Instance().GetSceneCamera();
        D3DXMATRIX matrix = hierarchy->GetSelectGameObject()->m_Transform->GetMatrix();
        //ここでマ二ピュレートなどを適応
        EditTransform(camera->GetViewMatrix(), camera->GetProjectionMatrix(), matrix, true);

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
                //tempQuaternion.w = 1.0f;
                hierarchy->GetSelectGameObject()->m_Transform->SetQuaternion(tempQuaternion);
                D3DXQUATERNION q = hierarchy->GetSelectGameObject()->m_Transform->GetQuaternion();
                ImGui::Text("x:%f y:%f z:%f w:%f", q.x, q.y, q.z, q.w);
            }
            break;
            case ImGuizmo::SCALE:
                hierarchy->GetSelectGameObject()->m_Transform->SetScale(scale);
                break;
            }
        }
    }

	ImGui::End();
}

void SceneWindow::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;

    if (editTransformDecomposition)
    {
        
        if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
            m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
            m_CurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
            m_CurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Universal", m_CurrentGizmoOperation == ImGuizmo::UNIVERSAL))
            m_CurrentGizmoOperation = ImGuizmo::UNIVERSAL;
        /*float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);*/

        if (m_CurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_S))
            useSnap = !useSnap;
        ImGui::Checkbox("##UseSnap", &useSnap);
        ImGui::SameLine();

        switch (m_CurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing)
        {
            ImGui::PushID(3);
            ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = io.DisplaySize.x;
    float viewManipulateTop = 0;
    static ImGuiWindowFlags gizmoWindowFlags = 0;
    ImVec2 windowSize;
    if (m_UseWindow)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.3f, 0.3f, 0.3f));
        ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        windowSize = ImVec2(windowWidth, windowHeight);
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
        viewManipulateTop = ImGui::GetWindowPos().y;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
    }
    else
    {
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    }

    m_IsChildWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
    m_IsMouseHoveringChildWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);

    ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
    //ImGuizmo::DrawCubes(cameraView, cameraProjection, matrix, 1);
     
    
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

    ImGuizmo::Manipulate(cameraView, cameraProjection, m_CurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL /*,useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL*/);

    ImGuizmo::ViewManipulate(cameraView, m_CamDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

    if (m_UseWindow)
    {
        ImGui::End();
        ImGui::PopStyleColor(1);
    }
}