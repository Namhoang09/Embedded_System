#include "uart.h"

void USART2_Init(void) {
	RCC->AHB1ENR |= 1 << 0; 			// Bật clock GPIOA
	RCC->APB1ENR |= 1 << 17; 			// Bật clock USART2

	// AF
	GPIOA->MODER &= ~(3 << (2 * 2));
	GPIOA->MODER |= 2 << (2 * 2);		// PA2: TX

	GPIOA->AFR[0] &= ~(0xF << (2 * 4));
	GPIOA->AFR[0] |= 7 << (2 * 4);		// AF7: USART mode

	USART2->BRR = 0x0683; 				// Baud rate = 9600 (16M/(16*9600) = 104.166 = 0x0683
	USART2->CR1 |= 1 << 3; 				// Cho phép truyền dữ liệu
	USART2->CR1 |= 1 << 13; 			// Bật USART2
}

void USART2_SendChar(char c) {
	while (!(USART2->SR & (1 << 7))); 	// Chờ thanh ghi truyền trống
	USART2->DR = c; 					// Viết dữ liệu cần gửi
}

void USART2_SendString(const char *str) {
	while (*str) {
		USART2_SendChar(*str++); 		// Gửi từng chữ trong chuỗi
	}
}
