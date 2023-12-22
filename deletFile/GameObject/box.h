#pragma once
#include "model.h"
#include "gameObject.h"


class Box : public GameObject
{
private:
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

