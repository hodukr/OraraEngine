#pragma once
#include "singleton.h"
#include "vector.h"

enum MouseInput
{
	None,
	LButtonDown,
	MButtonDown,
	RButtonDown,
	LButtonUp,
	MButtonUp,
	RButtonUp,
};

#define MOUSE_TRIGGER_LEFT	  0x00000001
#define MOUSE_TRIGGER_RIGHT	  0x00000002
#define MOUSE_TRIGGER_CENTER  0x00000004
#define MOUSE_DOWN_LEFT		  0x00000010
#define MOUSE_DOWN_RIGHT	  0x00000020
#define MOUSE_DOWN_CENTER	  0x00000040
#define MOUSE_UP_LEFT		  0x00000010
#define MOUSE_UP_RIGHT		  0x00000020
#define MOUSE_UP_CENTER		  0x00000040
#define MOUSE_WHEEL			  0x00000100

class Input
{
private:
	Singleton(Input);

	BYTE m_OldKeyState[256];
	BYTE m_KeyState[256];
	POINT m_MousePos;
	POINT m_Mousecenterpos;
	MouseInput m_MouseInput;
	MouseInput m_OldMouseInput;
	BOOL m_Mosueflg;
	BOOL m_MosueOldflg;
	short m_MouseHweel;
	bool m_IsMouseHweel;
	short m_StopMouseHweel;

public:
	void Init();
	void Uninit();
	void Update();

	bool GetKeyPress(BYTE KeyCode);
	bool GetKeyTrigger(BYTE KeyCode);
	bool GetKeyRelease(BYTE KeyCode);
	Vector2 GetMousePos();
	bool GetMouseInput(BOOL inputDefine);//InputのDefineを入れる
	BOOL GetMouseInput();//InputのDefineを入れる
	void SetMouseRot(short mouseHweel)
	{
		m_StopMouseHweel = mouseHweel;
		m_IsMouseHweel = true;
	}

	float GetMouseWheel() { return static_cast<float>(m_MouseHweel);}
};



