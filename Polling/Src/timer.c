#include "timer.h"

void TIM2_Init(void) {
    RCC->APB1ENR |= (1 << 0);  			// Bật clock TIM2

    TIM2->PSC = 16000 - 1;				// 16MHz/16000 = 1kHz
    TIM2->CR1 = 0;             			// Reset TIM2
    TIM2->EGR |= (1 << 0);     			// Cập nhật ngay giá trị PSC mới
}

// Tạo hàm delay
void delay_ms(uint32_t ms) {
    TIM2->ARR = ms - 1;           		// Thời gian đến khi tràn
    TIM2->CNT = 0;            			// Đặt giá trị bộ đếm về 0
    TIM2->SR &= ~(1 << 0);    			// Xóa cờ tràn trước khi bắt đầu
    TIM2->CR1 |= (1 << 0);    			// Cho phép TIM2 bắt đầu đếm

    while ((TIM2->SR & (1 << 0)) == 0); // Chờ đến khi UIF = 1 (tràn)
    TIM2->SR &= ~(1 << 0);    			// Xóa cờ UIF để chuẩn bị lần tiếp theo
    TIM2->CR1 &= ~(1 << 0);   			// Tắt timer tránh tốn năng lượng
}
