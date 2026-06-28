#ifndef PID_H
#define PID_H

// PID控制器结构体
typedef struct {
    float kp;
    float ki;
    float kd;

    float integral;
    float last_error;

    float output_limit;   // 防止PWM爆炸
} pid_controller_t;

// 初始化PID
void pid_init(pid_controller_t *pid, float kp, float ki, float kd, float limit);

// 更新PID输出，dt 是控制周期，单位秒
float pid_update(pid_controller_t *pid, float target, float actual, float dt);

#endif
