#ifndef MOTOR_H
#define MOTOR_H

void motor_init(void);

void motor_set_pwm_left(float pwm);
void motor_set_pwm_right(float pwm);

#endif