#pragma once
#include "singleton.h"
#include "shader.h"
#include <string>
#include <vector>
class ShaderManager
{
private:
    Singleton(ShaderManager);

	class Post* m_Post{};
	class EditorCamera* m_EditorCamera{};
	std::vector<ShaderDate*> shaders;
	std::list<Pass*> m_Pass;
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

	template<typename T>
    T* AddPass()
	{
		Pass* pass = new T();
		m_Pass.push_back(pass);
		pass->CreatePass();
		pass->Init();

		return dynamic_cast<T*>(pass);
	}

	template<typename T>
    T* GetPass(Shader id)
	{
		for (Pass* pass : m_Pass)
		{
			if (id == pass->GetPassId())
			{
				return dynamic_cast<T*>(pass);
			}
		}
		return nullptr;
	}

	EditorCamera* GetSceneCamera() { return m_EditorCamera; }

	int LoadShader(std::string file);
	ShaderDate* GetShader(int index) { return shaders[index]; }
};
