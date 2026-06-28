#include "kinematics.h"

// 轮距（你后面要标定）
#define WHEEL_BASE 0.15f

wheel_velocity_t compute_wheel_velocity(float v, float w)
{
    wheel_velocity_t vel;

    vel.left  = v - (w * WHEEL_BASE / 2.0f);
    vel.right = v + (w * WHEEL_BASE / 2.0f);

    return vel;
}