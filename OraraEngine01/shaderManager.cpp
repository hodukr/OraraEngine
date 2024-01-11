#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "shaderManager.h"
#include "pass_environmentMapping.h"
#include "pass_postPass.h"
#include "post.h"
#include "pass_depthShadow.h"

void ShaderManager::Init()
{
    m_Post = new Post;
    m_Post->Init();

    AddPass<EnvironmentMapping>();
    AddPass<DepthShadow>();
    AddPass<PostPass>();
}

void ShaderManager::Uninit()
{
    m_Post->Uninit();
    delete m_Post;

    for (Pass* pass : m_Pass)
    {
        pass->Uninit();
        delete pass;
    }
    m_Pass.clear();
}

void ShaderManager::Update()
{
    m_Post->Update();

    for (Pass* pass : m_Pass)
    {
        pass->Update();
    }
}

void ShaderManager::Draw()
{
    Scene* scene = Manager::GetScene();
    EnvironmentMapping* envMap = GetPass<EnvironmentMapping>(SHADER_ENVIRONMENTMAPPING);
    DepthShadow* shadow = GetPass<DepthShadow>(SHADER_SHADOW);

    envMap->Draw();

    Renderer::SetDefaultViewport();

    shadow->Draw();

    PostPass* post = GetPass<PostPass>(SHADER_POST);
    post->BeginPP();


    scene->Draw();

    Renderer::Begin();

    m_Post->Draw();
}
