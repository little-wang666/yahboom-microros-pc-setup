#include <math.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 说明：
// 这个文件是 ESP32 下位机固件的学习骨架。
// 当前不直接依赖 micro-ROS 头文件，是为了在没有安装 ESP-IDF/micro_ros_espidf_component 时也能阅读。
// 后续真实工程中，需要加入 rcl/rclc/geometry_msgs/nav_msgs 等 micro-ROS 头文件。

typedef struct {
    float linear_x_mps;   // 小车前进速度，单位 m/s，对应 ROS2 Twist.linear.x
    float angular_z_rps;  // 小车旋转角速度，单位 rad/s，对应 ROS2 Twist.angular.z
} CmdVel;

typedef struct {
    float left_mps;   // 左轮目标线速度
    float right_mps;  // 右轮目标线速度
} WheelTarget;

static const float WHEEL_BASE_M = 0.18f;  // 左右轮间距，真实值需要根据小车实际尺寸修改
static CmdVel latest_cmd_vel = {0.0f, 0.0f};

static WheelTarget convert_cmd_vel_to_wheels(CmdVel cmd)
{
    // 差速小车运动学：
    // 左轮速度 = 车体线速度 - 角速度 * 轮距 / 2
    // 右轮速度 = 车体线速度 + 角速度 * 轮距 / 2
    WheelTarget target;
    target.left_mps = cmd.linear_x_mps - cmd.angular_z_rps * WHEEL_BASE_M * 0.5f;
    target.right_mps = cmd.linear_x_mps + cmd.angular_z_rps * WHEEL_BASE_M * 0.5f;
    return target;
}

static void apply_motor_pwm(WheelTarget target)
{
    // TODO: 在这里把左右轮目标速度转换成 PWM。
    // 真实代码需要：
    // 1. 读取编码器速度
    // 2. PID 计算
    // 3. 设置电机方向 GPIO
    // 4. 输出 PWM 占空比
    printf("target left=%.3f m/s, right=%.3f m/s\n", target.left_mps, target.right_mps);
}

static void publish_odometry_placeholder(void)
{
    // TODO: 在这里发布 /odom。
    // 真实代码需要根据编码器积分估计 x、y、yaw，
    // 再通过 micro-ROS publisher 发给 PC。
}

static void motor_control_task(void *arg)
{
    (void)arg;

    // 20 ms 控制周期约等于 50 Hz，适合入门阶段的电机控制实验。
    const TickType_t period = pdMS_TO_TICKS(20);

    while (1) {
        WheelTarget target = convert_cmd_vel_to_wheels(latest_cmd_vel);
        apply_motor_pwm(target);
        publish_odometry_placeholder();
        vTaskDelay(period);
    }
}

static void micro_ros_task(void *arg)
{
    (void)arg;

    // TODO: 在真实工程中，这里完成 micro-ROS 初始化。
    // 典型流程：
    // 1. 配置 Wi-Fi 或 Serial transport
    // 2. 创建 rcl node
    // 3. 创建 /cmd_vel subscriber
    // 4. 创建 /odom publisher
    // 5. executor spin，持续处理 ROS2 消息

    while (1) {
        // 这里暂时模拟收到一条零速度指令。
        // 真实工程中 latest_cmd_vel 应在 /cmd_vel 回调函数里更新。
        latest_cmd_vel.linear_x_mps = 0.0f;
        latest_cmd_vel.angular_z_rps = 0.0f;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    // app_main 是 ESP-IDF 应用入口，相当于普通 C 程序里的 main。
    // 我们把通信和电机控制拆成两个 FreeRTOS task，方便理解职责边界。
    xTaskCreate(micro_ros_task, "micro_ros_task", 8192, NULL, 5, NULL);
    xTaskCreate(motor_control_task, "motor_control_task", 4096, NULL, 6, NULL);
}
