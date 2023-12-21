#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "shaderManager.h"
#include "environmentMapping.h"
#include "postPass.h"

void ShaderManager::Init()
{
}

void ShaderManager::Uninit()
{
}

void ShaderManager::Update()
{
}

void ShaderManager::Draw()
{
    Scene* scene = Manager::GetScene();

    //m_EnvironmentMapping->Draw();

    PostPass* post = Renderer::GetPass<PostPass>(SHADER_POST);
    post->BeginPP();

    Renderer::SetDefaultViewport();

    scene->Draw();
}
