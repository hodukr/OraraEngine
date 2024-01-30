#include "main.h"
#include "input.h"

void Input::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset(m_KeyState, 0, 256);
	m_MouseInput = None;
	m_Mosueflg = 0x00000000;
	m_MosueOldflg = 0x00000000;
}

void Input::Uninit()
{

}

void Input::Update()
{
	m_MosueOldflg = m_Mosueflg;
	m_Mosueflg = 0x00000000;
	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState(m_KeyState);
	GetCursorPos(&m_MousePos);
	ScreenToClient(GetWindow(), &m_MousePos);

	if (GetKeyState(VK_LBUTTON) & 0x80)
	{
		if ((m_MosueOldflg & MOUSE_DOWN_LEFT) == false && (m_MosueOldflg & MOUSE_TRIGGER_LEFT) == false)
		{
			m_Mosueflg |= MOUSE_TRIGGER_LEFT;
		}
		else
			m_Mosueflg |= MOUSE_DOWN_LEFT;
	}
	else
	{
		if (m_MosueOldflg & MOUSE_TRIGGER_LEFT)
		{
			m_Mosueflg |= MOUSE_UP_LEFT;
		}
	}


	if (GetKeyState(VK_RBUTTON) & 0x80)
	{
		//マウスの右ボタンが押された"
		if ((m_MosueOldflg & MOUSE_DOWN_RIGHT) == false && (m_MosueOldflg & MOUSE_TRIGGER_RIGHT) == false)
		{
			m_Mosueflg |= MOUSE_TRIGGER_RIGHT;
		}
		else
			m_Mosueflg |= MOUSE_DOWN_RIGHT;
	}
	else
	{
		if (m_MosueOldflg & MOUSE_TRIGGER_RIGHT)
		{
			m_Mosueflg |= MOUSE_UP_RIGHT;
		}
	}

	if (GetKeyState(VK_MBUTTON) & 0x80)
	{
		//マウスホイールが押された"
		m_Mosueflg |= MOUSE_TRIGGER_CENTER;
		if ((m_MosueOldflg & MOUSE_TRIGGER_CENTER) == false)
		{
			m_Mosueflg |= MOUSE_DOWN_CENTER;
		}
	}
	else
	{
		if (m_MosueOldflg & MOUSE_TRIGGER_CENTER)
		{
			m_Mosueflg |= MOUSE_UP_CENTER;
		}
	}

	if (m_IsMouseHweel)
	{
		m_MouseHweel = m_StopMouseHweel;
		m_Mosueflg |= MOUSE_WHEEL;
	}
	else
	{
		m_MouseHweel = 0;
	}
	m_IsMouseHweel = false;
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetKeyRelease(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode] & 0x80));
}

Vector2 Input::GetMousePos()
{
	Vector2 pos;
	pos.x = static_cast<float>(m_MousePos.x);
	pos.y = static_cast<float>(m_MousePos.y);

	return pos;
}

bool Input::GetMouseInput(BOOL inputDefine)
{
	return m_Mosueflg & inputDefine;
}

BOOL Input::GetMouseInput()
{
	return m_Mosueflg;
}
