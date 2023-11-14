#include"main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

#include "textureManager.h"
#include "camera.h"
#include "field.h"
#include "meshField.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "cylinder.h"
#include "box.h"
#include "rock.h"
#include "boxCollisionDraw.h"
#include "sphereCollisionDraw.h"
#include "tree.h"
#include "bullet.h"
#include "polygon2D.h"
#include "input.h"
#include "result.h"
#include "score.h"
#include "audio.h"
#include "skyDome.h"
#include "Life.h"
#include "fade.h"
#include "child.h"
#include "MouseHIt.h" 

void Game::Init()
{
	TextureManager::LoadTexture("asset\\texture\\Explosion1.png");

	Bullet::Load();

	AddGameObject<Camera>(0);
	AddGameObject<SkyDome>(1)->SetPosition(D3DXVECTOR3(0.0f,-3.0f,0.0f));
	//AddGameObject<Field>(1);
	MeshField* meshField = AddGameObject<MeshField>(1);

    AddGameObject<BoxCollisionDraw>(1)->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
    AddGameObject<SphereCollisionDraw>(1)->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
	
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-5.0f, 1.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(5.0f, 1.0f, 5.0f));
	//AddGameObject<Explosion>();
	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	cylinder->SetPosition(D3DXVECTOR3(7.0f, 0.0f, 10.0f));
	cylinder->SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));

	cylinder = AddGameObject<Cylinder>(1);

	cylinder->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 10.0f));
	cylinder->SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));

	Box* box = AddGameObject<Box>(1);
	box->SetPosition(D3DXVECTOR3(-7.0f, 0.0f, 10.0f));
	box->SetScale(D3DXVECTOR3(3.0f, 2.0f, 3.0f));

	box = AddGameObject<Box>(1);
	box->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -10.0f));
	box->SetScale(D3DXVECTOR3(3.0f, 2.0f, 3.0f));
	
	for (int i = 0; i < 20; i++)
	{
		Rock* rock = AddGameObject<Rock>(1);

		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = meshField->GetHeight(pos);

		rock->SetPosition(pos);
	}
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(0.0f, 3.0f, -5.0));

	/*for (int i = 0; i < 20; i++)
	{
		Tree* tree = AddGameObject<Tree>(1);

		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = meshField->GetHeight(pos);

		tree->SetPosition(pos);
	}*/

	AddGameObject<MouseHit>(1);

	//AddGameObject<Child>(1)->SetParent(player);

	//AddGameObject<Polygon2D>(2);
	AddGameObject<Score>(2);
	m_Fade = AddGameObject<Fade>(2);
	AddGameObject<Life>(2);

	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\GameBGM.wav");
	m_BGM->Play(true);
}

void Game::Uninit()
{
	Scene::Uninit();

	Bullet::Unload();

}
void Game::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
		m_Fade->FadeOut();

	if(m_Fade->GetFadeFinish())
		Manager::SetScene<Result>();
}
