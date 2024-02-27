#include "main.h"
#include "manager.h"
#include "scene.h"
#include "com_changeScene.h"
#include "input.h"

void ChangeScene::Init()
{
	if (m_SceneName != "")
		SetScene(m_SceneName);
}

void ChangeScene::Uninit()
{
}

void ChangeScene::EditorUpdate()
{
}

void ChangeScene::Update()
{

	if (Input::Instance().GetKeyTrigger(VK_RETURN))
	{
		SceneChange(m_SceneName);
	}
}

void ChangeScene::Draw()
{
}

void ChangeScene::SceneChange(std::string name)
{
	// ドットの前の部分文字列を取得
	std::string beforeDot = m_SceneName.substr(0, m_SceneName.find_last_of("."));
	Manager::SetScene(beforeDot);
}
