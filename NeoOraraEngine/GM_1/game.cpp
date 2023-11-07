#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "result.h"

#include "camera.h"
#include "polygon2D.h"
#include "score.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "cylinder.h"
#include "box.h"
#include "sky.h"
#include "audio.h"
#include "input.h"
#include "fade.h"
#include "child.h"
#include "meshField.h"

void Game::Init()
{
	AddGameObject<Camera>(0);
	AddGameObject<MeshField>(1);



	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(3.0f, 0.5f, 2.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-2.0f, 0.5f, 3.0f));
	Box* box = AddGameObject<Box>(1);
	box->SetPosition(D3DXVECTOR3(-5.0f, 0.0f, 1.0f));
	box->SetScale(D3DXVECTOR3(2.0f, 1.0f, 1.0f));
	AddGameObject<Sky>(1)->SetScale(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	Bullet::Load();
	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	cylinder->SetPosition(D3DXVECTOR3(5.0f, 0.0f, 1.0f));
	cylinder->SetScale(D3DXVECTOR3(2.0f, 1.0f, 1.0f));
	AddGameObject<Polygon2D>(2)->Init((char*)"asset\\texture\\grass.jpg");
	AddGameObject<Score>(2);
	AddGameObject<Fade>(2)->Init((char*)"asset\\texture\\grass.jpg");
	Player* player = AddGameObject<Player>(1);

	m_BGM = AddGameObject<GameObject>(1)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\bgm.wav");
	//m_BGM->Play(true);
}

void Game::Uninit()
{
	Bullet::Unroad();
	Scene::Uninit();
}


void Game::Update()
{
	Scene::Update();
	if (Input::GetKeyTrigger('N'))
	{
		Manager::SetScene<Result>();
	}
}
