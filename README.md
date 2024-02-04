# 金丸 豊岳　担当箇所


# 廣川 鳳介　担当箇所
**・シーン内のセーブ・ロード機能（cereal）**

1.セーブ
cerealを使用してjsonファイルにScene内のGameObject、またGameObject内のCompoentの情報を保存できるようにしました。
ComponetはComoponetクラスを基底としてポリモーフィズムで各派生Comoponetを保存できるように対応した。
※その場合に各派生クラスと基底クラスの関連づけをCerealに行わなければならないならない。

2.ロード
cerealの機能でポリモーフィズムなクラスを代入してくれるのでそのままSceneファイルを読み込み保存したデータをインスタンス化できる。

**エディター画面上でのGameObjectにComoponetの追加**

1.ImGui
ImGuiを使用してGuiを実装しました。

2.Componentのファクトリー
Componetをオブジェクトに追加する場合に派生クラス名のリスト化を行い、クラス名（const char*）を取得してインスタンス化を行う。
※この時に派生クラスを事前に手動でリストに設定しなければならない。
