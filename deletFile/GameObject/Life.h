#pragma once
#include "gameObject.h"

class Life :public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//ヒットポイント関連
	int m_hp;       //現在ヒットポイント
	int m_hpMax;    //最大ヒットポイント
	int m_hpOld;    //前回ヒットポイント

	int m_Count;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};
