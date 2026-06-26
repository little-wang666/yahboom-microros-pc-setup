import rclpy
from geometry_msgs.msg import Twist
from rclpy.node import Node


class CmdVelPublisher(Node):
    def __init__(self) -> None:
        super().__init__("cmd_vel_publisher")
        self.declare_parameter("linear_x", 0.2)
        self.declare_parameter("angular_z", 0.1)
        self.declare_parameter("frequency_hz", 1.0)

        self.publisher = self.create_publisher(Twist, "/cmd_vel", 10)
        frequency_hz = float(self.get_parameter("frequency_hz").value)
        self.timer = self.create_timer(1.0 / frequency_hz, self.publish_cmd_vel)
        self.get_logger().info("Publishing /cmd_vel test commands.")

    def publish_cmd_vel(self) -> None:
        msg = Twist()
        msg.linear.x = float(self.get_parameter("linear_x").value)
        msg.angular.z = float(self.get_parameter("angular_z").value)
        self.publisher.publish(msg)
        self.get_logger().info(
            f"Published /cmd_vel linear.x={msg.linear.x:.3f}, angular.z={msg.angular.z:.3f}"
        )


def main(args=None) -> None:
    rclpy.init(args=args)
    node = CmdVelPublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
