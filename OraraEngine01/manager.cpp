﻿#include "main.h"
#include "input.h" 
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "com_audio.h"
#include "guiManager.h"
#include "shaderManager.h"
#include "collisionManager.h"
#include <cereal/archives/json.hpp>
#include <fstream>

Scene* Manager::m_Scene{};//静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};
SceneState Manager::m_SceneState{ SCENESTATE_NONE };
SceneState Manager::m_NextSceneState{ SCENESTATE_NONE };
CollisionManager* m_CollisionManager{};
GameState Manager::m_GameState{GAMESTATE_NONE};
GameState Manager::m_NextGameState{ GAMESTATE_NONE };


void Manager::Init()
{
	Renderer::Init();
	Input::Instance().Init();
	Audio::InitMaster();
    GuiManager::Instance().SetUp();


	// フォルダのパスを指定 
	std::string folderPath = "asset\\scene";

	//シーン生成
	{//フォルダ内にNewScene.jsonがあれば読み込む
		bool isNewScene = false;
		try {
			// 指定されたフォルダ内のファイルをイテレート 
			for (const auto& entry : fs::directory_iterator(folderPath)) {
				// ファイル名をベクターに追加 
				if (entry.path().filename().string() == "NewScene.json")//フォルダ内にNewScene.jsonがあれば読み込む
				{
					isNewScene = true;
					break;
				}
			}
		}
		catch (const std::filesystem::filesystem_error& ex) {
			std::cerr << "Error: " << ex.what() << std::endl;
		}
		if (isNewScene)
		{
			std::string filename = "asset/scene/NewScene.json";
			std::ifstream inputFile(filename);
			cereal::JSONInputArchive archive(inputFile);
			Scene* inscene = new Scene();
			archive(*inscene);
			SetScene(inscene);
		}
		else
		{
			SetScene<Scene>();
		}
	}
	m_CollisionManager = new CollisionManager;

	ShaderManager::Instance().Init();

	//m_Scene = new Title();
	//m_Scene->Init();
	m_NextSceneState = SCENESTATE_SCENE;
	m_NextGameState = GAMESTATE_STOP;
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
				std::string filename = "asset/scene/" + m_Scene->GetName() + ".json";
				std::ofstream outputFile(filename);
				cereal::JSONOutputArchive o_archive(outputFile);

				o_archive(cereal::make_nvp("secne", *m_Scene));
			}

			{
				std::string filename = "asset/scene/" + m_Scene->GetName() + ".json";
				std::ifstream inputFile(filename);
				cereal::JSONInputArchive archive(inputFile);
				Scene* inscene = new Scene();
				archive(*inscene);
				SetScene(inscene);
			}
		}
		
	}

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
	if (m_GameState == GAMESTATE_PLAY)
	{
		m_Scene->Update();
	}
		m_CollisionManager->Update();
	m_Scene->Destroy();

	ShaderManager::Instance().Update();
}

void Manager::Draw()
{
	ShaderManager::Instance().Draw();

	GuiManager::Instance().Draw();

	Renderer::End();
}
