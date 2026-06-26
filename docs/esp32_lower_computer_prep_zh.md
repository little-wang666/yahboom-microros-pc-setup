# ESP32 下位机准备说明

当前阶段只准备 ESP32 下位机开发资料，不自动烧录固件。

## 为什么不现在烧录

ESP32 固件烧录需要确认：

- Yahboom 小车型号和控制板版本。
- ESP32 芯片型号。
- USB 串口设备名。
- 固件中的 Wi-Fi SSID、密码、PC IP、UDP 端口。
- Yahboom 最新教程中的分支、依赖和烧录命令。

这些信息没有确认前，强行烧录容易把厂家出厂固件或参数覆盖掉。

## 可以先准备什么

在 WSL2 Ubuntu 中执行：

```bash
bash "/mnt/d/codex project/microros_pc_setup/wsl/setup_esp32_micro_ros_sources.sh"
```

脚本会准备：

```text
~/esp32_microros_ws/
  MicroROS-Board/
  Mirco-Ros-Car_VM/
  micro_ros_espidf_component/
  micro-ROS-Agent/
```

这些是学习和后续对照教程用的源码，不会写入 Windows 系统环境。

## 后续硬件到货后的顺序

1. 确认小车型号、控制板版本、ESP32 芯片型号。
2. 确认 Yahboom 最新教程对应的仓库分支。
3. 备份出厂固件说明和配置。
4. 确认 USB 串口在 WSL 中是否可见。
5. 配置 Wi-Fi / UDP 参数。
6. 启动 PC 端 micro-ROS Agent。
7. 烧录 ESP32 固件。
8. 用 `ros2 topic list` 确认 topic。
9. 用低速 `/cmd_vel` 测试运动。
10. 检查 `/odom`、`/imu`、`/scan`。

## 关键提醒

- 第一阶段不要引入 STM32。
- 第一阶段不要急着改底层 PID。
- 先跑通官方系统，再做自定义固件实验。
- 每次改固件前记录改动和原始参数。
