#include "lcd.h"
#include "i2c.h"
#include "timer.h"
#include <stdio.h>

void LCD_Send_Cmd(uint8_t cmd) {
    LCD_Send_Byte(cmd, 0);			// Gửi lệnh
}

void LCD_Send_Data(uint8_t data) {
    LCD_Send_Byte(data, 1);			// Gửi dữ liệu
}

void LCD_Init(void) {
    delay_ms(50);					// Đợi LCD khởi động sau cấp nguồn
    LCD_Send_Cmd(0x33);				// Đưa về chế độ 8-bit (khởi động lần 1)
    LCD_Send_Cmd(0x32);				// Chuyển về chế độ 4-bit (khởi động lần 2)
    LCD_Send_Cmd(0x28); 			// 4-bit, 2 dòng, font 5x8
    LCD_Send_Cmd(0x0C); 			// Bật hiển thị, tắt con trỏ
    LCD_Send_Cmd(0x06); 			// Tự động tăng con trỏ, không dịch màn hình
    LCD_Send_Cmd(0x01); 			// Xóa màn hình
    delay_ms(2);					// Đợi xóa xong
}

// Đặt vị trí con trỏ
void LCD_Set_Cursor(uint8_t row, uint8_t col) {
	// Dòng 1 bắt đầu tại địa chỉ 0x80
	// Dòng 2 bắt đầu tại địa chỉ 0xC0
    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
    LCD_Send_Cmd(addr);
}

void LCD_Send_String(char *str) {
    while (*str) LCD_Send_Data(*str++);
}

void LCD_Update_State(uint16_t gas_ppm) {
    LCD_Set_Cursor(1, 7);			// Đặt vị trí con trỏ tại dòng 2 cột 7
    char buffer[10];
    sprintf(buffer, "%-5d", gas_ppm);
    LCD_Send_String(buffer);

//    if (gas_ppm <= SAFE_LEVEL) {
//        LCD_Send_String("SAFE    ");
//    } else if (gas_ppm <= ALERT_LEVEL) {
//        LCD_Send_String("ALERT   ");
//    } else if (gas_ppm <= DANGER_LEVEL) {
//        LCD_Send_String("DANGER  ");
//    } else {
//        LCD_Send_String("CRITICAL");
//    }
}
