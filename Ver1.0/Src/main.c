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
	GPIO_Init();
    I2C1_Init();
    LCD_Init();
    ADC1_Init();
    EXTI_Init();
    USART2_Init();

    uint8_t system_paused = 0, prev_system_paused = 0;
    uint16_t gas_ppm;

    LCD_Set_Cursor(0, 0);
    LCD_Send_String("W: ON  PPM: ");
    LCD_Set_Cursor(1, 0);
    LCD_Send_String("STATE: ");

    while(1) {
    	gas_ppm = MQ2_Read_PPM();

    	if(paused_flag) {
    	    paused_flag = 0;
    	    LCD_Set_Cursor(1, 7);
    	    LCD_Send_String("STOP    ");
    	    system_paused ^= 1;
    	}

    	if(reset_flag) {
    		gas_ppm = 0;
    	    reset_flag = 0;
    	    LCD_Update_PPM(gas_ppm);
    	    LCD_Set_Cursor(1, 7);
    	    LCD_Send_String("RESET   ");
    	    delay_ms(500);
    	    if(system_paused) {
    	    	LCD_Set_Cursor(1, 7);
    	    	LCD_Send_String("STOP    ");
    	    }
    	}

    	if(system_paused != prev_system_paused){
    	    LCD_Set_Cursor(0, 3);
    	    switch(system_paused) {
    	        case 1:
    	            LCD_Send_String("OFF");
    	            break;
    	        case 0:
    	            LCD_Send_String("ON ");
    	            break;
    	    }
    	    prev_system_paused = system_paused;
    	}

    	if(system_paused) {
    		System_Shutdown();

    		LCD_Update_PPM(gas_ppm);
    		delay_ms(300);
    	} else {
    		LCD_Update_PPM(gas_ppm);
    		LCD_Update_State(gas_ppm);
    		delay_ms(300);

    		if (gas_ppm <= SAFE_LEVEL) {
    			System_Safe();
    		} else if (gas_ppm <= ALERT_LEVEL) {
    			System_Alert();
    		} else if (gas_ppm <= DANGER_LEVEL) {
    		    System_Danger();

    		    char msg[32];
    		    sprintf(msg, "GAS DETECTED,%d\n", gas_ppm);
    		    USART2_SendString(msg);
    		} else {
    		    System_Critical(gas_ppm);
    		}
    	}
    }

}
