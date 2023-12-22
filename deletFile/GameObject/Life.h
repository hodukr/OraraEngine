#pragma once
#include "gameObject.h"

class Life :public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//�q�b�g�|�C���g�֘A
	int m_hp;       //���݃q�b�g�|�C���g
	int m_hpMax;    //�ő�q�b�g�|�C���g
	int m_hpOld;    //�O��q�b�g�|�C���g

	int m_Count;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};
