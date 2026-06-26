# Windows 隔离原则

本项目的核心原则是不要把机器人开发依赖堆到 Windows 原生环境里。Windows 负责显示、编辑器和宿主系统；ROS2 和 micro-ROS 开发放进 WSL2 Ubuntu 22.04。

## 做什么

- 在 Windows 中安装 VS Code。
- 在 Windows 中安装 VS Code Remote - WSL 扩展。
- 在 WSL2 Ubuntu 22.04 中安装 ROS2 Humble。
- 在 WSL2 中创建 `~/ros2_ws`。
- 在 WSL2 中运行 `colcon build`、`ros2`、`micro_ros_agent`。
- 在 WSL2 中创建 Python venv。

## 不做什么

- 不在 Windows 原生 PowerShell 里安装 ROS2。
- 不把 ROS2 Python 包安装到 Windows Python。
- 不把 Linux 的构建依赖装进 Windows 系统目录。
- 不把每个 ROS2 workspace 都写入 `.bashrc`。
- 不在没有确认硬件型号前烧录 ESP32。

## 推荐目录

Windows 侧仓库：

```text
D:\codex project\microros_pc_setup
```

WSL2 侧 ROS2 workspace：

```text
~/ros2_ws
```

WSL2 侧 ESP32 参考源码：

```text
~/esp32_microros_ws
```

## 为什么 workspace overlay 不自动写入 `.bashrc`

`/opt/ros/humble/setup.bash` 是 ROS2 Humble 的基础环境，可以写入 `.bashrc`。

`~/ros2_ws/install/setup.bash` 是当前 workspace 的 overlay。如果每个项目都写入 `.bashrc`，以后同时做多个 ROS2 项目时，环境会互相覆盖，排查问题会很麻烦。因此本项目要求手动执行：

```bash
source ~/ros2_ws/install/setup.bash
```
