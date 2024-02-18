#include "main.h"
#include "guiw_debug.h"


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

    // メモリ使用量のグラフ
    static float memoryUsage[180]; // 過去のメモリ使用量の値を保持する配列
    for (int i = 0; i < 179; i++)
        memoryUsage[i] = memoryUsage[i + 1];

    // 現在のメモリ使用量を計算して配列に追加
    // ここでは仮の値としてランダムな値を使用しています
    float currentMemoryUsage = rand() % 100; // 仮のメモリ使用量（0～99）
    memoryUsage[179] = currentMemoryUsage;

    // 過去のメモリ使用量の最大値を取得
    float maxMemoryUsage = *std::max_element(memoryUsage, memoryUsage + 180);

    // メモリ使用量のグラフを描画
    ImGui::PlotHistogram("Memory Usage", memoryUsage, sizeof(memoryUsage) / sizeof(float), 0, NULL, 0.0f, maxMemoryUsage, ImVec2(280.0f, 80.0f));

    ImGui::End();
}
