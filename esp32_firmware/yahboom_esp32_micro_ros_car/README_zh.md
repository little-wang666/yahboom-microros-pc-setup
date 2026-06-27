# yahboom_esp32_micro_ros_car 固件包说明

这个目录是 ESP32 下位机固件的学习骨架，面向 ESP-IDF + FreeRTOS + micro-ROS。

当前不会自动烧录，也不会覆盖 Yahboom 出厂固件。它的任务是让你提前理解 ESP32 端代码应该如何组织，后续确认具体板卡和厂家教程后，再把骨架迁移到真实工程中。

## 固件包任务

- 连接 Wi-Fi。
- 启动 micro-ROS client。
- 订阅 PC 发来的 `/cmd_vel`。
- 把 `linear.x` 和 `angular.z` 转换为左右轮目标速度。
- 预留编码器读取、PID、PWM 输出、odom 发布位置。

## 目录结构

```text
yahboom_esp32_micro_ros_car/
  CMakeLists.txt
  sdkconfig.defaults
  main/
    CMakeLists.txt
    main.c
```

## 后续真实烧录前必须确认

1. Yahboom 小车型号。
2. ESP32 芯片型号。
3. 控制板电机引脚、PWM 通道、编码器引脚。
4. Wi-Fi SSID、密码、PC IP、UDP 端口。
5. Yahboom 最新教程使用的 ESP-IDF 版本。
6. `micro_ros_espidf_component` 的放置方式。

## 典型 ESP-IDF 命令

以下命令只作为未来参考，当前仓库不自动执行：

```bash
idf.py set-target esp32s3
idf.py menuconfig
idf.py build
idf.py flash
idf.py monitor
```

## 学习重点

1. ESP32 不是运行完整 ROS2，而是运行 micro-ROS client。
2. `/cmd_vel` 是从 PC 发来的速度目标。
3. ESP32 要把速度目标变成电机 PWM。
4. 实时控制循环通常由 FreeRTOS task 周期执行。
5. 编码器反馈用于 PID 和里程计估算。
