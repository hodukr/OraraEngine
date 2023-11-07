#pragma once
#include <string>

class Component
{
protected:
	std::string m_name = "none";
public:
	void SetName(std::string name) { m_name = name; }
	std::string GetName(void) {return m_name; }

	virtual void Init() {}//âºëzä÷êî
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
};