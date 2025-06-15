#include "exti.h"

volatile uint8_t paused_flag = 0;
volatile uint8_t reset_flag = 0;

void EXTI_Init(void) {
	RCC->APB2ENR |= 1 << 14;				// Bật clock SYSCFG

	SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4)); // Gán EXTI0 -> PA0
	SYSCFG->EXTICR[0] &= ~(0xF << (1 * 4));	// Gán EXTI1 -> PA1

	EXTI->IMR  |= (1 << 0) | (1 << 1);   	// Cho phép ngắt EXTI0 & EXTI1
	EXTI->FTSR |= (1 << 0) | (1 << 1);   	// Kích hoạt ngắt khi có sườn xuống

	NVIC_EnableIRQ(EXTI0_IRQn);				// Cho phép hoạt động trong NVIC
	NVIC_EnableIRQ(EXTI1_IRQn);				// Cho phép hoạt động trong NVIC
}

// Xử lý ngắt với stop button
void EXTI0_IRQHandler(void) {
	if(EXTI->PR & (1 << 0)) {
		paused_flag = 1;
		EXTI->PR |= (1 << 0);				// Xóa cờ ngắt
	}
}

// Xử lý ngắt với reset button
void EXTI1_IRQHandler(void) {
	if(EXTI->PR & (1 << 1)) {
		reset_flag = 1;
		EXTI->PR |= (1 << 1);				// Xóa cờ ngắt
	}
}
