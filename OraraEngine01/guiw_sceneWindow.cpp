#include "main.h"
#include "manager.h"
#include "guiw_sceneWindow.h"
#include "scene.h"
#include "renderer.h"
#include "pass_postPass.h"
#include "shaderManager.h"
#include "guiManager.h"
#include "guiw_accessFolder.h"


void SceneWindow::Draw()
{
	ImGui::Begin("Scene");

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 viewportPos = viewport->Pos;
    ImVec2 viewportSize = viewport->Size;
    int windowWidth = static_cast<int>(viewportSize.x);
    int windowHeight = static_cast<int>(viewportSize.y);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(windowWidth);
    io.DisplaySize.y = static_cast<float>(windowHeight);

    ImVec2 WindowSize(windowWidth * 0.5f, windowHeight * 0.5f);
    ImVec2 windowPos((windowWidth / 2.0f) - (windowWidth / 4.0f), 0);

	//レンダリングテクスチャを取得
	PostPass* post = ShaderManager::Instance().GetPass<PostPass>(SHADER_POST);
	//レンダリングテクスチャを0番にセット  
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, post->GetPPTexture());

	ImGui::Image((ImTextureID)*post->GetPPTexture(),WindowSize);

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

	ImGui::End();
}
