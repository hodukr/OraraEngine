#include "main.h"
#include "rendererManager.h"
#include "engineManager.h"
#include "uiManager.h"


void EngineManager::Init()
{
    //Renderer::Init();
    RendererManager::Instance().Init();
    UIManager::Instance().Init();
    //Input::Init();

    //m_Scene = new Title();
    //m_Scene->Init();

    //SetScene<Title>();
}

void EngineManager::Uninit()
{
    RendererManager::Instance().Uninit();

    /*m_Scene->Uninit();
      delete m_Scene;

      Renderer::Uninit();
      Input::Uninit();*/
}

void EngineManager::Update()
{
    /*Input::Update();

    m_Scene->Update();*/
}

void EngineManager::Draw()
{

    UIManager::Instance().Draw();
    RendererManager::Instance().End();

    /*Renderer::Begin();

    m_Scene->Draw();

    Renderer::End();*/
}
