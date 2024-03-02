#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "sceneCamera.h"
#include "com_transform.h"
#include "input.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include "guiManager.h"
#include "guiw_sceneWindow.h"


void EditorCamera::Init()
{
    m_Position = Vector3(0.0f, 5.0f, -10.0f);

}

void EditorCamera::Uninit()
{

}

void EditorCamera::Update()
{
    //�}�E�X���E�N���b�N����Ă�����
    if (Input::Instance().GetMouseInput(MOUSE_DOWN_RIGHT))
    {

        Vector3 vel = Vector3(0.0f, 0.0f, 0.0f);
        if (Input::Instance().GetKeyPress('W'))
        {
            vel += GetForward();
        }
        if (Input::Instance().GetKeyPress('S'))
        {
            vel -= GetForward();
        }
        if (Input::Instance().GetKeyPress('D'))
        {
            vel += GetRight();
        }
        if (Input::Instance().GetKeyPress('A'))
        {
            vel -= GetRight();
        }
        if (Input::Instance().GetKeyPress('Q'))
        {
            vel += GetUp();
        }
        if (Input::Instance().GetKeyPress('E'))
        {
            vel -= GetUp();
        }
        vel.NormalizThis();
        m_Position += vel;
    }
    SceneWindow* window = GuiManager::Instance().GetGuiWindow<SceneWindow>();
    if (!ImGuizmo::IsUsing() && window->GetChildWindowFocused() && window->GetMouseHoveringChildWindow())
    {
        //���N���b�N�������ꂽ�u�Ԃ̃|�W�V�����ƃJ�����̉�]�p�x���擾
        if (Input::Instance().GetMouseInput(MOUSE_TRIGGER_LEFT))
        {
            m_LeftClickMousePos = Input::Instance().GetMousePos();
            m_LeftClickCameraRot = m_Rotation;
        }

        //���N���b�N����Ă���ԃ}�E�X�̈ړ��������J��������]������
        if (Input::Instance().GetMouseInput(MOUSE_DOWN_LEFT))
        {
            Vector2 vec{};
            vec = Input::Instance().GetMousePos() - m_LeftClickMousePos;
            m_Rotation = Vector3(m_LeftClickCameraRot + (Vector3().Right() * ((vec.y / SCREEN_HEIGHT) * D3DX_PI / 2.0f) +
                Vector3().Up() * ((vec.x / SCREEN_WIDTH * 1.0f) * D3DX_PI / 2.0f)));
        }
    }
    else
    {
        m_LeftClickMousePos = Input::Instance().GetMousePos();
        m_LeftClickCameraRot = m_Rotation;
    }

    m_Target = GetForward() + m_Position.dx();
}

void EditorCamera::Draw()
{
    //�r���[�}�g���N�X�ݒ� 
    D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 position = m_Position.dx();
    D3DXVECTOR3 target = m_Target.dx();

    D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

    Renderer::SetViewMatrix(&m_ViewMatrix);


    //�v���W�F�N�V�����}�g���N�X�ݒ� 
    D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    Renderer::SetProjectionMatrix(&m_ProjectionMatrix);

    Renderer::SetCameraPosition(position);
}