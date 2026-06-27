import rclpy
from geometry_msgs.msg import Twist
from rclpy.node import Node


class CmdVelPublisher(Node):
    """周期性发布 /cmd_vel 的最小 ROS2 控制节点。

    这个节点现在只做一件事：按照固定频率发布 Twist 速度消息。
    后续接入小车后，ESP32 micro-ROS 节点会订阅 /cmd_vel，
    再把 linear.x 和 angular.z 转换成左右轮速度。
    """

    def __init__(self) -> None:
        super().__init__("cmd_vel_publisher")

        # 参数写在节点里，而不是写死在 publish 函数中。
        # 这样以后可以通过 launch 文件或命令行快速调整速度。
        self.declare_parameter("linear_x", 0.2)
        self.declare_parameter("angular_z", 0.1)
        self.declare_parameter("frequency_hz", 1.0)

        # 创建发布者：话题名是 /cmd_vel，消息类型是 geometry_msgs/msg/Twist。
        # 队列长度 10 表示短时间内如果处理不过来，最多缓存 10 条消息。
        self.publisher = self.create_publisher(Twist, "/cmd_vel", 10)
        frequency_hz = float(self.get_parameter("frequency_hz").value)

        # 定时器每隔 1/frequency_hz 秒调用一次 publish_cmd_vel。
        # 这就是 ROS2 Python 节点中常见的“周期任务”写法。
        self.timer = self.create_timer(1.0 / frequency_hz, self.publish_cmd_vel)
        self.get_logger().info("Publishing /cmd_vel test commands.")

    def publish_cmd_vel(self) -> None:
        # Twist 消息包含 linear 和 angular 两部分。
        # 差速小车通常使用 linear.x 表示前进速度，angular.z 表示绕自身旋转角速度。
        msg = Twist()
        msg.linear.x = float(self.get_parameter("linear_x").value)
        msg.angular.z = float(self.get_parameter("angular_z").value)

        # 发布后，任何订阅 /cmd_vel 的节点都能收到这条速度指令。
        self.publisher.publish(msg)
        self.get_logger().info(
            f"Published /cmd_vel linear.x={msg.linear.x:.3f}, angular.z={msg.angular.z:.3f}"
        )


def main(args=None) -> None:
    # 初始化 rclpy，这是所有 ROS2 Python 节点启动前必须做的事情。
    rclpy.init(args=args)
    node = CmdVelPublisher()
    try:
        # spin 会让节点进入事件循环，持续处理定时器、订阅、服务等回调。
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        # 退出前主动销毁节点并关闭 rclpy，避免资源残留。
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
