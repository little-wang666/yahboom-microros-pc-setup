#include "pid.h"

/**
 * @brief PID初始化
 */
void pid_init(pid_controller_t *pid,
              float kp, float ki, float kd,
              float limit)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->integral = 0.0f;
    pid->last_error = 0.0f;

    pid->output_limit = limit;
}

/**
 * @brief PID更新函数（带dt版本，工程标准）
 * @param target 目标值（速度）
 * @param actual 实际值（编码器速度）
 * @param dt 控制周期（秒，例如0.01 = 10ms）
 */
float pid_update(pid_controller_t *pid,
                 float target,
                 float actual,
                 float dt)
{
    // =========================
    // 1. 计算误差
    // =========================
    float error = target - actual;

    // =========================
    // 2. 积分项（带dt）
    // =========================
    pid->integral += error * dt;

    // ---- anti-windup（积分限幅）----
    if (pid->integral > 1000.0f)
        pid->integral = 1000.0f;

    if (pid->integral < -1000.0f)
        pid->integral = -1000.0f;

    // =========================
    // 3. 微分项（带dt）
    // =========================
    float derivative = (error - pid->last_error) / dt;

    // 更新误差
    pid->last_error = error;

    // =========================
    // 4. PID输出
    // =========================
    float output =
        pid->kp * error +
        pid->ki * pid->integral +
        pid->kd * derivative;

    // =========================
    // 5. 输出限幅（防止PWM爆炸）
    // =========================
    if (output > pid->output_limit)
        output = pid->output_limit;

    if (output < -pid->output_limit)
        output = -pid->output_limit;

    return output;
}