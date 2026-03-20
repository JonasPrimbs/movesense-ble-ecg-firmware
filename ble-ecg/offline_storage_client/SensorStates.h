#pragma once

enum SensorStates {
    Started,
    Connected,
    Streaming,
    Logging,
    Reconnected
};

enum SensorEvents {
    Connect,
    Disconnect,

    SubscribeEcg,
    UnsubscribeEcg,

    SubscribeImu,
    UnsubscribeImu,
};

enum DataStates {
    None,
    ECG,
    IMU,
    Both,
};