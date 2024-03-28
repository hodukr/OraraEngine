#ifndef shakeCamera_H
#define shakeCamera_H

#include "component.h"

class ShakeCamera:public Component
{
private:
    float m_ShakeTime{};
    Vector3 m_Offset{};
    Vector3 m_ShakeVectol{};
    float m_ShakeSpeed{1.5f};
    float m_ShakeAmplitude{1.0f};
    float m_AttenuationRate{0.9f};//å∏êäó¶
    bool m_IsShake = false;
    
    class Camera* camera{};
public:
    void DrawInspector()
    {
        SET_DATE(m_ShakeVectol);
        SET_DATE(m_ShakeSpeed);
        SET_DATE(m_AttenuationRate);
    }
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;

    void SetShake(bool flg) { m_IsShake = flg; }
    template<class Archive>
    void serialize(Archive & archive)
    {
        Component::serialize<Archive>(archive);
        try
        {
            archive(CEREAL_NVP(m_ShakeVectol), CEREAL_NVP(m_ShakeSpeed), CEREAL_NVP(m_AttenuationRate));
        }
        catch (const std::exception&)
        {

        }
    }
};

#endif /* shakeCamera_H */
