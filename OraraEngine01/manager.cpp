#include "main.h"
#include "input.h" 
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"
#include "title.h"
#include "audio.h"
#include "guiManager.h"
#include "collisionManager.h"

Scene* Manager::m_Scene{};//静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};
CollisionManager* g_CollisionManager{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	GuiManager::Instance().Init();

    g_CollisionManager = new CollisionManager;
    
	//m_Scene = new Title();
	//m_Scene->Init();

	SetScene<Game>();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;
	GuiManager::Instance().Uninit();

    g_CollisionManager->Uninit();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update()
{
	Input::Update();
	GuiManager::Instance().Update();

	if(m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}

	m_Scene->Update();
    g_CollisionManager->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	GuiManager::Instance().Draw();


	Renderer::End();
}
