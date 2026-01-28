# 提案5: RAIIによる自動登録とモダンAPI

この提案は、**「ユーザーがいかに少ないコード量で、ミスなく安全に実装できるか」**（Developer Experience）を最優先した設計です。
RAII (Resource Acquisition Is Initialization) パターンを活用した自動登録と、直感的なAPIを提供します。

## 1. コンセプト: "It just works"

これまでの提案でユーザーにとって手間だった「マネージャーへの手動登録」を廃止します。
デバイスのインスタンスを作成するだけで、自動的に通信ネットワークに参加できるようにします。

## 2. クラス設計の刷新

### 2.1 `CANBus` (旧 CANManager)

管理クラスをより直感的な `CANBus` という名前にします。これは物理的な「バス」をソフトウェア上で表現したものです。
デバイスタイプとIDの組み合わせで複数デバイスを安全に管理します。

```cpp
class CANBus {
    // 受信ID（上位ビット：タイプ+ID）をキーにしたルーティングテーブル
    // 下位3ビット（コマンド部）は無視してマッチングします
    std::map<uint32_t, CANDevice*> devices_;
    drivers::DriverInterface& driver_;
    
    // ユーザーは直接呼ばない（CANDeviceが呼ぶ）
    friend class CANDevice;
    void attach(CANDevice* device, uint32_t rx_id);
    void detach(uint32_t rx_id);

public:
    CANBus(drivers::DriverInterface& driver) : driver_(driver) {}
    
    // バス全体の更新
    void update();
    
    // 生のフレーム送信（基本はDevice経由で行うが、デバッグ用に公開）
    bool send_raw(const CANFrame& frame);
};
```

### 2.2 `CANDevice` (自動登録基底クラス)

このクラスを継承するだけで、登録解除処理が自動化されます。
デバイスタイプとIDを指定することで、ユニークなCAN-IDを自動計算し、競合を防ぎます。

```cpp
class CANDevice {
protected:
    CANBus& bus_;
    id::DeviceType type_; // デバイス種別
    uint8_t id_;          // ユニットID (0-15)

public:
    // コンストラクタで自動登録 (Attach)
    // CAN-ID = (Type << 7) | (ID << 3)
    CANDevice(CANBus& bus, id::DeviceType type, uint8_t id) 
        : bus_(bus), type_(type), id_(id) {
        // コマンド部を除いたベースIDで登録
        uint32_t key = id::pack(type, id, 0) >> 3;
        bus_.attach(this, key);
    }

    // デストラクタで自動解除 (Detach)
    virtual ~CANDevice() {
        uint32_t key = id::pack(type, id, 0) >> 3;
        bus_.detach(key);
    }
    
    // 【改善点】オーバーロードによる直感的な送信API
    // 例: データのみ送信
    template <typename CmdEnum>
    bool send(CmdEnum cmd, const std::vector<uint8_t>& data);

    // 受信ハンドラ（純粋仮想関数）
    virtual void on_receive(const CANFrame& frame) = 0;
};
```

## 3. 使用例: 驚くほどシンプルに

ユーザーが書くコード（main.cpp）は劇的に簡素化されます。
異なる種類のデバイス同士であれば、同じID番号でも共存可能です。

```cpp
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/motor_driver.hpp"

// ユーザー定義のセンサー
class MySensor : public gn10_can::CANDevice {
public:
    // 親クラスのコンストラクタでタイプとIDを指定
    MySensor(gn10_can::CANBus& bus, uint8_t id) 
        : CANDevice(bus, gn10_can::id::DeviceType::SensorHub, id) {}

    void on_receive(const gn10_can::CANFrame& frame) override {
        // ...処理...
    }
};

int main() {
    gn10_can::drivers::Stm32CanDriver driver;
    gn10_can::CANBus bus(driver);

    // インスタンス化するだけで通信可能！
    // register_device() の呼び出し忘れバグが発生しない
    
    // モータードライバ Type=1, ID=1 (CAN-ID 0x??)
    gn10_can::devices::MotorDriver motor(bus, 1);
    
    // センサーHub Type=5, ID=1 (CAN-ID 0x??)
    // ID番号が同じでもタイプが違うため、異なるCAN-IDとして扱われます
    MySensor sensor(bus, 1);

    while (true) {
        bus.update();
        
        motor.send_target(100.0f);
    }
}
```

## 4. メリット・デメリット

### メリット
- **使いやすさNo.1**: 初期化コードが最小限で済みます。
- **安全性 (RAII)**: インスタンスがスコープアウトして削除されると、自動的にバスからも登録抹消されるため、ダングリングポインタ（無効なポインタへのアクセス）を防げます。
- **APIの隠蔽**: 複雑な `CANFrame` 構造体操作をユーザーに見せず、`send({1, 2, 3})` のように書けます。

### デメリット
- **隠れた依存性**: 「コンストラクタで副作用（登録処理）が発生する」ことが見えにくいため、純粋な単体テストを行う際にMock Busが必要になります。
