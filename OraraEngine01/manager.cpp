#include "main.h"
#include "input.h" 
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "com_audio.h"
#include "guiManager.h"
#include "shaderManager.h"
#include "collisionManager.h"

Scene* Manager::m_Scene{};//静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};
CollisionManager* m_CollisionManager{};




void Manager::Init()
{
	Renderer::Init();
	Input::Instance().Init();
	Audio::InitMaster();
    GuiManager::Instance().SetUp();
	SetScene<Scene>();

	m_CollisionManager = new CollisionManager;

	ShaderManager::Instance().Init();

	//m_Scene = new Title();
	//m_Scene->Init();

}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	//m_CollisionManager->Uninit();
	//delete m_CollisionManager;

	ShaderManager::Instance().Uninit();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Instance().Uninit();
}

void Manager::Update()
{
	Input::Instance().Update();


	if(m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;

			m_CollisionManager->Uninit();
		}

		m_Scene = m_NextScene;
		m_Scene->Init();
		m_CollisionManager->Init();
        GuiManager::Instance().Init();

		m_NextScene = nullptr;
	}
	GuiManager::Instance().Update();
	m_Scene->Update();
	m_CollisionManager->Update();
	ShaderManager::Instance().Update();
}

void Manager::Draw()
{
	ShaderManager::Instance().Draw();

	GuiManager::Instance().Draw();

	Renderer::End();
}
