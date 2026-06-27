# Yahboom MicroROS 小车 PC 上位机环境包

这个仓库用于准备 Yahboom MicroROS 小车的 PC 上位机开发环境。设计目标是：Windows 只作为宿主系统和 VS Code 图形界面，ROS2 Humble、micro-ROS Agent、colcon workspace、Python 虚拟环境都放在 WSL2 Ubuntu 22.04 中，避免污染 Windows 原生环境。

## 当前完成度

已准备：

- WSL2 Ubuntu 22.04 安装入口脚本：`windows/install_wsl_ubuntu22_admin.ps1`
- ROS2 Humble PC 上位机初始化脚本：`wsl/setup_ros2_humble_pc.sh`
- `~/ros2_ws` workspace 初始化逻辑
- Python venv：`~/ros2_ws/.venv`
- 最小 ROS2 Python package：`yahboom_pc_control`
- ROS2 启动组合 package：`yahboom_robot_bringup`
- `/cmd_vel` 测试发布节点
- launch 文件：`cmd_vel_test.launch.py`
- micro-ROS Agent UDP 启动脚本
- micro-ROS Agent Serial 启动脚本
- ESP32 下位机准备说明和源代码获取脚本
- ESP-IDF 工具链安装脚本：`wsl/install_esp_idf_tools.sh`
- ESP-IDF 下位机固件学习骨架：`esp32_firmware/yahboom_esp32_micro_ros_car`

当前这台电脑已实测完成：

- WSL2 发行版：`Ubuntu-22.04`，位置在 `D:\wsl\Ubuntu-22.04`
- ROS2：Humble，安装在 WSL 的 `/opt/ros/humble`
- PC 工作区：WSL 内 `~/ros2_ws`
- 已构建包：`yahboom_pc_control`、`yahboom_robot_bringup`、`micro_ros_agent`、`micro_ros_msgs`
- ESP-IDF：`v5.2.5`，主目录在 `D:\Espressif\v5.2.5\esp-idf`
- ESP-IDF 工具链：Espressif 官方 EIM 放在 `C:\Espressif\tools`
- ESP32 骨架工程：已用 `esp32s3` 目标编译通过

说明：ROS2 和 micro-ROS Agent 没有安装到 Windows 原生环境，仍然隔离在 WSL2 中。ESP32 烧录工具需要 Windows 直接访问串口，所以 ESP-IDF 使用 Espressif 官方 Windows 工具链；其中 ESP-IDF 源码在 D 盘，工具链缓存由官方安装器放在 `C:\Espressif\tools`。

## 本机已完成验收清单

下面这些不是“计划要做”，而是已经在这台电脑上实际跑通过的内容：

1. WSL2 已启用，`Ubuntu-22.04` 已导入并设置为 WSL2 发行版。
2. Ubuntu 22.04 已能正常启动，项目路径可从 WSL 访问：`/mnt/d/codex project/microros_pc_setup`。
3. ROS2 Humble 已安装在 WSL 内，基础命令 `ros2` 可用。
4. PC 上位机工作区 `~/ros2_ws` 已创建，并完成 `colcon build`。
5. `yahboom_pc_control` 已构建成功，可以发布 `/cmd_vel` 测试速度指令。
6. `yahboom_robot_bringup` 已构建成功，可以作为后续真车联调的 launch 入口。
7. `micro_ros_agent` 已从源码按 Humble 分支构建成功，后续可通过 UDP 或 Serial 连接 ESP32 下位机。
8. `micro_ros_msgs` 已构建成功，micro-ROS 消息依赖已补齐。
9. VS Code Remote - WSL、Python、C/C++、CMake Tools 扩展已安装，后续可以用 VS Code 打开 WSL 工作区学习代码。
10. ESP-IDF v5.2.5 已安装，`idf.py --version` 已验证通过。
11. ESP32 学习骨架 `esp32_firmware/yahboom_esp32_micro_ros_car` 已按 `esp32s3` 目标完整编译通过。
12. 已生成后续可烧录所需的固件产物，包括 bootloader、partition table 和 `yahboom_esp32_micro_ros_car.bin`。
13. 已新增 `windows/build_esp32_firmware.ps1`，后续可一键重新编译 ESP32 工程。
14. 已新增 `windows/open_esp_idf_shell.ps1`，后续可打开 ESP-IDF 专用终端执行 `idf.py build`、`idf.py -p COMx flash monitor`。
15. 仓库中的 ROS2 包、ESP32 工程骨架和脚本都放在 `D:\codex project\microros_pc_setup`，方便你直接打开、阅读和继续写代码。

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
  esp32_firmware/
    yahboom_esp32_micro_ros_car/
  windows/
    install_wsl_ubuntu22_admin.ps1
    build_esp32_firmware.ps1
    open_esp_idf_shell.ps1
  wsl/
    setup_ros2_humble_pc.sh
    install_esp_idf_tools.sh
    setup_esp32_micro_ros_sources.sh
    scripts/
      start_agent_udp.sh
      start_agent_serial.sh
    ros2_ws_src/
      yahboom_pc_control/
      yahboom_robot_bringup/
```

## 已创建的软件包

### PC 上位机：`yahboom_pc_control`

位置：`wsl/ros2_ws_src/yahboom_pc_control`

任务：

- 发布 `/cmd_vel`。
- 用 `geometry_msgs/msg/Twist` 表达线速度和角速度。
- 作为后续键盘控制、视觉控制、导航控制的最小起点。
- 代码中已经加入中文注释，方便逐行学习。

### PC 启动组合：`yahboom_robot_bringup`

位置：`wsl/ros2_ws_src/yahboom_robot_bringup`

任务：

- 启动 micro-ROS Agent。
- 启动安全的 `/cmd_vel` 测试节点。
- 作为未来真车联调的一键启动入口。
- launch 文件中已经加入中文注释，解释参数和节点作用。

### ESP32 下位机：`yahboom_esp32_micro_ros_car`

位置：`esp32_firmware/yahboom_esp32_micro_ros_car`

任务：

- 提供 ESP-IDF 工程骨架。
- 用 FreeRTOS task 拆分 micro-ROS 通信和电机控制。
- 演示 `/cmd_vel` 到左右轮速度的差速运动学转换。
- 预留 PWM、PID、编码器、odom 发布位置。
- 当前只作为学习骨架，不自动烧录真实 ESP32。

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

也可以在 Windows PowerShell 中一次性安装 WSL 侧 ROS2、ESP32 参考源码和 ESP-IDF：

```powershell
powershell -ExecutionPolicy Bypass -File "D:\codex project\microros_pc_setup\windows\run_wsl_environment_setup.ps1"
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

也可以启动组合 launch：

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 launch yahboom_robot_bringup pc_bringup_udp.launch.py
```

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

ESP32 学习骨架在：

```text
esp32_firmware/yahboom_esp32_micro_ros_car
```

它解释了 ESP32 侧为什么要用 FreeRTOS task、如何接收 `/cmd_vel`、如何把速度转换成左右轮目标速度，以及未来在哪里补 PWM、PID、编码器和 `/odom`。

如果你后续要准备 ESP-IDF 工具链，在 WSL 中执行：

```bash
bash "/mnt/d/codex project/microros_pc_setup/wsl/install_esp_idf_tools.sh"
```

这个脚本只安装 ESP-IDF 到 `~/esp/esp-idf`，不会烧录 ESP32，也不会改 Windows 原生环境。

当前电脑已经安装了 Windows 侧 ESP-IDF v5.2.5。编译 ESP32 骨架工程可以直接在 PowerShell 执行：

```powershell
powershell -ExecutionPolicy Bypass -File "D:\codex project\microros_pc_setup\windows\build_esp32_firmware.ps1"
```

如果你想打开一个可持续输入命令的 ESP-IDF 终端：

```powershell
powershell -ExecutionPolicy Bypass -File "D:\codex project\microros_pc_setup\windows\open_esp_idf_shell.ps1"
```

打开后常用命令是：

```powershell
idf.py build
idf.py -p COMx flash monitor
```

这里的 `COMx` 要等 ESP32 用 USB 连接电脑后，在设备管理器里查看真实串口号。

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
