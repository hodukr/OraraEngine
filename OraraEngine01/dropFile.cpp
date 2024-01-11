#include "main.h"
#include "dropFile.h"
#include "accessFolder.h"
#include <list>
#include <string>

HDROP DropFile::m_Drop{};
std::list<fs::path> DropFile::m_fullPathNames;


void DropFile::DropFileData()
{
    if (AccessFolder::Instance().GetNowFolder().empty())
        return;

    UINT count = 0;
    UINT length = 0;
    wchar_t* fullPathName = nullptr;

    // ドラッグされたファイル数を取得
    count = DragQueryFile(m_Drop, 0xFFFFFFFF, NULL, 0);

    // ファイル数でループ
    for (int i = 0; i < static_cast<int>(count); i++)
    {
        // フルパスの長さを取得(NULL終端を含まず)
        length = DragQueryFile(m_Drop, i, NULL, 0);

        // フルパスを受け取る変数を初期化(NULL終端を含む)
        fullPathName = new wchar_t[length + 1];
        wmemset(fullPathName, L'\0', length + 1);

        // フルパスを取得
        DragQueryFile(m_Drop, i, reinterpret_cast<LPSTR>(fullPathName), length + 1);

        //ここでwchar_t*型からconst char*に変換しないとなぜかバグる
        const char* fullPathNameChar = reinterpret_cast<const char*>(fullPathName);


        m_fullPathNames.push_back(fullPathNameChar);

        // 変数の解放
        delete[] fullPathName;

    }

    for (auto filePath : m_fullPathNames) {

        CopyFileToProjectFolder(filePath);

    }
    m_fullPathNames.clear(); // リストのクリア

}

void DropFile::CopyFileToProjectFolder(fs::path filePath)
{
    
    // プロジェクトのルートディレクトリを取得
    //ここにプロジェクトフォルダまでのパスが格納されている
    fs::path projectRoot = fs::current_path();

    // "asset\\texture"フォルダの相対パスを設定
    //プロジェクトフォルダから保存したい場所へのパスが格納されている
    fs::path relativeFolderPath = "asset\\" + AccessFolder::Instance().GetNowFolder();

    // プロジェクトフォルダにコピーするためのファイルパスを作成
    fs::path destinationPath = projectRoot / relativeFolderPath / fs::path(filePath).filename();

    if (fs::is_regular_file(filePath))
    {
       // ファイルをコピーする
       //ここでファイルをコピーして指定のパスのフォルダに格納している
       fs::copy_file(filePath, destinationPath, fs::copy_options::overwrite_existing);
    }
    else
    {
        assert(fs::exists(filePath));
        assert(fs::exists(destinationPath));

        //同じパスではなく、シンボリックリンク／ハードリンクでもないので、等価ではない
        assert(!fs::equivalent(filePath, destinationPath));
    }
   
   
      
    
}
