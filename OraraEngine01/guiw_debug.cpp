#include "main.h"
#include "renderer.h"
#include "guiw_debug.h"
#include "shaderManager.h"
#include "pass_depthShadow.h"


void Debug::SetWindowConfig()
{
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.762f, SCREEN_HEIGHT * 0.56f));
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH - SCREEN_WIDTH * 0.761f, SCREEN_HEIGHT * 0.45f));
}

void Debug::Draw()
{
    ImGui::Begin("Debug",nullptr,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("%.3f ms/frame(%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    static float value[180];
    for (int i = 0; i < 179; i++)
        value[i] = value[i + 1];

    value[179] = ImGui::GetIO().DeltaTime * 1000.0f;

    ImGui::PlotHistogram("", value, sizeof(value) / sizeof(float), 0, NULL, 0.0f, 100.0f, ImVec2(280.0f, 80.0f));

    DepthShadow* shadow = ShaderManager::Instance().GetPass<DepthShadow>(SHADER_SHADOW);
    ImGui::Image((ImTextureID)*shadow->GetTexture(), ImVec2(200.0f, 200.0f));

    ImGui::End();
}
