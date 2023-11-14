#pragma once

#include <string>
#include <vector>
#include <exception>
#include <functional>
#include <iostream>

class Assembly
{
public:
    // リフレクション用のインターフェース
    class IReflection
    {
    public:
        std::function<const std::string& ()> GetTypeName;
        std::function<void* ()> CreateInstance;

        // 所有権の移動によりコピーの無駄を無くしておく
        IReflection(
            std::function<const std::string& ()>&& getTypeName,
            std::function<void* ()>&& createInstance) :
            GetTypeName(getTypeName),
            CreateInstance(createInstance)
        {
        }
    };
private:
    // リフレクション用のインターフェースの集合
    static std::vector<Assembly::IReflection> assemblies;
public:
    // クラス名を一覧表示
    static void ShowClassNames()
    {
        for (IReflection& a : assemblies)
        {
            std::cout << a.GetTypeName() << std::endl;
        }
    }

    // ファクトリ(インスタンス生成)用のメソッド
    static void* CreateInstance(const std::string& typeName)
    {
        for (IReflection& a : assemblies)
        {
            if (a.GetTypeName() == typeName)
            {
                return a.CreateInstance();
            }
        }
        throw std::exception(("Assembly::CreateInstance NotFound typeName=" + typeName).c_str());
    }

    // リフレクションを楽に実装するためのラッパークラス
    template< class T > class Reflection
    {
    private:
        const std::string typeName = typeid(T).name();
    public:
        Reflection()
        {
            Assembly::assemblies.emplace_back(std::bind(&Reflection<T>::GetTypeName, this), CreateInstance);
        }
        const std::string& GetTypeName() const
        {
            return typeName;
        }
        static void* CreateInstance() { return new T(); }
    };
};

// リフレクション登録のためのマクロ
// ※名前空間を設定しているのは、変数名の重複を避けるため。
#define SetReflection(className) namespace className##Reflection \
	{ \
		 static Assembly::Reflection< className > reflection; \
	}
