#pragma once
#include <typeinfo>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>


#define SET_IMGUIWINDOW_CLASS(Classname)\
CEREAL_REGISTER_TYPE(Classname)\
CEREAL_REGISTER_POLYMORPHIC_RELATION(GuiWindowBase, Classname)

class GuiWindowBase
{
protected:
	bool m_IsShowWindow = true;
public:
	virtual void Init(){}
	virtual void Uninit(){}
	virtual void Update(){}
	virtual void SetWindowConfig(){}
	virtual void Draw(){}
	void SetShowWindow(bool flg) { m_IsShowWindow = flg; }
	bool GetShowWindow() { return m_IsShowWindow; }

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(m_IsShowWindow));
	}
};

CEREAL_REGISTER_TYPE(GuiWindowBase)