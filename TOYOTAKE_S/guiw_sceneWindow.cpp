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

	//�����_�����O�e�N�X�`�����擾
	PostPass* post = ShaderManager::Instance().GetPass<PostPass>(SHADER_POST);
	//�����_�����O�e�N�X�`����0�ԂɃZ�b�g  
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, post->GetPPTexture());

	ImGui::Image((ImTextureID)*post->GetPPTexture(),WindowSize);

    Scene* scene = Manager::GetScene();

    // �h���b�O�^�[�Q�b�g��ݒ�
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_PAYLOAD_TYPE"))
        {
            //const char* draggedItemName = *(const char**)payload->Data;

            //�h���b�O��̗v�f�ɑ΂��鏈��
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
