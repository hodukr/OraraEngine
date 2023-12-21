#include "main.h"
#include "input.h" 
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"
#include "title.h"
#include "audio.h"
#include "guiManager.h"
#include "shaderManager.h"
#include "post.h"
#include "collisionManager.h"

Scene* Manager::m_Scene{};//静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};
CollisionManager* m__CollisionManager{};
Post* m_Post{};
ShaderManager* m_ShaderManager{};


void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
    GuiManager::Instance().SetUp();
	SetScene<Scene>();

	m__CollisionManager = new CollisionManager;

	m_Post = new Post;
	m_Post->Init();

	m_ShaderManager = new ShaderManager;
	m_ShaderManager->Init();

	//m_Scene = new Title();
	//m_Scene->Init();

}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	m__CollisionManager->Uninit();
	delete m__CollisionManager;
	m_Post->Uninit();
	delete m_Post;
	m_ShaderManager->Uninit();
	delete m_ShaderManager;

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
	m_ShaderManager->Update();
	m_Post->Update();
}

void Manager::Draw()
{
	//Renderer::Begin();

	// m_Scene->Draw();

	//m_Scene->Draw();
	m_ShaderManager->Draw();
	Renderer::Begin();
	m_Post->Draw();

	GuiManager::Instance().Draw();

	Renderer::End();
}
