#include "adc.h"
#include <math.h>

volatile uint16_t adc_result = 0, latest_ppm = 0;
volatile uint8_t new_adc_ready = 0;

void ADC1_Init(void) {
    RCC->AHB1ENR |= 1 << 1;             // Bật clock GPIOB
    RCC->APB2ENR |= 1 << 8;             // Bật clock ADC1

    GPIOB->MODER &= ~(3 << (0 * 2));
    GPIOB->MODER |=  (3 << (0 * 2));    // PB0: analog mode

    ADC1->CR2 = 0;
    ADC1->SQR3 = 8;                     // Kênh 8 (PB0)
    ADC1->SMPR2 |= (7 << (8 * 3));      // Sampling time tối đa

    ADC1->CR1 |= 1 << 5;                // Bật EOC interrupt
    NVIC_EnableIRQ(ADC_IRQn);           // Cho phép ngắt ADC trong NVIC

    ADC1->CR2 |= 1 << 0;                // Bật ADC
}

void ADC1_Start(void) {
    ADC1->CR2 |= 1 << 30;               // Khởi động chuyển đổi
}

void ADC_IRQHandler(void) {
    if (ADC1->SR & ADC_SR_EOC) {
        adc_result = ADC1->DR;
        latest_ppm = MQ2_Read_PPM();
        new_adc_ready = 1;
    }
}

uint16_t MQ2_Read_PPM(void) {
    float voltage = (adc_result / 4095.0f) * VREF;
    float Rs = (VREF - voltage) * RL / voltage;
    float ratio = Rs / R0;

    float ppm = pow(10.0f, (log10f(ratio) - B) / A);

    if (ppm < 0) ppm = 0;
    if (ppm > 4000) ppm = 4000;

    return (uint16_t)(ppm + 0.5f);
}
