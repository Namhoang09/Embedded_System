#include "i2c.h"
#include "timer.h"

void I2C1_Init(void) {
	RCC->AHB1ENR |= 1 << 1;				// Bật clock GPIOB
    RCC->APB1ENR |= 1 << 21;			// Bật clock I2C1

    // AF
    GPIOB->MODER &= ~(3 << (SCL * 2));
    GPIOB->MODER |= 2 << (SCL * 2);		// PB6: SCL
    GPIOB->OTYPER |= 1 << SCL;			// Open-drain
    GPIOB->AFR[0] |= 4 << (SCL * 4);	// AF4: I2C mode
    GPIOB->OSPEEDR |= (3 << (6 * 2));	// High speed đảm bảo I2C hoạt động ổn định
    GPIOB->MODER &= ~(3 << (SDA * 2));
    GPIOB->MODER |= 2 << (SDA * 2);		// PB7: SDA
    GPIOB->OTYPER |= 1 << SDA;			// Open-drain
    GPIOB->OSPEEDR |= (3 << (7 * 2));	// High speed đảm bảo I2C hoạt động ổn định
    GPIOB->AFR[0] |= 4 << (SDA * 4);	// AF4: I2C mode

    I2C1->CR2 = 16;     				// Tần số PCLK1 = 16 MHz
    I2C1->CCR = 80;     				// Chu kỳ clock cho I2C (16MHz/(2*100kHz))
    I2C1->TRISE = 17;					// Trise = 1000ns/T_PCLK + 1 = 1000ns/(1/16MHz) + 1
    I2C1->CR1 |= 1 << 0;				// Bật I2C1
}

// Gửi 1 byte dữ liệu
void I2C1_Write(uint8_t data) {
	while (!(I2C1->SR1 & (1 << 7)));  	// Chờ TXE = 1: thanh ghi DR trống
	I2C1->DR = data;                    // Gửi data
	while (!(I2C1->SR1 & (1 << 2))); 	// Chờ truyền xong hoàn toàn
}

// Gửi 1 byte lệnh đến LCD
void LCD_Send_Byte(uint8_t data, uint8_t rs) {
    uint8_t upper = data & 0xF0;		// 4 bit cao
    uint8_t lower = (data << 4) & 0xF0;	// 4 bit thấp
    uint8_t data_arr[4] = {
        upper | 0x0C | rs,				// Enable = 1, Backlight = 1, RS = x
        upper | 0x08 | rs,				// Enable = 0
        lower | 0x0C | rs,
        lower | 0x08 | rs
    };

    // Giao tiếp với LCD
    I2C1->CR1 |= (1 << 8);				// Bắt đầu I2C
    while (!(I2C1->SR1 & (1 << 0)));	// Đợi gửi START xong
    I2C1->DR = SLAVE_ADDR_LCD;			// Gửi địa chỉ LCD
    while (!(I2C1->SR1 & (1 << 1)));	// Chờ phản hồi ACK
    (void)I2C1->SR2;					// Đọc SR2 để clear ADDR

    for (int i = 0; i < 4; i++) {
        I2C1_Write(data_arr[i]);		// Gửi từng byte
    }

    I2C1->CR1 |= (1 << 9);				//  Kết thúc truyền
    delay_ms(1);
}
