# yahboom_robot_bringup 软件包说明

这个 ROS2 软件包负责“启动组合”。它本身不写复杂控制算法，而是把多个节点按正确顺序启动起来。

## 当前任务

- 启动 micro-ROS Agent。
- 启动 PC 侧 `/cmd_vel` 测试发布节点。
- 给未来真车联调提供统一入口。

## 主要文件

- `launch/pc_bringup_udp.launch.py`：UDP 模式联调入口。
- `package.xml`：声明依赖 `micro_ros_agent` 和 `yahboom_pc_control`。
- `setup.py`：安装 launch 文件到 ROS2 share 目录。

## 运行方式

先确保已经 build：

```bash
cd ~/ros2_ws
colcon build
source install/setup.bash
```

启动 UDP Agent 和安全的零速度测试节点：

```bash
ros2 launch yahboom_robot_bringup pc_bringup_udp.launch.py
```

指定端口和测试速度：

```bash
ros2 launch yahboom_robot_bringup pc_bringup_udp.launch.py udp_port:=8888 linear_x:=0.05 angular_z:=0.0
```

## 学习重点

1. launch 文件如何声明参数。
2. launch 文件如何启动多个节点。
3. micro-ROS Agent 在 PC 与 ESP32 之间扮演什么角色。
4. 真车联调时为什么默认速度要从 0 或很小开始。
