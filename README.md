# 2D-RPG 統合オーサリングソフト

## Summary

「RPG◯クール」や「W◯LF RPGエディター」のようなオリジナルRPGを製作するためのソフトおよびゲームプログラムです。  
製作にはSquirrel言語によるスクリプトの記述が必要なため、プログラマーもしくはプログラミングに興味があるユーザーを対象にしています。  

RPGはゼロベースでの開発を行うとなると膨大な時間と工数を要するゲームジャンルの代表格です。  
一般的なゲーム開発に必要な最低限の機能を持たせたゲームプログラムをベースに組み込み用のスクリプトを使って拡張していくことで大幅な工数削減を図ることができます。  
上記2つの類似ソフトには無かった「プレーンテキストによるファイル管理」を行うことにより、Git等のバージョン管理ツールを用いたチーム開発も実現できるようにしています。  

なお、デフォルト素材は用意していませんが、Releasesに添付しているサンプルゲームのデータベースおよびスクリプトをプリセットデータを用意しているためすぐにゲームを作り始めることができます。


## ScreenShots

### スケジュール管理機能

プロジェクトに参画しているメンバーやセクション、タスクの管理を行うことができます。  

![10_schedule-01](https://user-images.githubusercontent.com/20965271/81160577-33e20080-8fc5-11ea-8d77-ac244fb47b96.png)

![10_schedule-02](https://user-images.githubusercontent.com/20965271/81160579-35132d80-8fc5-11ea-9902-88befd5a1f06.png)


### スクリプト編集機能

Squirrel言語によるスクリプト編集機能を搭載しています。  
リファレンスを内蔵しており、これを活用した豊富な入力支援機能も備えています。

![20_script-01](https://user-images.githubusercontent.com/20965271/81160582-36445a80-8fc5-11ea-8cd7-fbfce212fc6f.png)


### データベース編集機能

ゲーム中のデータ群を統括管理する仕組みを搭載しています。  
RDBMSを使用しておらず、独自実装したデータベースシステムを使用しています。  
表形式のデータベースのみならず、専用エディターを内蔵しているデータベースもあります。  

「ユーザー定義データベース」はその名の通り、構造をすべて自由に定義できるものになっています。  
このデータベースとスクリプトを作り込んでいくことにより、非常に自由度の高い独自システムを作ることができます。  

![30_db-01](https://user-images.githubusercontent.com/20965271/81160587-380e1e00-8fc5-11ea-834e-b9b5024b57f0.png)

![30_db-02](https://user-images.githubusercontent.com/20965271/81160592-38a6b480-8fc5-11ea-9449-f41d6b5a04f1.png)

![30_db-03](https://user-images.githubusercontent.com/20965271/81160595-39d7e180-8fc5-11ea-81b3-b062cf44e5fe.png)

![30_db-04](https://user-images.githubusercontent.com/20965271/81160597-3a707800-8fc5-11ea-8920-4dc320c61d2a.png)

![30_db-05a](https://user-images.githubusercontent.com/20965271/81160602-3c3a3b80-8fc5-11ea-92a9-0a65f45eb454.png)

![30_db-05b](https://user-images.githubusercontent.com/20965271/81160606-3e9c9580-8fc5-11ea-9f84-42dc83b7391a.gif)

![30_db-06](https://user-images.githubusercontent.com/20965271/81160613-40665900-8fc5-11ea-87cd-4363bcdbca34.png)

![30_db-07](https://user-images.githubusercontent.com/20965271/81160616-40feef80-8fc5-11ea-8888-129ead790bf7.png)


### マップ編集機能

マップは影層を含め4レイヤーで構成されます。  
お絵かきソフトのような感覚でタイルを配置していきながら作り込んでいきます。  

マップ上での動作は「イベント」というものを配置して、その中にスクリプトを書き込んでいくことで実現しています。  
マップイベント製作で非常によく使うテンプレートやコマンドについては、専用ダイアログから簡単に入力できるようになっています。  

![40_map-01](https://user-images.githubusercontent.com/20965271/81160621-42301c80-8fc5-11ea-8553-497583f9faeb.jpg)

![40_map-02](https://user-images.githubusercontent.com/20965271/81160624-452b0d00-8fc5-11ea-9d7b-5262c10ce95e.png)


### デバッグ機能

マップ上を歩きながらイベントをテストしたり、ゲームバランスの調整をしたり、リリース版と同等のテストをしたりできます。  
開発中はバックグラウンドでデバッグコンソールが使用できるようになっており、情報表示やデバッグ用の操作を自由に行うことができます。  
もちろん、デバッグコマンドもスクリプトによって自由に拡張できるようになっています。  

![80_debug-01](https://user-images.githubusercontent.com/20965271/81160627-465c3a00-8fc5-11ea-8ac6-44e83f02b557.png)

![80_debug-02](https://user-images.githubusercontent.com/20965271/81160632-46f4d080-8fc5-11ea-8e97-a8813b28e8c1.png)

![80_debug-03](https://user-images.githubusercontent.com/20965271/81160636-478d6700-8fc5-11ea-9ca8-8d98abecdb8d.jpg)


### スクリーンショット内に含まれる素材の提供元

- キャラクターグラフィック提供
  - 「First Seed Material」REFMAP様
  - 「ぴぽや」ぴぽ様
  - 赤まぐろ (小樽商科大学 創作活動部)
  - 白苺 (小樽商科大学 創作活動部)

- 背景・エフェクトグラフィック提供
  - 「ぴぽや」ぴぽ様
  - 「よく訓練された素材屋」玉英様

- マップタイルグラフィック提供
  - 「First Seed Material」REFMAP様
  - 「ねくらマップチップ」ねくら様

- アイコングラフィック提供
  - 「沙夢」亀高聖様
  - 「ぴぽや」ぴぽ様

- システムグラフィック提供
  - 「Creava's Nest」Creava様


### 製品版の発行

ゲームのデバッグとバランス調整を一通り終えたら、いよいよ製品版の発行を行います。  
この工程を踏むことで、各種素材データや重要なシナリオをプレイヤーによって取り出せないようにアーカイブ処理を施すことができます。  
また、デバッグ用の機能をすべて取り除くため軽量化も図ることができます。

![90_release-01](https://user-images.githubusercontent.com/20965271/81160640-48be9400-8fc5-11ea-9be7-a5dc65867c6d.png)


## Dependency

[License.txt](Editor/PlainText/License.txt)


## Author

[tissueMO](https://github.com/tissueMO)
