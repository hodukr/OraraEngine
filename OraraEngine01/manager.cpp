#include "main.h"
#include "input.h" 
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "audio.h"
#include "guiManager.h"
#include "shaderManager.h"
#include "collisionManager.h"

Scene* Manager::m_Scene{};//静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};
CollisionManager* m__CollisionManager{};




void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
    GuiManager::Instance().SetUp();
	SetScene<Scene>();

	m__CollisionManager = new CollisionManager;

	ShaderManager::Instance().Init();

	//m_Scene = new Title();
	//m_Scene->Init();

}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	m__CollisionManager->Uninit();
	delete m__CollisionManager;

	ShaderManager::Instance().Uninit();

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
	m__CollisionManager->Update();
	ShaderManager::Instance().Update();
}

void Manager::Draw()
{
	ShaderManager::Instance().Draw();

	GuiManager::Instance().Draw();

	Renderer::End();
}
