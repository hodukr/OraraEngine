#include"main.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "titleLogo.h"
#include "input.h"
#include "game.h"
#include "fade.h"

void Title::Init()
{
	AddGameObject<TitleLogo>(2);
	m_Fade = AddGameObject<Fade>(2);
}
void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
		m_Fade->FadeOut();
	
	if (m_Fade->GetFadeFinish())
		Manager::SetScene<Game>();
}