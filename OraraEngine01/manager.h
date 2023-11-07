#pragma once
//�z�Q�ƂɂȂ�̂őO���錾
//class Scene;
//�O���錾�̓|�C���^�Ŏg�����̂�
//�p���ɂ͎g�p�s��

#include "scene.h"

class Manager
{
private:
	//Maneger�̓C���X�^���X���Ȃ��̂ŐÓI�ɍ��
	static Scene* m_Scene;
	static Scene* m_NextScene;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene(void) { return m_Scene; }

	template<typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}
	
};