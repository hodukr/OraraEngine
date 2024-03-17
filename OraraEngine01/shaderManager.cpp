#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "shaderManager.h"
#include "pass_environmentMapping.h"
#include "pass_postPass.h"
#include "post.h"
#include "pass_depthShadow.h"
#include "pass_createTexture.h"
#include "sceneCamera.h"
#include "shader.h"
void ShaderManager::Init()
{
    m_Post = new Post;
    m_Post->Init();

#ifdef _DEBUG
    m_EditorCamera = new EditorCamera;
    m_EditorCamera->Init();
#endif // DEzBUG

    
    AddPass<EnvironmentMapping>();
    AddPass<DepthShadow>();
    AddPass<PostPass>();
    AddPass<CreateTexture>();
}

void ShaderManager::Uninit()
{
    if (m_EditorCamera)
        m_EditorCamera->Uninit();
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
    if(m_EditorCamera)
    if(Manager::GetSceneState() == SCENESTATE_SCENE)m_EditorCamera->Update();
    
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

    if (m_EditorCamera)
    m_EditorCamera->Draw();
    scene->Draw();

#ifdef _DEBUG
    CreateTexture* cTex = GetPass<CreateTexture>(SHADER_CREATETEXTURE);
    cTex->BeginCT();
#else
    Renderer::Begin();
#endif // _DEBUG
    m_Post->Draw();
#ifdef _DEBUG
    Renderer::Begin();
#endif // _DEBUG
}

int ShaderManager::LoadShader(std::string file)
{
    int ind = 0;
    for (auto s : shaders)
    {
        if (s->GetFile() == file)
        {
            return ind;
        }
        ind++;
    }
    ShaderDate* date = new ShaderDate(file);
    shaders.emplace_back(date);
    return shaders.size() - 1;
}
