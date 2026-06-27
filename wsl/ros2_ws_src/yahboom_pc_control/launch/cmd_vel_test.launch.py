from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # Launch 文件的任务是“描述要启动哪些节点以及参数是什么”。
    # 这里启动 yahboom_pc_control 包中的 cmd_vel_publisher 节点，
    # 用于在没有真实小车时先学习 /cmd_vel 速度话题。
    return LaunchDescription(
        [
            Node(
                package="yahboom_pc_control",
                executable="cmd_vel_publisher",
                name="cmd_vel_publisher",
                output="screen",
                parameters=[
                    {
                        "linear_x": 0.2,
                        "angular_z": 0.1,
                        "frequency_hz": 1.0,
                    }
                ],
            )
        ]
    )
