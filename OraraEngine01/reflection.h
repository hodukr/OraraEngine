#pragma once
#include <vector>
#include <functional>
#include <string>
#include "singleton.h"
//#include <cereal/types/memory.hpp>

class ReflectionList
{
public:
    class IReflection//インターフェイス
    {
    public:
        std::string m_Name;
        std::function<void* ()> m_CreateInstans;
        IReflection(std::string name, std::function<void* ()> createFunction)
            :m_Name(name), m_CreateInstans(createFunction){}

    };

private:
    static std::vector<IReflection> m_InstansList;

public:
    std::vector<std::string> GetNameList()
    {
        std::vector<std::string> nl;
        for (auto it : m_InstansList)
        {
            nl.emplace_back(it.m_Name);
        }
        return nl;
    }
    void* CreateInstans(std::string name)
    {
        for (auto it : m_InstansList)
        {
            if (it.m_Name == name)
            {
                return it.m_CreateInstans();
            }
        }
        return nullptr;
    }


    template<class T>
    class Reflection
    {
    private:
        const std::string m_TypeName = typeid(T).name();

    public:
        Reflection()
        {
            m_InstansList.emplace_back(m_TypeName, CreateInctans);
        }

        static void* CreateInctans() { return new T; }
        const std::string GetName() const { return m_TypeName; }

    };

};
#define SetReflection(typeName)namespace typeName##Reflection \
{\
static ReflectionList::Reflection<typeName> reflection;\
}


#define SetReflectionComponent(typeName)namespace typeName##Reflection \
{\
static ReflectionList::Reflection<typeName> reflection;\
}\
