# 金丸 豊岳　担当箇所


# 廣川 鳳介　担当箇所
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
