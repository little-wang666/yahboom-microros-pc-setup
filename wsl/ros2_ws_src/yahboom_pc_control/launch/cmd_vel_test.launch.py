from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
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
