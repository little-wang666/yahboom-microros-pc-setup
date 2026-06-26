# 硬件到货后联调清单

这份清单用于小车到货后的第一次联调。目标是低风险跑通官方链路。

## 1. 物理检查

- 小车底盘装配完成。
- 电机线连接正确。
- 编码器线连接正确。
- IMU、雷达、控制板连接正确。
- 电池电压正常。
- 轮子悬空或放在安全低速测试区域。

## 2. 控制板检查

- 确认 Yahboom MicroROS 控制板型号。
- 确认 ESP32 芯片型号。
- 确认 USB 串口设备。
- 确认 Wi-Fi 模式和 UDP 端口。

## 3. PC 端启动

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
~/ros2_ws/scripts/start_agent_udp.sh 8888
```

如果走串口：

```bash
~/ros2_ws/scripts/start_agent_serial.sh /dev/ttyUSB0 115200
```

## 4. ROS2 topic 检查

```bash
ros2 topic list
ros2 topic echo /odom
ros2 topic echo /imu
ros2 topic echo /scan
```

## 5. 低速运动测试

先直线低速：

```bash
ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.05}, angular: {z: 0.0}}"
```

再原地低速旋转：

```bash
ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: 0.1}}"
```

立即停止：

```bash
ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: 0.0}}"
```

## 6. RViz2

```bash
rviz2
```

检查：

- fixed frame 是否正确。
- LaserScan 是否显示。
- TF 是否报错。
- odom 是否更新。

## 7. 出问题时先看这些

- PC 和小车是否在同一网络。
- ESP32 配置的 PC IP 是否正确。
- micro-ROS Agent 端口是否一致。
- 防火墙是否拦截 UDP。
- WSL2 网络是否能收到 UDP。
- 串口权限是否正确。
- 电源是否稳定。
