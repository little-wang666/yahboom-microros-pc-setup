#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "kinematics.h"
#include "pid.h"
#include "encoder.h"
#include "motor.h"

/* =========================
   全局控制参数
========================= */

const float dt = 0.01f;   // 10ms控制周期

/* =========================
   控制器实例
========================= */

pid_controller_t pid_left;
pid_controller_t pid_right;

encoder_t enc_left;
encoder_t enc_right;

/* =========================
   ROS2 cmd_vel（临时模拟）
========================= */

typedef struct {
    float linear_x;
    float angular_z;
} cmd_vel_t;

volatile cmd_vel_t latest_cmd = {0.0f, 0.0f};

/* =========================
   运动学转换
========================= */

static WheelTarget cmd_to_wheel(cmd_vel_t cmd)
{
    WheelTarget target;

    float L = 0.18f; // 轮距

    target.left_mps  = cmd.linear_x - cmd.angular_z * L * 0.5f;
    target.right_mps = cmd.linear_x + cmd.angular_z * L * 0.5f;

    return target;
}

/* =========================
   控制任务（核心闭环）
========================= */

static void control_task(void *arg)
{
    (void)arg;

    while (1)
    {
        /* 1. 运动学 */
        WheelTarget target = cmd_to_wheel(latest_cmd);

        /* 2. encoder读取实际速度 */
        float v_left_actual  = encoder_get_speed(&enc_left, dt);
        float v_right_actual = encoder_get_speed(&enc_right, dt);

        /* 3. PID计算 */
        float pwm_left =
            pid_update(&pid_left,
                       target.left_mps,
                       v_left_actual,
                       dt);

        float pwm_right =
            pid_update(&pid_right,
                       target.right_mps,
                       v_right_actual,
                       dt);

        /* 4. 电机输出 */
        motor_set_pwm_left(pwm_left);
        motor_set_pwm_right(pwm_right);

        /* 5. 控制周期 */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/* =========================
   ROS2任务（先用模拟）
========================= */

static void ros_task(void *arg)
{
    (void)arg;

    while (1)
    {
        // TODO：这里以后换成 micro-ROS subscriber

        // 临时测试：让车前进
        latest_cmd.linear_x = 0.2f;
        latest_cmd.angular_z = 0.0f;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/* =========================
   ESP32入口
========================= */

void app_main(void)
{
    printf("System Init Start...\n");

    /* 1. PID初始化 */
    pid_init(&pid_left, 1.0f, 0.2f, 0.01f, 1000);
    pid_init(&pid_right, 1.0f, 0.2f, 0.01f, 1000);

    /* 2. encoder初始化（GPIO你后面改） */
    encoder_init(&enc_left, 4, 5);
    encoder_init(&enc_right, 18, 19);

    /* 3. motor初始化 */
    motor_init();

    /* 4. 创建任务 */
    xTaskCreate(control_task,
                "control_task",
                4096,
                NULL,
                10,
                NULL);

    xTaskCreate(ros_task,
                "ros_task",
                4096,
                NULL,
                5,
                NULL);

    printf("System Init Done\n");
}