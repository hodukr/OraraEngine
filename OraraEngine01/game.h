#pragma once
#include "scene.h"


class Game:public Scene
{
private:
	class Audio* m_BGM{};
	class Fade*  m_Fade{};

public:
	void Init()override;
	void Update()override;
	void Uninit()override;
};