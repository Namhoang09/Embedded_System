#include "adc.h"
#include <math.h>

void ADC1_Init(void) {
	RCC->AHB1ENR |= 1 << 1;				// Bật clock GPIOB
    RCC->APB2ENR |= 1 << 8;				// Bật clock ADC1

    // Analog mode
    GPIOB->MODER &= ~(3 << (0 * 2));
    GPIOB->MODER |= (3 << (0 * 2));     // PB0: MQ2

    ADC1->CR2 = 0;                      // Reset CR2
    ADC1->SQR3 = 8;                     // Channel 8 (PB0)
    ADC1->SMPR2 |= (7 << (8 * 3)); 		// Chọn thời gian lấy mẫu dài nhất
    ADC1->CR2 |= 1 << 0;           		// Bật ADC1
}

uint16_t ADC1_Read(void) {
    ADC1->CR2 |= 1 << 30;        		// Bắt đầu chuyển đôi
    while (!(ADC1->SR & 1 << 1));    	// Chờ EOC (chuyển đổi xong)
    return ADC1->DR;					// Trả về kết quả 12-bit
}

uint16_t MQ2_Read_PPM(void) {
	uint16_t adc_value = ADC1_Read();				// Đọc giá trị ADC
    float voltage = (adc_value / 4095.0f) * VREF;	// Chuyển ADC sang điện áp
    float Rs = (VREF - voltage) * RL / voltage;		// Tính điện trở cảm biến qua mạch phân áp
    float ratio = Rs / R0;

    // Công thức gần đúng từ đồ thị LPG trong datasheet MQ-2:
    // log(Rs/R0) = A * log(ppm) + B
    float ppm = pow(10.0f, (log10f(ratio) - B) / A);

    // Quy định giá trị biên
    if(ppm < 0) ppm = 0;
    if(ppm > 4000) ppm = 4000;

    return (uint16_t)(ppm + 0.5f);					// Trả kết quả là số nguyên
}
