from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # 这个 bringup launch 文件用于“PC 上位机联调入口”。
    # 它同时启动 micro-ROS Agent 和一个测试 /cmd_vel 发布节点。
    # 真车调试时可以把 cmd_vel_publisher 注释掉，换成键盘、导航或视觉控制节点。
    udp_port_arg = DeclareLaunchArgument(
        "udp_port",
        default_value="8888",
        description="ESP32 micro-ROS 通过 UDP 连接 PC Agent 时使用的端口。",
    )

    linear_x_arg = DeclareLaunchArgument(
        "linear_x",
        default_value="0.0",
        description="测试速度指令的前进速度，真车第一次联调建议保持 0。",
    )

    angular_z_arg = DeclareLaunchArgument(
        "angular_z",
        default_value="0.0",
        description="测试速度指令的旋转角速度，真车第一次联调建议保持 0。",
    )

    micro_ros_agent = Node(
        package="micro_ros_agent",
        executable="micro_ros_agent",
        name="micro_ros_agent_udp",
        output="screen",
        arguments=["udp4", "--port", LaunchConfiguration("udp_port"), "-v6"],
    )

    cmd_vel_test_node = Node(
        package="yahboom_pc_control",
        executable="cmd_vel_publisher",
        name="cmd_vel_publisher",
        output="screen",
        parameters=[
            {
                "linear_x": LaunchConfiguration("linear_x"),
                "angular_z": LaunchConfiguration("angular_z"),
                "frequency_hz": 1.0,
            }
        ],
    )

    return LaunchDescription(
        [
            udp_port_arg,
            linear_x_arg,
            angular_z_arg,
            micro_ros_agent,
            cmd_vel_test_node,
        ]
    )
