#include"main.h"
#include "gameObject.h"
#include "shaderManager.h"
#include "com_post.h"
#include "post.h"

void ComponentPost::Update()
{
	Post* post = ShaderManager::Instance().GetPost();

	post->SetIsPost(m_IsPost);

	post->SetDebug(m_Debug);
	
	post->SetPostShader(m_GameObject->GetMaterial()->GetShaderNum());

	post->SetTextureNum(m_GameObject->GetMaterial()->GetTextureNum());
}

void ComponentPost::EditorUpdate()
{
	Post* post = ShaderManager::Instance().GetPost();

	post->SetIsPost(m_IsPost);

	post->SetDebug(m_Debug);

	post->SetPostShader(m_GameObject->GetMaterial()->GetShaderNum());
	
	post->SetTextureNum(m_GameObject->GetMaterial()->GetTextureNum());

}


