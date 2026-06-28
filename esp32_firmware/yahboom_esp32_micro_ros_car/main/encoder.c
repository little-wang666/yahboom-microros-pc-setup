#include "encoder.h"

#define FILTER_ALPHA 0.8f

void encoder_init(encoder_t *enc,
                  gpio_num_t pin_a,
                  gpio_num_t pin_b)
{
    enc->pulse_count = 0;
    enc->speed = 0;
    enc->last_speed = 0;

    enc->pin_a = pin_a;
    enc->pin_b = pin_b;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin_a),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);

    gpio_isr_handler_add(pin_a,
                          encoder_isr_handler,
                          (void*) enc);
}

void IRAM_ATTR encoder_isr_handler(void *arg)
{
    encoder_t *enc = (encoder_t *)arg;
    enc->pulse_count++;
}

float encoder_get_speed(encoder_t *enc, float dt)
{
    int pulses = enc->pulse_count;
    enc->pulse_count = 0;

    float rev_per_sec = pulses / dt;

    enc->speed =
        FILTER_ALPHA * enc->last_speed +
        (1 - FILTER_ALPHA) * rev_per_sec;

    enc->last_speed = enc->speed;

    return enc->speed;
}