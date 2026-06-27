# 学习路线

这条路线按“先跑通 PC 上位机，再接硬件，再理解下位机”的顺序安排。不要一开始就改 ESP32 固件，先让官方链路稳定跑起来。

## 1. WSL2 与 VS Code Remote - WSL

目标：

- 知道 Windows 和 WSL2 Ubuntu 的边界。
- 能打开 Ubuntu 终端。
- 能在 VS Code 里通过 Remote - WSL 打开 `~/ros2_ws`。

练习：

```bash
pwd
ls
cd ~/ros2_ws
code .
```

## 2. ROS2 基础概念

目标：

- 理解 workspace、package、node、topic、message、launch。
- 知道 `source /opt/ros/humble/setup.bash` 和 `source install/setup.bash` 的区别。

练习：

```bash
source /opt/ros/humble/setup.bash
ros2 --help
ros2 topic list
```

## 3. 最小 `/cmd_vel` 控制链路

目标：

- 理解 `/cmd_vel` 是 ROS 机器人常见速度控制话题。
- 能发布 `geometry_msgs/msg/Twist`。

练习：

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 launch yahboom_pc_control cmd_vel_test.launch.py
```

另开终端：

```bash
source /opt/ros/humble/setup.bash
ros2 topic echo /cmd_vel
```

## 4. Python ROS2 节点

目标：

- 看懂 `yahboom_pc_control/cmd_vel_publisher.py`。
- 理解 timer、publisher、parameter 的作用。

建议改动：

- 把 `linear_x` 改成 `0.1`。
- 把 `angular_z` 改成 `0.0`。
- 把 `frequency_hz` 改成 `2.0`。

每次改完执行：

```bash
cd ~/ros2_ws
colcon build
source install/setup.bash
ros2 launch yahboom_pc_control cmd_vel_test.launch.py
```

## 5. micro-ROS Agent

目标：

- 理解 Agent 是 PC ROS2 与 ESP32 micro-ROS 节点之间的桥。
- 区分 UDP 和 Serial。

UDP：

```bash
~/ros2_ws/scripts/start_agent_udp.sh 8888
```

Serial：

```bash
~/ros2_ws/scripts/start_agent_serial.sh /dev/ttyUSB0 115200
```

## 6. PC bringup 启动组合

目标：

- 理解 `yahboom_robot_bringup` 这个包只负责“把多个节点一起启动”。
- 知道 Agent 和 `/cmd_vel` 节点可以放在同一个 launch 文件中。

练习：

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 launch yahboom_robot_bringup pc_bringup_udp.launch.py
```

## 7. ESP32 固件骨架

目标：

- 打开 `esp32_firmware/yahboom_esp32_micro_ros_car/main/main.c`。
- 看懂 FreeRTOS task、`CmdVel`、`WheelTarget`、差速运动学。
- 知道哪些地方以后要替换成真实 micro-ROS、PWM、PID、编码器代码。

先只阅读，不急着烧录。

## 8. 小车到货后的第一轮联调

目标：

- 先跑通官方系统，不改底层。
- 先低速测试，避免机械风险。

检查：

```bash
ros2 topic list
ros2 topic echo /odom
ros2 topic echo /imu
ros2 topic echo /scan
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.05}, angular: {z: 0.0}}"
```

## 9. RViz2 与传感器可视化

目标：

- 能在 RViz2 中看到机器人状态、雷达点云或 LaserScan。
- 理解 TF、odom、base_link、laser frame 的关系。

练习：

```bash
rviz2
```

## 10. 运动学与校准

目标：

- 理解线速度、角速度、轮距、轮径、编码器。
- 知道为什么同样的 `/cmd_vel` 在不同地面上效果会不同。

学习顺序：

1. 直线低速运动。
2. 原地旋转。
3. 编码器反馈。
4. odom 累计误差。
5. 速度和角速度校准。

## 11. ESP32 固件理解

目标：

- 看懂 Yahboom 固件如何订阅 `/cmd_vel`。
- 理解 `/cmd_vel` 到电机 PWM、编码器、PID、odom 的链路。

注意：

- 在官方系统跑通前，不建议修改固件。
- 每次固件改动前先备份原始配置。
- 烧录前确认板卡型号、端口和电源。

## 12. 后续扩展

建议方向：

1. 键盘控制节点。
2. 安全限速节点。
3. OpenCV 视觉节点。
4. 路径规划节点。
5. RViz2 可视化配置。
6. SLAM 建图。
7. Nav2 导航。
8. 多机或多节点实验。
