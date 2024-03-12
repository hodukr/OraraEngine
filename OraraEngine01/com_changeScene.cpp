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
	m_IsFadeIn = true;
	m_StartFadeIn = true;

}

void ChangeScene::Update()
{
	Post* post = ShaderManager::Instance().GetPost();
	if (m_UseButton)
	{
		if (Input::Instance().GetKeyTrigger(VK_RETURN))
		{
			post->SetIsWipe(true);
			post->SetThreshold(0.0f);
			post->SetWipeSpeed(0.01f);
			m_IsFadeOut = true;
		}
	}

	if (m_IsFadeOut && !post->GetIsWipe())
	{
		m_IsFadeOut = false;
		SceneChange(m_SceneName);
	}

	if (m_StartFadeIn)
	{
		post->SetIsWipe(true);
		post->SetThreshold(1.0f);
		m_StartFadeIn = false;
	}

	if (m_IsFadeIn)
	{
		post->SetWipeSpeed(-0.01f);

		if (!post->GetIsWipe())
			m_IsFadeIn = false;
	}
}

void ChangeScene::SceneChange(std::string name)
{
	// ドットの前の部分文字列を取得
	std::string beforeDot = name.substr(0, name.find_last_of("."));
	Manager::SetScene(beforeDot);
}
