#include "motor.h"
#include "driver/ledc.h"

#define PWM_MAX 1000

void motor_init(void)
{
    // LEDC PWM初始化（ESP32标准方式）
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000,
    };
    ledc_timer_config(&timer);
}

void motor_set_pwm_left(float pwm)
{
    if (pwm > PWM_MAX) pwm = PWM_MAX;
    if (pwm < -PWM_MAX) pwm = -PWM_MAX;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (int)pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void motor_set_pwm_right(float pwm)
{
    if (pwm > PWM_MAX) pwm = PWM_MAX;
    if (pwm < -PWM_MAX) pwm = -PWM_MAX;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, (int)pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}