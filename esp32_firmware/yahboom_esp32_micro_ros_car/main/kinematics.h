#ifndef KINEMATICS_H
#define KINEMATICS_H

// 左右轮速度结构体
typedef struct {
    float left;
    float right;
} wheel_velocity_t;

// 输入：v,w  输出：左右轮速度
wheel_velocity_t compute_wheel_velocity(float v, float w);

#endif