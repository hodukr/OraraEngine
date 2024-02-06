#include "reflection.h"
#include "guiw_inspector.h"
#include "gameObject.h"
#include "imgui/imgui.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
namespace fs = std::filesystem;

void Inspector::Init()
{
    m_GameObject = nullptr;
    m_PopupComponent = nullptr;
    m_NumVector = 0;
}

void Inspector::Uninit()
{
    m_GameObject = nullptr;
    m_IsRockVector.clear();
}

void Inspector::Update()
{

}

void Inspector::Draw()
{
    ImGui::Begin("Inspector", &m_IsShowWindow, ImGuiWindowFlags_NoScrollbar);
    if (m_GameObject)
    {
        //オブジェクト名表示
        char buffer[256];
        strncpy_s(buffer, m_GameObject->GetName().c_str(), sizeof(buffer));
        ImGui::InputText("##GameObjectName", buffer, sizeof(buffer));
        m_GameObject->SetName(buffer);



        //コンポーネントの表示 
        for (auto& obj : *m_GameObject->GetList())
        {
            DrawComponent(obj.get());
            m_NumVector = 0;
        }




        if (ImGui::Button("AddComponent")) {

            ImGui::OpenPopup("ComponentList");
        }
        // ポップアップメニュー
        if (ImGui::BeginPopup("ComponentList")) {
            for (auto it : ReflectionList().GetNameList())
            {
                std::string name = it.substr(it.find(" ") + 1);
                if (ImGui::MenuItem(name.c_str())) {
                    m_GameObject->AddComponent(ReflectionList().CreateInstans(it.c_str()));

                }

            }
            ImGui::EndPopup();

        }

        static char str[256];
        ImGui::InputText("ComponentName", str, 256);

        if (ImGui::Button("CreateComponent")) {
            CreatComponent(str);
            strcpy_s(str, "");
        }
    }

    ImGui::End();
}

void Inspector::DrawComponent(Component* component)
{
    std::string comname = typeid(*component).name();
    std::string name = comname.substr(6);

    bool isTreenode = ImGui::TreeNode(name.c_str());

    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("ComponentConfig");
        m_PopupComponent = component;
    }


    if (m_PopupComponent == component)
    {
        if (ImGui::BeginPopup("ComponentConfig")) {

            if (ImGui::Selectable("Delet"))
            {
                m_PopupComponent->SetDestroy();

            }
            ImGui::EndPopup();
        }
    }

    if (isTreenode)
    {
        std::vector<TypeDate> datelist = component->GetDateList();

        for (auto date : datelist)
        {
            Drawvariable(date);
        }

        if (typeid(Mesh).name() == comname)
        {
            Mesh* com = dynamic_cast<Mesh*>(component);

            if (ImGui::TreeNode(com->GetModelpas().c_str()))
            {
                // フォルダのパスを指定 
                std::string folderPath = "asset\\model";

                // ファイル名を格納するためのベクターを作成 
                std::vector<std::string> fileNames;

                try { 
                    // 指定されたフォルダ内のファイルをイテレート 
                    for (const auto& entry : fs::directory_iterator(folderPath)) {
                        std::string filename = entry.path().filename().string();
                        std::string extension = filename.substr(filename.find_last_of(".") +1);
                        if (extension == "obj")
                        {
                            // ファイル名をベクターに追加 
                            fileNames.push_back(entry.path().filename().string());
                        }
                    }
                }
                catch (const std::filesystem::filesystem_error& ex) {
                    std::cerr << "Error: " << ex.what() << std::endl;
                }

                // ファイル名を出力 
                for (const auto& fileName : fileNames) {
                    if (ImGui::Selectable(fileName.c_str()))
                    {
                        com->SetModel(fileName.c_str());
                    }
                }
                ImGui::TreePop();
            }
            Material* material = com->GetMaterial();
            std::string comname = typeid(*material).name();
            std::string name = comname.substr(6);
            if (ImGui::TreeNode(name.c_str())) {
                // フォルダのパスを指定 
                std::string folderPath = "shader";

                // ファイル名を格納するためのベクターを作成 
                std::vector<std::string> fileNamesVS;
                std::vector<std::string> fileNamesPS;

                try {
                    // 指定されたフォルダ内のファイルをイテレート 
                    for (const auto& entry : fs::directory_iterator(folderPath)) {

                        std::string filename = entry.path().filename().string();
                        std::string extension = filename.substr(filename.find_last_of(".") - 2);
                        if (extension == "VS.cso")
                        {
                            // ファイル名をベクターに追加 
                            fileNamesVS.push_back(entry.path().filename().string());
                        }
                        if (extension == "PS.cso")
                        {
                            fileNamesPS.push_back(entry.path().filename().string());
                        }
                    }
                }
                catch (const std::filesystem::filesystem_error& ex) {
                    std::cerr << "Error: " << ex.what() << std::endl;
                }
                if (ImGui::BeginCombo("VS", material->GetShaderVSName().c_str()))
                {
                    // ファイル名を出力 
                    for (const auto& fileName : fileNamesVS) {
                        if (ImGui::Selectable(fileName.c_str()))
                        {
                            material->SetShaderVS(fileName.c_str());
                        }
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::BeginCombo("PS",material->GetShaderPSName().c_str()))
                {
                    // ファイル名を出力 
                    for (const auto& fileName : fileNamesPS) {
                        if (ImGui::Selectable(fileName.c_str()))
                        {
                            material->SetShaderPS(fileName.c_str());
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::TreePop();

            }

        }

        ImGui::TreePop();

    }
}

void Inspector::Drawvariable(TypeDate& vardate)
{
    switch (vardate.MemberDate.index())
    {
    case TYPE_INT:
        ImGui::InputInt(vardate.Name.c_str(), std::get<TYPE_INT>(vardate.MemberDate));
        break;
    case TYPE_FLOAT:
        ImGui::InputFloat(vardate.Name.c_str(), std::get<TYPE_FLOAT>(vardate.MemberDate));

        break;
    case TYPE_STRING:
        char str[256];
        strncpy_s(str, std::get<TYPE_STRING>(vardate.MemberDate)->c_str(), sizeof(str));
        ImGui::InputText(vardate.Name.c_str(), str,256);
        *std::get<TYPE_STRING>(vardate.MemberDate) = str;
        break;
    case TYPE_BOOL:
        ImGui::Checkbox(vardate.Name.c_str(), std::get<TYPE_BOOL>(vardate.MemberDate));
        break;
    case TYPE_VECTOR3:
    {
        std::string cb;
        cb = "##" + vardate.Name;

        ImGui::Checkbox(cb.c_str(), &m_IsRockVector[m_NumVector]);
        ImGui::SameLine();
        Vector3* vector = std::get<Vector3*>(vardate.MemberDate);

        std::string vecx = "##X" + vardate.Name;
        std::string vecy = "##Y" + vardate.Name;
        std::string vecz = "##Z" + vardate.Name;

        float x = vector->x;
        float y = vector->y;
        float z = vector->z;

        ImGui::SetNextItemWidth(70.0f);
        if (ImGui::InputFloat(vecx.c_str(), &x) && m_IsRockVector[m_NumVector])
        {
            y = x;
            z = x;
        }
        ImGui::SameLine();

        ImGui::SetNextItemWidth(70.0f);
        if (ImGui::InputFloat(vecy.c_str(), &y) && m_IsRockVector[m_NumVector])
        {
            x = y;
            z = y;
        }
        ImGui::SameLine();

        ImGui::SetNextItemWidth(70.0f);
        if (ImGui::InputFloat(vecz.c_str(), &z) && m_IsRockVector[m_NumVector])
        {
            x = z;
            y = z;
        }
        ImGui::SameLine();
        ImGui::Text(vardate.Name.c_str());
        *vector = Vector3(x, y, z);
        m_NumVector++;
    }
        break;
    case TYPE_D3DXCOLOR:
        ImGui::ColorEdit4(vardate.Name.c_str(), *std::get<TYPE_D3DXCOLOR>(vardate.MemberDate));
        break;
    case TYPE_FOLDERPASS:
        if (ImGui::BeginCombo(vardate.Name.c_str(), std::get<TYPE_FOLDERPASS>(vardate.MemberDate)->Date.c_str()))
        {
            std::vector<std::string> files = AccessFolder(std::get<TYPE_FOLDERPASS>(vardate.MemberDate)->Pass.c_str());
            // ファイル名を出力 
            for (const auto& fileName : files) {
                if (ImGui::Selectable(fileName.c_str()))
                {
                    std::get<TYPE_FOLDERPASS>(vardate.MemberDate)->Date = fileName;
                    std::get<TYPE_FOLDERPASS>(vardate.MemberDate)->IsSet = true;
                }
            }
            ImGui::EndCombo();
        }
        break;
    default:
        break;
    }
}

std::vector<std::string> Inspector::AccessFolder(const char* folderPass)
{
    // フォルダのパスを指定 
    std::string folderPath = folderPass;

    // ファイル名を格納するためのベクターを作成 
    std::vector<std::string> fileNames;

    try {
        // 指定されたフォルダ内のファイルをイテレート 
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            std::string name = entry.path().filename().string();
            fileNames.push_back(name);
        }
    }
    catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return fileNames;
}

void Inspector::CreatComponent(std::string comname)
{
    std::string createname = "com_" + comname + ".h";
    fs::path name= fs::current_path();
    for (const auto &file : fs::directory_iterator(name))
    {//大文字小文file字を区別しない比較
        std::string filename = file.path().filename().string();
        if (filename.size() == createname.size() && std::equal(filename.begin(), filename.end(), createname.begin(), [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
            }))
        {
            return;
        }
    }

    {//ヘッダーファイル生成
        std::ofstream header_file("com_" + comname + ".h");
        if (header_file.is_open()) {
            header_file << "#ifndef " << comname << "_H\n";
            header_file << "#define " << comname << "_H\n\n";

            header_file << "#include \"component.h\"\n\n";

            header_file << "class " << comname << ":public Component\n";
            header_file << "{\n";
            header_file << "private:\n\n";
            header_file << "public:\n";
            header_file << "    void Init()override;\n";
            header_file << "    void Uninit()override;\n";
            header_file << "    void Update()override;\n";
            header_file << "    void Draw()override;\n\n";
            header_file << "    template<class Archive>\n";
            header_file << "    void serialize(Archive & archive)\n";
            header_file << "    {\n";
            header_file << "        //archive(CEREAL_NVP());\n";
            header_file << "    }\n";
            header_file << "};\n";
            header_file << "\n#endif /* " << comname << "_H */\n";
            header_file.close();
        }
    }
    {//ソースファイル生成
        std::ofstream header_file("com_" + comname + ".cpp");
        if (header_file.is_open()) {
            header_file << "#include com_\"" <<comname <<".h\"\n\n";
            header_file << comname << "::Init()\n{\n\n}\n";
            header_file << comname << "::Uninit()\n{\n\n}\n";
            header_file << comname << "::Update()\n{\n\n}\n";
            header_file << comname << "::Draw()\n{\n\n}\n";
            header_file.close();
        }
    }

    {//com_commonにincludeとdefineの追加
        std::ifstream file_in("com_common.h");
        if (!file_in) {
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        int current_line = 0;
        bool incfig = true;
        while (getline(file_in, line)) {
            current_line++;
            if (line == " " && incfig) {
                incfig = false;
                std::string date = line;
                line = "#include \"com_"+ comname + ".h\"\n";
                line += date;
            }
            lines.push_back(line);
        }

        file_in.close();

        std::ofstream file_out("com_common.h");
        if (!file_out) {
            return;
        }

        for (const std::string& l : lines) {
            file_out << l << std::endl;
        }

        file_out.close();
    }
}
