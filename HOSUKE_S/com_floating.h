#ifndef floating_H
#define floating_H

#include "component.h"

class Floating:public Component
{
private:
    class WaterSurface* m_WaterSurface{};
    float m_Velocity = 0.1f;
public:
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;
    void Draw()override;

    template<class Archive>
    void serialize(Archive & archive)
    {
        //archive(CEREAL_NVP());
    }
};

#endif /* floating_H */
