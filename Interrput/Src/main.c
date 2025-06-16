#include "stm32f4xx.h"
#include "gpio.h"
#include "adc.h"
#include "timer.h"
#include "i2c.h"
#include "lcd.h"
#include "exti.h"
#include "uart.h"
#include <stdio.h>

int main(void) {
	TIM2_Init();
	TIM3_Init();
	GPIO_Init();
    I2C1_Init();
    LCD_Init();
    ADC1_Init();
    EXTI_Init();
    USART2_Init();

    uint8_t lcd_update_flag = 0, system_paused = 0;

    LCD_Set_Cursor(0, 0);
    LCD_Send_String("W: ON  PPM: ");
    LCD_Set_Cursor(1, 0);
    LCD_Send_String("STATE: ");

    while (1) {
    	if (paused_flag) {
    	    paused_flag = 0;
    	    system_paused ^= 1;

    	    LCD_Set_Cursor(0, 3);
    	    if (system_paused)
    	    	LCD_Send_String("OFF");
    	    else
    	    	LCD_Send_String("ON ");
    	}

    	if (reset_flag) {
    	    reset_flag = 0;
    	    latest_ppm = 0;
    	    lcd_update_flag = 1;

    	    LCD_Update_PPM(latest_ppm);
    	    LCD_Set_Cursor(1, 7);
    	    LCD_Send_String("RESET   ");
    	    delay_ms(500);
    	}

    	if (new_adc_ready) {
    	    new_adc_ready = 0;
    	    lcd_update_flag = 1;

    	    if (!system_paused) {
    	        if (latest_ppm <= SAFE_LEVEL) {
    	            System_Safe();
    	        } else if (latest_ppm <= ALERT_LEVEL) {
    	            System_Alert();
    	        } else if (latest_ppm <= DANGER_LEVEL) {
    	            System_Danger();

    	            char msg[32];
    	            sprintf(msg, "GAS DETECTED,%d\n", latest_ppm);
    	            USART2_SendString(msg);
    	        } else {
    	            System_Critical(latest_ppm);
    	        }
    	    } else {
    	        System_Shutdown();
    	    }
    	}

    	if (lcd_update_flag) {
    	    lcd_update_flag = 0;

    	    LCD_Update_PPM(latest_ppm);

    	    if(system_paused) {
    	        LCD_Set_Cursor(1, 7);
    	        LCD_Send_String("STOP    ");
    	    } else {
    	    	LCD_Update_State(latest_ppm);
    	    }
    	}
    }
}
