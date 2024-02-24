#include"main.h"
#include "gameObject.h"
#include "shaderManager.h"
#include "com_post.h"
#include "post.h"

void ComponentPost::Update()
{
	Post* post = ShaderManager::Instance().GetPost();

	post->SetIsPost(m_IsPost);
	
	post->SetPostShader(m_GameObject->GetMaterial()->GetShaderNum());
}

void ComponentPost::EditorUpdate()
{
	Post* post = ShaderManager::Instance().GetPost();

	post->SetIsPost(m_IsPost);

	post->SetPostShader(m_GameObject->GetMaterial()->GetShaderNum());
	
}


