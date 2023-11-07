#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "title.h"
#include "polygon2D.h"
#include "fade.h"

void Title::Init()
{
	AddGameObject<Polygon2D>(2)->Init((char*)"asset\\texture\\title.png");
	AddGameObject<Fade>(2)->Init((char*)"asset\\texture\\title.png");
}

void Title::Update()
{
	Scene::Update();

}
