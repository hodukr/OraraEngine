#pragma once
class UIManager
{
private:
    UIManager() {};
    ~UIManager() {};
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    static UIManager* m_instance;
public:
    static UIManager* getInstance() {
        if (m_instance == nullptr) {
            m_instance = new UIManager();
        }
        return m_instance;
    }

    static void releaseInstance() {
        if (m_instance != nullptr) {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    void Init();
    void Uninit();
    void Update();
};

UIManager* UIManager::m_instance = nullptr;
