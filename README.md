私たちはUnityを参考にしたゲームエンジンの制作に取り組んでいます。

# 金丸 豊岳　担当箇所（TOYOTAKE_Sフォルダにすべて、または一部担当したスクリプトがあります。）
**ファイル操作**
1. Assetフォルダ
- UntiyのAssetフォルダを再現
- プロジェクトのassetという名前のフォルダ内に入っているフォルダやファイルを見れるようにしました。
- asset直下にフォルダの追加、削除できるようにしました。
- フォルダの中のファイル削除・アイコンサイズ変更をできるようにしました。
- フォルダ・ファイルに対応したアイコンが出るようにしました。
  - 対応フォルダ・ファイル：空のフォルダ、中身が入っているフォルダ、.wav、.json、.obj、.fbx
  - .png、.jpg、.ddsはテクスチャそのものが描画されます。
- C++17対応のfilesystemを使ってプロジェクト内のフォルダ・ファイルの操作をしています。
2. 外部ファイルの追加
- 外部のファイルをドラック＆ドロップで追加できるようにしました。
- 先ほど作ったAssetフォルダで選ばれているフォルダに追加できます。
- WindowsAPIの関数を使ってドロップされたファイルのパスを取りfilesystemで操作しています。

**シーン画面**
1. カメラ操作
- デバック用のカメラ操作を作成しました。
- 操作：マウス右クリック＋WASD　前後左右、マウス右クリック＋QE　上下、左クリック＋フリック　視点移動) 
2. 描画
- レンダリングパスの最後の画面のテクスチャをImGUIで描画しています。
- Assetフォルダからシーン画面にドラック＆ドロップでモデルなどを配置できるようにしました。
- 追加予定
  -  モデルなどをマウスで操作。
  -  エディタモードとゲームモード実装

**レンダリング関連**
1. レンダリングパス
- パスを生成しやすくするためPassクラスの実装これを基底にパスを作成できる
  - 現在:環境マップ、影、ポストエフェクト実装
- パスごとにクラスを作ることによって必要な情報をクラスごとに書けるようになりました。
  - 環境マップ:位置と映すオブジェクトを選択、影:映すオブジェクトを選択など   
2. シェーダー
- hlslで書いたシェーダーを追加(Vertex,Pixel)
  - 現在:色変更、環境マップ、HPゲージ,ノイズ、影、ライティング、水面表現などのシェーダーを作成
- 追加予定
  -  ゲームに合わせたシェーダーの追加
  -  シェーダーごとの変数操作
  -  ポストエフェクトを実行画面で追加削除
 
**当たり判定**
1. 形
- Unityのように形のUIを描画し描画に合わせた当たり判定を付けました。
　-現在:箱、球 
- UIの描画は頂点を指定しDirectXのLINESTRIPをつかって描画しています。
2. 判定・機能
- CollisionShapeという基底クラスを使って形を作れるようにしました。
　- 機能
  　- 追加すると勝手に補正される
    - NONE、STAY、ENTER、EXITの状態を追加。
    - Unityのトリガー(補正なしで判定だけ取る)の追加
- 判定
　- 箱型：AABBによる判定
  - 球型：距離による判定 
- コールバックで状態と当たった相手の形のクラスが返ってくるので状態ごとのコードを書けるようになりました。
  - ※他のプロジェクトで動作確認済みだが現プロジェクトで使えるようになっていない
- 追加予定
  -  OBBによる判定
  -  現プロジェクトで状態ごとのコードを書けるように
  -  他の形追加(カプセルなど)
 
# 廣川 鳳介　担当箇所（HOSUKE_Sフォルダにすべて、または一部担当したスクリプトがあります。）
**外部ライブラリの導入**
1. cereal 
- Sceneデータ・DebugWindowデータの保存に使用しました。
2. Dear ImGui
- エディター上のGUIに使用しています。
- メインウィンドウのサイズが変更された場合にマウスとGUiのポジションがずれるのを修正しました。
- 各GUIをメニューウィンドウで表示、ウィンドウの非表示ができるようにしました。
- ImGui用のManagerClassを作成し、ウィンドウの表示、非表示がしやすいような設計しました。
  - ※エンジン側の処理からImGuiの関数を使用しないようにする。Debugで使用した場合必ず消去すること。

**シーン内のセーブ・ロード機能（cereal）**
1. セーブ
- cerealを使用してjsonファイルにScene内のGameObject、またGameObject内のCompoentの情報を保存できるようにしました。
- ComponetはComoponetクラスを基底としてポリモーフィズムで各派生Comoponetを保存できるように対応しました。
  - ※その場合に各派生クラスと基底クラスの関連づけをCerealに行わなければいけません。
2. ロード
- ロード時に現在のシーンの消去、次のシーンをセットし読み込むようにしました。
- cerealの機能でポリモーフィズムなクラスを代入してくれるのでそのままSceneファイルを読み込み保存したデータをインスタンス化できます。

**エディター画面上でのGameObject・Comoponetの操作**
1. GameObjectの追加
- hierarchyウィンドウの作成をおこないました。
- エディター上でのGameObjectの追加・削除を行えるようにしました。
- 追加時にデフォルトで名前がGameObjectになりますが二つ目以降に数字で名前が被らないようにしました。
2. Componentのファクトリー
- Componetをオブジェクトに追加する場合に派生クラス名のリスト化を行い、クラス名（const char*）を取得してインスタンス化を行います。
  - ※この時に派生クラスを事前に手動でリストに設定しなければいけません。
3. Inspectorウィンドウの作成
- GameObject内の編集をエディター上で行えるようにしました。
- 各派生Componentのメンバ変数をlist化しlistに登録された型の変数をエディター上で編集できるようにしました。
  - 現在実装できている型(int,float,Vector3,bool)
  - 今後char*型を実装していきたいと考えています。またファイルアクセスの指定も実装していきたいです。
  
**作成Componet**
1. Transform 
- position,Scale,Rotationの要素を持たせ各要素得のセッターを複数作成。
- GameObjectClassを生成時に必ず入るようにしました。
- 追加予定
  -  親子関係の構築

2. praticle
- 炎の表現を目標に制作しました。
- 設定パラメータ
  - StateColor-開始時の色
  - StartRotation-開始時の回転角度
  - StartLifetime-パーティクルの寿命
  - EmissionRate-パーティクルの発生頻度
  - Speed-開始時の速さ
  - MaxParticles-パーティクルの最大表示数
  - AddStateVelocity-開始時にパーティクルの移動方向を絞る（デフォルトはランダムで全方向に飛ぶ）
  - AddVelocity-パーティクルに加えるベクトル
  - GradationColor-グラデーション中の色の指定（三色まで）
  - IsGradation-グラデーションをするかどうかのフラグ
  - Fluctuation-揺らぎ
  - ~~TexName-テクスチャファイル名~~(エディター上での設定はまだできていない)
  - GradationLength-グラデーションの各色の長さ調整
  - IsFade-フェードアウトをするかどうかのフラグ
  - StartFadeTime-フェードをし始める時間
  - declineFade-フェードの消える強さ
 - 追加予定
   - 一フレームで生成されるパーティクルの数の制御
   - 面でのパーティクル生成範囲の指定
