#include "main.h"
#include "manager.h"
#include "scene.h"
#include "com_changeScene.h"
#include "input.h"
#include "post.h"
#include "shaderManager.h"

void ChangeScene::Init()
{
	if (m_SceneName != "")
		SetScene(m_SceneName);
	m_isFadeIn = true;
	m_StartFadeIn = true;

}

void ChangeScene::Uninit()
{
}

void ChangeScene::EditorUpdate()
{
}

void ChangeScene::Update()
{
	Post* post = ShaderManager::Instance().GetPost();
	if (Input::Instance().GetKeyTrigger(VK_RETURN))
	{
		post->SetIsWipe(true);
		post->SetThreshold(0.0f);
		post->SetWipeSpeed(0.01f);
		m_isFadeOut = true;
	}

	if (m_isFadeOut && !post->GetIsWipe())
	{
		m_isFadeOut = false;
		SceneChange(m_SceneName);
	}

	if (m_StartFadeIn)
	{
		post->SetIsWipe(true);
		post->SetThreshold(1.0f);
		m_StartFadeIn = false;
	}

	if (m_isFadeIn)
	{
		post->SetWipeSpeed(-0.01f);

		if (!post->GetIsWipe())
			m_isFadeIn = false;
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
