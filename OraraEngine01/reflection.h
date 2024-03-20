#pragma once
#include "singleton.h"
#include <functional>
#include<algorithm>

class ReflectionList
{
public:
    class IReflection//インターフェイス
    {
    public:
        string m_Name;
        function<void* ()> m_CreateInstans;
        IReflection(string name, function<void* ()> createFunction)
            :m_Name(name), m_CreateInstans(createFunction){}

    };

private:
    static vector<IReflection> m_InstansList;

public:
    vector<string> GetNameList()
    {
        vector<string> nl;
        for (auto it : m_InstansList)
        {
            nl.emplace_back(it.m_Name);
        }
        return nl;
    }
    void* CreateInstans(string name)
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

    // クイックソートのパーティション関数
    static int Partition(vector<IReflection>& arr, int low, int high) {
        string pivot = arr[low].m_Name;
        int left = low + 1;
        int right = high;

        while (true) {
            while (left <= right && arr[left].m_Name[6] <= pivot[6]) left++;
            while (left <= right && arr[right].m_Name[6] > pivot[6]) right--;

            if (left >= right) break;

            swap(arr[left], arr[right]);
        }

        swap(arr[low], arr[right]);
        return right;
    }

    // クイックソートの再帰関数
    static void QuickSort(vector<IReflection>& arr, int low, int high) {
        if (low < high) {
            int pivotIndex = ReflectionList::Partition(arr, low, high);
            QuickSort(arr, low, pivotIndex - 1);
            QuickSort(arr, pivotIndex + 1, high);
        }
    }


    template<class T>
    class Reflection
    {
    private:
        const string m_TypeName = typeid(T).name();

    public:
        Reflection(bool endflg = false)
        {
            if(!endflg)
            m_InstansList.emplace_back(m_TypeName, CreateInctans);
            else
            {
                ReflectionList::QuickSort(m_InstansList,0, m_InstansList.size() -1);
            }
        }

        static void* CreateInctans() { return new T; }
        const string GetName() const { return m_TypeName; }

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
