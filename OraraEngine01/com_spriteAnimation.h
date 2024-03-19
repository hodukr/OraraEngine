#pragma once
#include "component.h"

class SpriteAnimation :public Component
{
private:
	ID3D11Buffer* m_VertexBuffer{};

	Vector2 m_Divide{1.0f,1.0f};
	Vector2 m_Pattern{0.0f,0.0f};
	int m_NumTexture{1};
	int m_AnimCount{0};
	int m_AnimSpeed{10};
	int m_Loop{0};
	bool m_AnimFinish = false;
	int m_Index{};
	int m_Count{};

public:
	SpriteAnimation() { m_DrawLayer = GAME_OBJECT_DRAW_LAYER_3D; }
	void DrawInspector()
	{
		SET_DATE(m_NumTexture);
		SET_DATE(m_AnimSpeed);
		SET_DATE(m_Loop);
		if (SET_NAME_DATE("DivideX", m_Divide.x))
		{
			SetPattern();
		}
		if (SET_NAME_DATE("DivideY", m_Divide.y))
		{
			SetPattern();
		}
	}
	void Init();
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetAnimSpeed(int speed) { m_AnimSpeed = speed;}
	void SetPattern();
	template<class Archive>
	void serialize(Archive& archive)
	{
		try
		{
			archive(CEREAL_NVP(m_Divide),
				CEREAL_NVP(m_AnimSpeed),
				CEREAL_NVP(m_Loop),
				CEREAL_NVP(m_NumTexture));
		}
		catch (const exception&)
		{

		}
	}
};