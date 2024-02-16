#pragma once
#include <typeinfo>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>

class GuiWindowBase
{
protected:
	bool m_IsShowWindow = false;
public:
	virtual void Init(){}
	virtual void Uninit(){}
	virtual void Update(){}
	virtual void SetWinodwConfig(){}
	virtual void Draw(){}
	void SetShowWindow(bool flg) { m_IsShowWindow = flg; }
	bool GetShowWindow() { return m_IsShowWindow; }

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(m_IsShowWindow));
	}
};