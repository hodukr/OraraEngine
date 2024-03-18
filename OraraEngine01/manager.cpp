#include "main.h"
#include "input.h" 
#include "manager.h"
#include "renderer.h"
#include "loading.h"
#include "shaderManager.h"

#ifdef _DEBUG
#include "guiManager.h"
#endif // DEBUG

#include "collisionManager.h"
#include <cereal/archives/json.hpp>

Scene* Manager::m_Scene{};//静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};
Scene* Manager::m_LoadScene{};
SceneState Manager::m_SceneState{ SCENESTATE_NONE };
SceneState Manager::m_NextSceneState{ SCENESTATE_NONE };
CollisionManager* Manager::m_CollisionManager{};
GameState Manager::m_GameState{GAMESTATE_NONE};
GameState Manager::m_NextGameState{ GAMESTATE_NONE };

void Manager::Init()
{
	Renderer::Init();
	Input::Instance().Init();
	Audio::InitMaster();
	string filename;
#ifdef _DEBUG
    GuiManager::Instance().SetUp();
	m_NextSceneState = SCENESTATE_SCENE;
	m_NextGameState = GAMESTATE_STOP;
	filename = "resource/DebugScene.json";
#else
	m_NextSceneState = SCENESTATE_GAME;
	m_NextGameState = GAMESTATE_PLAY;
	filename = "resource/StateScene.json";

#endif // DEBUG
	//シーン生成
	if (std::filesystem::is_regular_file(filename))
	{
		ifstream inputFile(filename);
		cereal::JSONInputArchive archive(inputFile);
		string stateScene{};
		archive(stateScene);
		SetScene(stateScene);
	}
	else
	{
		SetScene<Scene>();

	}
	m_CollisionManager = new CollisionManager;



}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	ShaderManager::Instance().Uninit();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Instance().Uninit();
}

void Manager::Update()
{
	Input::Instance().Update();
	if (m_SceneState != m_NextSceneState)
	{
		m_SceneState = m_NextSceneState;
	}
	if (m_GameState != m_NextGameState)
	{
		m_GameState = m_NextGameState;
		if (m_Scene)
		{
			if (m_NextGameState == GAMESTATE_PLAY)
			{
				string filename = "asset/scene/" + m_Scene->GetName() + ".json";
				ofstream outputFile(filename);
				cereal::JSONOutputArchive o_archive(outputFile);

				o_archive(cereal::make_nvp("secne", *m_Scene));
			}

			SetScene(m_Scene->GetName());
		}
		
	}
	if (m_LoadScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
			ShaderManager::Instance().Uninit();
			m_CollisionManager->Uninit();
		}

		m_Scene = m_LoadScene;
		m_Scene->Init();
		m_LoadScene = nullptr;
	}

	if(m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = m_NextScene;
		MTInit();
		m_NextScene = nullptr;
	}
#ifdef _DEBUG
	GuiManager::Instance().Update();
#endif // _DEBUG

	if (m_GameState == GAMESTATE_PLAY)
	{
		m_Scene->Update();
	}
	if (m_GameState == GAMESTATE_STOP)
	{
		m_Scene->EditorUpdate();
	}

    m_CollisionManager->Update();
	m_Scene->Destroy();

	ShaderManager::Instance().Update();
}

void Manager::Draw()
{
	ShaderManager::Instance().Draw();
#ifdef _DEBUG
	GuiManager::Instance().Draw();
#endif // _DEBUG

	Renderer::End();
}

void Manager::MTInit()
{
	m_Scene->Init();
	m_CollisionManager->Init();
#ifdef _DEBUG
	GuiManager::Instance().Init();
#endif // _DEBUG
	ShaderManager::Instance().Init();
}

void Manager::SetLoadScene(string scene)
{
	string filename = "asset/scene/" + scene + ".json";
	ifstream inputFile(filename);
	cereal::JSONInputArchive archive(inputFile);
	Scene* inscene = new Scene();
	archive(*inscene);
	m_LoadScene = new Loading(inscene);
}

void Manager::SetLoaded(Scene* scene)
{
	m_NextScene = scene;
}

void Manager::SetScene(string scene)
{
	try
	{
		string filename = "asset/scene/" + scene + ".json";
		ifstream inputFile(filename);
		cereal::JSONInputArchive archive(inputFile);
		Scene* inscene = new Scene();
		archive(*inscene);
		m_NextScene = inscene;
	}
	catch (const exception&)
	{
		m_NextScene = new Scene;
		m_NextScene->SetName(scene);
	}


}