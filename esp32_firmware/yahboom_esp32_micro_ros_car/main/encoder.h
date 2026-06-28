#ifndef ENCODER_H
#define ENCODER_H

#include "driver/gpio.h"
#include "esp_attr.h"

typedef struct {

    volatile int pulse_count;

    float speed;
    float last_speed;

    gpio_num_t pin_a;
    gpio_num_t pin_b;

} encoder_t;

void encoder_init(encoder_t *enc,
                  gpio_num_t pin_a,
                  gpio_num_t pin_b);

void IRAM_ATTR encoder_isr_handler(void *arg);

float encoder_get_speed(encoder_t *enc, float dt);

#endif