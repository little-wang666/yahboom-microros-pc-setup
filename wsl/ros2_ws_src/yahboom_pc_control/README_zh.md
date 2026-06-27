# yahboom_pc_control 软件包说明

这个 ROS2 Python 软件包负责 PC 上位机最基础的速度控制实验。

## 当前任务

- 创建 `/cmd_vel` 发布节点。
- 周期性发布 `geometry_msgs/msg/Twist` 消息。
- 帮你理解 PC 如何向 ESP32 小车发送速度指令。

## 主要文件

- `yahboom_pc_control/cmd_vel_publisher.py`：核心节点代码。
- `launch/cmd_vel_test.launch.py`：启动节点并传入默认速度参数。
- `package.xml`：声明 ROS2 依赖。
- `setup.py`：把 Python 函数注册成 ROS2 可执行节点。

## 运行方式

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 launch yahboom_pc_control cmd_vel_test.launch.py
```

另开终端查看话题：

```bash
source /opt/ros/humble/setup.bash
ros2 topic echo /cmd_vel
```

## 后续学习点

1. 把固定速度改成键盘控制。
2. 增加最大速度限制，避免小车突然高速运动。
3. 增加“超时自动停车”逻辑。
4. 让视觉或导航节点输出 `/cmd_vel`。
