#pragma once
#include "gameObject.h"
#include "model.h"

enum PlayerState {
	NONE,
	I,
	Jump,
	G
};

class Player:public GameObject
{
private:

	//Model* m_Model{};
	class AnimationModel* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
	class Audio* m_ShotSE;
	class Shadow* m_Shadow;
	
	D3DXMATRIX m_Matrix;

	int m_Time{};
	std::string m_AnimationName{};
	std::string m_NextAnimationName{};
	float m_BlendRate{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXMATRIX GetMatrix() { return m_Matrix; }
};