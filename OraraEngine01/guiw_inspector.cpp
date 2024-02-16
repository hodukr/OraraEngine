﻿#include "reflection.h"
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

void Inspector::SetWinodwConfig()
{
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.762f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH - SCREEN_WIDTH * 0.761f, SCREEN_HEIGHT *0.56f));
}

void Inspector::Draw()
{
    ImGui::Begin("Inspector", &m_IsShowWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
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

        //マテリアルの表示
        if (m_GameObject->GetMaterial())
        {
            DrawMaterial();
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

        //Componentfile作成
        static char str[256];
        
        if (ImGui::Button("CreateComponentfile")) {
            ImGui::OpenPopup("CreateComponent");
        }
        if (ImGui::BeginPopup("CreateComponent")) {
            ImGui::InputText("ComponentName", str, 256);
            if (ImGui::Button("Create") && strcmp(str, "") != 0)
            {
                CreatComponentFile(str);
                strcpy_s(str, "");
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();

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

        ImGui::TreePop();

    }
}

void Inspector::DrawMaterial()
{
    if (ImGui::TreeNode("Material"))
    {
        
        if (ImGui::BeginCombo("Shader", m_GameObject->GetMaterial()->GetFileName().c_str()))
        {
            std::vector<std::string> shaderfile = AccessFolder("shader");
            // ファイル名を出力 
            for (const auto& fileName : shaderfile) {

                if (fileName.find("VS.cso") == fileName.npos)continue;
                std::string name = fileName.substr(0, fileName.find('V'));
                if (ImGui::Selectable(name.c_str()))
                {
                    m_GameObject->GetMaterial()->SetShader(name.c_str());
                }
            }
            ImGui::EndCombo();
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
    {
        FolderPass* date = std::get<TYPE_FOLDERPASS>(vardate.MemberDate);
        if (ImGui::BeginCombo(vardate.Name.c_str(), date->Date.c_str()))
        {
            std::vector<std::string> files = AccessFolder(date->Pass.c_str());
            // ファイル名を出力 
            for (const auto& fileName : files) {
                if (fileName.find(date->Extension) == std::string::npos)continue;
                if (ImGui::Selectable(fileName.c_str()))
                {
                    date->Date = fileName;
                    date->IsSet = true;
                }
            }
            ImGui::EndCombo();
        }
    }
        break;
    case TYPE_CUSTOMVECTOR3:
    {
        CustomVector3* date = std::get<TYPE_CUSTOMVECTOR3>(vardate.MemberDate);
        if (date->State == STATE＿CORRECTION)
        {
            std::string cb;
            cb = "##" + vardate.Name;

            ImGui::Checkbox(cb.c_str(), &m_IsRockVector[m_NumVector]);
            ImGui::SameLine();
            Vector3* vector = date->Vector3date;

            std::string vecx = "##X" + vardate.Name;
            std::string vecy = "##Y" + vardate.Name;
            std::string vecz = "##Z" + vardate.Name;

            float x = D3DXToDegree(vector->x);
            float y = D3DXToDegree(vector->y);
            float z = D3DXToDegree(vector->z);

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
            x = D3DXToRadian(x);
            y = D3DXToRadian(y);
            z = D3DXToRadian(z);
            *vector = Vector3(x, y, z);
            m_NumVector++;
        }
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

void Inspector::CreatComponentFile(std::string comname)
{
    std::string createname = "com_" + comname + ".h";
    fs::path name= fs::current_path();
    bool isfile = true;
    for (const auto &file : fs::directory_iterator(name))
    {//大文字小文file字を区別しない比較
        std::string filename = file.path().filename().string();
        if (filename.size() == createname.size() && std::equal(filename.begin(), filename.end(), createname.begin(), [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
            }))
        {
            isfile = false;
        }
    }

    if (isfile) 
    {//ヘッダーファイル生成
        {
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
    //ソースファイル生成
        {
            std::ofstream header_file("com_" + comname + ".cpp");
            if (header_file.is_open()) {
                header_file << "#include \"main.h\"\n\n";
                header_file << "#include \"com_" << comname << ".h\"\n\n";
                header_file << "void " << comname << "::Init()\n{\n\n}\n";
                header_file << "void " << comname << "::Uninit()\n{\n\n}\n";
                header_file << "void " << comname << "::Update()\n{\n\n}\n";
                header_file << "void " << comname << "::Draw()\n{\n\n}\n";
                header_file.close();
            }
        }
    }

    std::string projectname = "OraraEngine01.vcxproj";
    //プロジェクトに追加
    AddFileToProject(projectname, "com_" + comname + ".h", true);
    AddFileToProject(projectname, "com_" + comname + ".cpp",false);
    //com_commonにincludeとSET_COMPONENT_CLASSの追加
    {
        std::ifstream file_in("com_common.h");
        if (!file_in) {
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        int current_line = 0;
        while (getline(file_in, line)) 
        {
            current_line++;
            if (line.find("//endinclude") != std::string::npos) 
            {
                std::string date = line;
                line = "#include \"com_"+ comname + ".h\"\n";
                line += date;
            }
            else if (line.find("//endSET_COMPONENT_CLASS") != std::string::npos) 
            {
                std::string date = line;
                line = "SET_COMPONENT_CLASS(" + comname + ")\n";
                line += date;
            }
            lines.push_back(line);
        }

        file_in.close();

        std::ofstream file_out("com_common.h");
        if (!file_out) 
        {
            return;
        }

        for (const std::string& l : lines) 
        {
            file_out << l << std::endl;
        }

        file_out.close();
    }
    std::ifstream file_in("com_common.h");
    if (!file_in) {
        return;
    }

    //reflection.cppにSetReflectionComponent追加
    {
        std::ofstream file("reflection.cpp", std::ios_base::app); // ファイルを追記モードで開く

        if (!file.is_open()) {
            std::cerr << "ファイルを開けませんでした。" << std::endl;
            return;
        }

        std::string line;

        // ファイルの最後の行を読み取る
        std::ifstream infile("reflection.cpp");
        if (infile.is_open()) {
            while (getline(infile, line)) {
                // 最後の行を見つける
            }
            infile.close();
        }
        else {
            std::cerr << "ファイルを開けませんでした。" << std::endl;
            return;
        }

        // 最後の行に追加するテキスト
        std::string textToAdd = "SetReflectionComponent(" + comname + ")";

        // 最後の行にテキストを追加
        file << textToAdd << std::endl;

        // ファイルを閉じる
        file.close();

    }
    
}

using namespace std;

void Inspector::AddFileToProject(const string& project_file, const string& file_path, bool is_header) {
    ifstream ifs(project_file);
    if (!ifs.is_open()) {
        cerr << "Failed to open project file: " << project_file << endl;
        return;
    }

    string line;
    ofstream ofs(project_file + ".tmp");
    if (!ofs.is_open()) {
        cerr << "Failed to create temporary project file." << endl;
        return;
    }

    bool found_item_group = false;
    bool found_existing_item_group = false;
    bool added_file = false;

    while (getline(ifs, line)) {
        if (!found_item_group && line.find("<ItemGroup>") != string::npos) {
            found_item_group = true;
        }

        if (found_item_group && !found_existing_item_group && (line.find("<ClCompile") != string::npos || line.find("<ClInclude") != string::npos)) {
            found_existing_item_group = true;
        }

        if (found_item_group && found_existing_item_group && !added_file) {
            // Add new file entry
            if (is_header) {
                ofs << "    <ClInclude Include=\"" << file_path << "\"/>\n";
            }
            else {
                ofs << "    <ClCompile Include=\"" << file_path << "\"/>\n";
            }
            added_file = true;
        }

        ofs << line << endl;
    }

    if (!found_item_group || !found_existing_item_group) {
        // If ItemGroup for source/header files doesn't exist, create a new one
        ofs << "  <ItemGroup>\n";
        if (is_header) {
            ofs << "    <ClInclude Include=\"" << file_path << "\" />\n";

        }
        else {
            ofs << "    <ClCompile Include=\"" << file_path << "\" />\n";
        }
        ofs << "  </ItemGroup>\n";
    }

    ifs.close();
    ofs.close();

    ////フィルタ分け
    {
        //入力用
        ifstream ifs("OraraEngine01.vcxproj.filters");
        if (!ifs.is_open()) {
            cerr << "Failed to open filters file: " << "OraraEngine01.vcxproj.filters" << endl;
            return;
        }

        vector<string> lines;
        string line;

        while (getline(ifs, line)) {
            lines.push_back(line);
        }

        ifs.close();

        //出力用
        ofstream ofs("OraraEngine01.vcxproj.filters");
        if (!ofs.is_open()) {
            cerr << "Failed to open filters file for writing: " << "OraraEngine01.vcxproj.filters" << endl;
            return;
        }
        bool addfile = true;
        for (const string& l : lines) {
            ofs << l << endl;
            if (is_header)
            {
                if (addfile && l.find("<ClInclude Include=") != string::npos) {
                    ofs << "    <ClInclude Include=\"" << file_path << "\">\n";
                    ofs << "      <Filter>Component</Filter>\n";
                    ofs << "    </ClInclude>\n";
                    addfile = false;
                }

            }
            else
            {
                if (addfile && l.find("<ClCompile Include=") != string::npos) {
                    ofs << "    <ClCompile Include=\"" << file_path << "\">\n";
                    ofs << "      <Filter>Component</Filter>\n";
                    ofs << "    </ClCompile>\n";
                    addfile = false;
                }
            }
            
        }
        ofs.close();
    }

    // 元のプロジェクトファイルを一次的なプロジェクトファイルに置き換える
    remove(project_file.c_str());
    rename((project_file + ".tmp").c_str(), project_file.c_str());
}
