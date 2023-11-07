#include "main.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "audio.h"

//静的メンバ変数は再宣言が必要
Scene* Manager::m_NowScene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	SetScene<Game>();
	ChangeScene();

}
void Manager::Uninit()
{
	m_NowScene->Uninit();

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();

}

void Manager::Update()
{
	ChangeScene();
	Input::Update();
	m_NowScene->Update();

}

void Manager::Draw()
{
	Renderer::Begin();
	m_NowScene->Draw();

	Renderer::End();
}
