# feat/* ブランチの違いと特徴の調査結果

## 概要

このドキュメントは、gn10-can リポジトリに存在する3つの feat/* ブランチの違いと特徴をまとめたものです。各ブランチは、CANライブラリの設計上の課題（循環参照の解決、使いやすさの向上など）に対する異なるアプローチを実装しています。

## 対象ブランチ

1. **feat/proposal-1-interface** - インターフェース分離による依存関係の逆転
2. **feat/proposal-4-forward-decl** - 前方宣言を活用したシンプル構成
3. **feat/proposal-5-raii** - RAIIによる自動登録とモダンAPI

## 比較表

| 項目 | proposal-1-interface | proposal-4-forward-decl | proposal-5-raii |
|------|---------------------|------------------------|-----------------|
| **主要コンセプト** | 依存関係逆転の原則 (DIP) | 前方宣言による循環参照解消 | RAII による自動管理 |
| **管理クラス名** | CANBusController | CANManager | CANBus |
| **インターフェース** | ICANSender, ICANReceiver | なし（前方宣言のみ） | なし（friend クラス） |
| **登録方法** | 手動（register_receiver） | 手動（register_device） | 自動（コンストラクタ） |
| **循環参照の解決** | インターフェース分離 | 前方宣言 + .cpp実装 | friend + RAII |
| **コード量** | 多（インターフェース追加） | 少（最小変更） | 中（API改善含む） |
| **使いやすさ** | 標準的 | 標準的 | 非常に高い |
| **テスタビリティ** | 非常に高い | 中 | 中〜高 |

## 各ブランチの詳細

---

### 1. feat/proposal-1-interface

**設計思想**: 依存関係逆転の原則（Dependency Inversion Principle）を適用

#### 主要な変更点

- `CANManager` → `CANBusController` に名称変更
- 新しいインターフェースの導入:
  - `ICANSender`: フレーム送信機能
  - `ICANReceiver`: フレーム受信機能
- `CANDevice` は `ICANReceiver` を実装し、`ICANSender` に依存
- `CANBusController` は `ICANSender` を実装

#### アーキテクチャ

```
DriverInterface
       ↑
       |
CANBusController (implements ICANSender)
       ↑                    ↑
       |                    |
       |              ICANSender
       |                    ↑
       |                    |
    registers          uses for send
       |                    |
ICANReceiver ← implements ← CANDevice
```

#### 使用例

```cpp
gn10_can::drivers::Stm32CanDriver driver;
gn10_can::CANBusController controller(driver);

MyMotorDriver motor(controller, 0x200);
controller.register_receiver(&motor);

while (true) {
    controller.update();
}
```

#### メリット

- ✅ 循環参照の完全な解消
- ✅ 高いテスタビリティ（モックが容易）
- ✅ 責務の明確な分離
- ✅ SOLID原則に準拠

#### デメリット

- ❌ ファイル数とコード量の増加
- ❌ インターフェースの概念理解が必要
- ❌ やや複雑な設計

---

### 2. feat/proposal-4-forward-decl

**設計思想**: C++の基本機能（前方宣言）を活用した最小限の変更

#### 主要な変更点

- クラス名は `CANManager`, `CANDevice` のまま維持
- ヘッダーファイルで前方宣言を使用:
  ```cpp
  class CANManager; // in can_device.hpp
  class CANDevice;  // in can_manager.hpp
  ```
- 実装（.cpp）ファイルで完全な定義をインクルード
- ID ベースのルーティング機能強化

#### アーキテクチャ

```
DriverInterface
       ↑
       |
   CANManager (holds std::map<uint32_t, CANDevice*>)
       ↑                    
       |                    
    pointer               
       |                    
   CANDevice (holds CANManager*)
```

#### 使用例

```cpp
gn10_can::drivers::Stm32CanDriver driver;
gn10_can::CANManager manager(driver);

MotorDriver motor1(&manager, 0x100);
manager.register_device(&motor1);

while (true) {
    manager.update();
}
```

#### メリット

- ✅ 最もシンプルな解決策
- ✅ 既存コードからの変更が最小限
- ✅ コンパイル時間の短縮
- ✅ 学習コストが低い

#### デメリット

- ❌ 密結合（CANDevice が CANManager の実装に依存）
- ❌ テスト時のモック作成がやや困難
- ❌ 依然として相互依存関係が存在

---

### 3. feat/proposal-5-raii

**設計思想**: Developer Experience (DX) 最優先、"It just works"

#### 主要な変更点

- `CANManager` → `CANBus` に名称変更（物理バスの概念）
- RAII パターンによる自動登録/解除:
  - コンストラクタで自動的にバスに登録
  - デストラクタで自動的にバスから解除
- `friend class CANDevice` を使用して内部APIを隠蔽
- ID管理の改善（Type + ID ベースのキー）

#### アーキテクチャ

```
DriverInterface
       ↑
       |
    CANBus (friend of CANDevice)
       ↑                    
       |                    
  attach/detach          
  (automatic)               
       |                    
   CANDevice (RAII managed)
   - Constructor: auto attach
   - Destructor: auto detach
```

#### 使用例

```cpp
gn10_can::drivers::Stm32CanDriver driver;
gn10_can::CANBus bus(driver);

// インスタンス化するだけで自動登録！
MyMotor motor_left(bus, 0x100);
MyMotor motor_right(bus, 0x101);

while (true) {
    bus.update();
    motor_left.set_speed(100);
}
// スコープアウトで自動的に登録解除
```

#### メリット

- ✅ 最高の使いやすさ（最小コード量）
- ✅ 登録忘れのバグを防止
- ✅ スコープベースの安全な管理
- ✅ ダングリングポインタの防止
- ✅ 直感的なAPI

#### デメリット

- ❌ コンストラクタの副作用が見えにくい
- ❌ 単体テスト時にモックバスが必要
- ❌ C++のRAII概念の理解が必要

---

## ファイル変更の比較

### proposal-1-interface
```
主な追加ファイル:
+ include/gn10_can/core/can_bus_controller.hpp
+ include/gn10_can/core/ican_sender.hpp
+ include/gn10_can/core/ican_receiver.hpp
+ src/core/can_bus_controller.cpp

削除ファイル:
- include/gn10_can/core/can_manager.hpp
- src/core/can_manager.cpp

変更: 17 files (+224, -233)
```

### proposal-4-forward-decl
```
主な追加ファイル:
+ src/core/can_device.cpp (新規)

主な変更:
* include/gn10_can/core/can_device.hpp (大幅簡素化)
* include/gn10_can/core/can_manager.hpp (ルーティング改善)

変更: 13 files (+121, -186)
```

### proposal-5-raii
```
主な追加ファイル:
+ include/gn10_can/core/can_bus.hpp
+ src/core/can_bus.cpp

削除ファイル:
- include/gn10_can/core/can_manager.hpp
- src/core/can_manager.cpp

主な変更:
* include/gn10_can/core/can_device.hpp (RAII 実装)
* include/gn10_can/devices/motor_driver.hpp (API 改善)

変更: 16 files (+362, -445)
```

---

## 設計原則と適用パターン

| ブランチ | 主要な設計パターン | SOLID 原則 |
|---------|------------------|-----------|
| proposal-1 | Dependency Inversion, Observer | DIP, ISP, SRP |
| proposal-4 | Forward Declaration (言語機能) | SRP |
| proposal-5 | RAII, Facade | SRP, (部分的に DIP) |

---

## 推奨事項と選択基準

### proposal-1-interface を選ぶべき場合:
- テスト駆動開発（TDD）を重視するプロジェクト
- 大規模な開発チームでの長期保守
- SOLID原則への準拠が重要
- 複数の実装を切り替える可能性がある

### proposal-4-forward-decl を選ぶべき場合:
- 既存コードベースへの影響を最小限にしたい
- シンプルさとパフォーマンスを優先
- 小〜中規模のプロジェクト
- C++の基本機能のみを使いたい

### proposal-5-raii を選ぶべき場合:
- 初心者や新規メンバーが多いチーム
- 使いやすさ（DX）を最優先したい
- バグの発生を最小限に抑えたい
- モダンなC++のベストプラクティスを採用したい

---

## 技術的な詳細

### ID管理とルーティング

#### proposal-1
- `ICANReceiver::get_target_id()` で受信IDを取得
- `std::map<uint32_t, ICANReceiver*>` でルーティング

#### proposal-4
- 登録時に明示的にRX IDを指定
- `std::map<uint32_t, CANDevice*>` でルーティング
- ベースIDとマスクによる効率的なマッチング

#### proposal-5
- Type(4bit) + ID(4bit) をキーとして使用
- コマンド部（3bit）を除いたベースIDで登録
- `std::map<uint32_t, CANDevice*>` でルーティング

### メモリ管理

すべてのブランチで:
- デバイスはポインタで管理（所有権は外部）
- 動的メモリ割り当ては最小限（組み込み向け）
- `std::map` の使用（必要に応じて固定配列に変更可能）

---

## コミット履歴の分析

### proposal-1: 4 commits (main より先)
```
1c6d0f3 feat: enhance CANDevice and CANBusController...
6b722dd feat: replace test_can_manager with test_can_bus_controller...
08787ee feat: refactor CAN communication structure...
ba6a64d feat: implement CANManager class...
```

### proposal-4: 多数のcommits
- 設計ドキュメントの更新を含む
- 段階的な実装とテストの追加
- ビルドシステムの改善も含まれる

### proposal-5: 多数のcommits
- モダンAPIの段階的な導入
- テストケースの全面改訂
- 使用例とドキュメントの刷新

---

## テストコードの違い

### proposal-1
```cpp
TEST(CANBusControllerTest, RegisterAndRoute) {
    MockDriver driver;
    CANBusController controller(driver);
    MockReceiver receiver(0x100);
    controller.register_receiver(&receiver);
    // ...
}
```

### proposal-4
```cpp
TEST(CANManagerTest, RegisterDevice) {
    MockDriver driver;
    CANManager manager(driver);
    MockDevice device(&manager, 0x100);
    manager.register_device(&device, 0x100);
    // ...
}
```

### proposal-5
```cpp
TEST(CANBusTest, AutoRegisterOnConstruction) {
    MockDriver driver;
    CANBus bus(driver);
    // インスタンス化で自動登録
    MockDevice device(bus, 0x100);
    // デストラクタで自動解除
}
```

---

## 結論

3つの feat/* ブランチは、それぞれ異なる設計哲学と優先事項を持っています:

1. **proposal-1-interface**: ソフトウェア工学の原則を重視した「教科書的」な実装
2. **proposal-4-forward-decl**: 実用性とシンプルさを重視した「実践的」な実装  
3. **proposal-5-raii**: ユーザー体験を重視した「モダン」な実装

最終的な選択は、プロジェクトの規模、チームの経験レベル、保守性への要求などに応じて決定すべきです。

## 関連ドキュメント

- [提案1の詳細](design/01_interface_injection.md)
- [提案4の詳細](design/04_simple_forward_decl.md)
- [提案5の詳細](design/05_modern_auto_register.md)
- [設計提案の比較まとめ](design/comparison_summary.md) (mainブランチ)

---

**作成日**: 2026-01-28  
**対象リポジトリ**: ararobo/gn10-can  
**調査者**: GitHub Copilot
