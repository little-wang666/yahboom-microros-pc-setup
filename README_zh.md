# Yahboom MicroROS 小车 PC 上位机环境包

这个仓库用于准备 Yahboom MicroROS 小车的 PC 上位机开发环境。设计目标是：Windows 只作为宿主系统和 VS Code 图形界面，ROS2 Humble、micro-ROS Agent、colcon workspace、Python 虚拟环境都放在 WSL2 Ubuntu 22.04 中，避免污染 Windows 原生环境。

## 当前完成度

已准备：

- WSL2 Ubuntu 22.04 安装入口脚本：`windows/install_wsl_ubuntu22_admin.ps1`
- ROS2 Humble PC 上位机初始化脚本：`wsl/setup_ros2_humble_pc.sh`
- `~/ros2_ws` workspace 初始化逻辑
- Python venv：`~/ros2_ws/.venv`
- 最小 ROS2 Python package：`yahboom_pc_control`
- `/cmd_vel` 测试发布节点
- launch 文件：`cmd_vel_test.launch.py`
- micro-ROS Agent UDP 启动脚本
- micro-ROS Agent Serial 启动脚本
- ESP32 下位机准备说明和源代码获取脚本

未完成，也不应该在现阶段强行完成：

- ESP32 固件烧录
- Yahboom 具体车型的 Wi-Fi / UDP / 串口参数写入
- 真实小车的 `/odom`、`/imu`、`/scan` 联调验证
- RViz2 与雷达/地图真实数据验证

原因：下位机固件烧录依赖具体板卡、串口设备、Yahboom 对应车型教程和硬件连接状态。现在先准备 PC 上位机和 ESP 开发资料，等硬件到手后再按厂家教程烧录和验证。

## 目录结构

```text
microros_pc_setup/
  README_zh.md
  LICENSE
  .gitignore
  docs/
    esp32_lower_computer_prep_zh.md
    hardware_bringup_checklist_zh.md
    learning_roadmap_zh.md
    windows_isolation_zh.md
  windows/
    install_wsl_ubuntu22_admin.ps1
  wsl/
    setup_ros2_humble_pc.sh
    setup_esp32_micro_ros_sources.sh
    scripts/
      start_agent_udp.sh
      start_agent_serial.sh
    ros2_ws_src/
      yahboom_pc_control/
```

## 安装步骤

1. 右键 PowerShell，选择“以管理员身份运行”。

2. 执行：

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
& "D:\codex project\microros_pc_setup\windows\install_wsl_ubuntu22_admin.ps1"
```

3. 如果 Windows 提示重启，先重启。

4. 第一次打开 Ubuntu-22.04，按提示创建 Linux 用户名和密码。

5. 在 Ubuntu 终端中执行：

```bash
bash "/mnt/d/codex project/microros_pc_setup/wsl/setup_ros2_humble_pc.sh"
```

6. 安装完成后打开上位机 workspace：

```bash
cd ~/ros2_ws
code .
```

## 上位机验证

终端 1：

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 topic echo /cmd_vel
```

终端 2：

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 launch yahboom_pc_control cmd_vel_test.launch.py
```

看到 `/cmd_vel` 中持续出现 `linear.x=0.2`、`angular.z=0.1` 一类数据，就说明最小 ROS2 控制链路正常。

## micro-ROS Agent

UDP 默认端口 `8888`：

```bash
~/ros2_ws/scripts/start_agent_udp.sh
```

指定 UDP 端口：

```bash
~/ros2_ws/scripts/start_agent_udp.sh 8888
```

串口方式示例：

```bash
~/ros2_ws/scripts/start_agent_serial.sh /dev/ttyUSB0 115200
```

## ESP32 下位机准备

当前仓库不会自动烧录 ESP32。可以先在 WSL 中获取 ESP32 相关源码和参考项目：

```bash
bash "/mnt/d/codex project/microros_pc_setup/wsl/setup_esp32_micro_ros_sources.sh"
```

它会在 `~/esp32_microros_ws` 中准备：

- Yahboom MicroROS-Board 仓库
- Yahboom Mirco-Ros-Car_VM 仓库
- micro-ROS ESP-IDF component
- micro-ROS Agent 源码参考

后续拿到小车后，再根据实际硬件型号、串口设备和 Yahboom 最新教程进行 ESP32 固件配置与烧录。

## 接下来学习路线

详细路线见 [docs/learning_roadmap_zh.md](docs/learning_roadmap_zh.md)。建议顺序：

1. 学会 WSL2 和 VS Code Remote - WSL 的基本使用。
2. 理解 ROS2 workspace、package、node、topic、message。
3. 跑通 `/cmd_vel` 发布和 `ros2 topic echo`。
4. 理解 launch 文件如何启动节点。
5. 学会启动 micro-ROS Agent，并理解 UDP / Serial 的区别。
6. 硬件到手后按清单检查电源、串口、Wi-Fi、Agent、topic。
7. 先低速控制小车，再看 `/odom`、`/imu`、`/scan`。
8. 再进入 RViz2、建图、导航、视觉和路径规划。
9. 最后研究 Yahboom ESP32 固件中 `/cmd_vel` 到电机控制、编码器、PID、odom 的链路。

## 隔离原则

详细说明见 [docs/windows_isolation_zh.md](docs/windows_isolation_zh.md)。核心原则：

- 不在 Windows 原生环境安装 ROS2。
- 不把 ROS2 依赖混进 Windows Python。
- ROS2、colcon、micro-ROS Agent 都放进 WSL2 Ubuntu。
- 代码进入 `~/ros2_ws/src`，不要散落在系统目录。
- 只把 `/opt/ros/humble/setup.bash` 写入 `~/.bashrc`，workspace overlay 手动 source。

## 参考资料

- Microsoft WSL 安装文档：https://learn.microsoft.com/en-us/windows/wsl/install
- VS Code Remote - WSL 文档：https://code.visualstudio.com/docs/remote/wsl
- ROS2 Humble Ubuntu deb 文档：https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html
- micro-ROS Agent：https://github.com/micro-ROS/micro-ROS-Agent
- micro-ROS ESP-IDF component：https://github.com/micro-ROS/micro_ros_espidf_component
- Yahboom Mirco-Ros-Car_VM：https://github.com/YahboomTechnology/Mirco-Ros-Car_VM
- Yahboom MicroROS-Board：https://github.com/YahboomTechnology/MicroROS-Board
