#include "main.h"
#include "manager.h"
#include "title.h"
#include "result.h"
#include "polygon2D.h"
#include "input.h"

void Result::Init()
{
	AddGameObject<Polygon2D>(2)->Init((char*)"asset\\texture\\result.png");
}

void Result::Update()
{
	if (Input::GetKeyTrigger('N'))
	{
		Manager::SetScene<Title>();
	}
}
