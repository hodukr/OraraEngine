#include "main.h"
#include "input.h" 
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"
#include "title.h"
#include "audio.h"
#include "guiManager.h"

Scene* Manager::m_Scene{};//静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
    GuiManager::Instance().SetUp();
	SetScene<Scene>();
	//m_Scene = new Title();
	//m_Scene->Init();

}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if(m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = m_NextScene;
		m_Scene->Init();
        GuiManager::Instance().Init();

		m_NextScene = nullptr;
	}
    GuiManager::Instance().Update();

	m_Scene->Update();	
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	GuiManager::Instance().Draw();


	Renderer::End();
}
