#pragma once
#include <list>
#include <any>
class GameManager
{
private:
	static 
	std::list<std::any> aa;
public:
	void Init();
	void Uninit();
	void Update();
};