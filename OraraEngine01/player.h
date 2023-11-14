#pragma once
#include <list>
#include "gameObject.h"
#include "bullet.h"
#include <string>

enum PLAYER_STATE
{
	PLAYER_STATE_GROUND,
	PLAYER_STATE_JUMP,
};

class Player : public GameObject
{
private:
	PLAYER_STATE m_PlayerState = PLAYER_STATE_GROUND;
	bool m_isGround{};

	//Model* m_Model{};
	class AnimationModel* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};

	class Audio* m_ShotSE{};
	class Shadow* m_Shadow{};

	D3DMATRIX m_Matrix;

	int      m_Count{};
	int      m_Time{};
	float    m_BlendRate{};

	std::string m_AnimationName{};
	std::string m_NextAnimationName{};

	void SetAnimation(std::string animName);

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void UpdateGround();
	void UpdateJump();

	D3DMATRIX GetMatrix() { return m_Matrix; }
}; 

