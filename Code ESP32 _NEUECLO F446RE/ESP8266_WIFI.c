#include <stdio.h>
#include <string.h>
#include "ESP8266_WIFI.h"
#include "motor.h"
#include "stepper.h"

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart3;
uint8_t rxBuffer[10] = {0};  // لتخزين البيانات المستلمة
extern uint16_t servoPosition;  // يتم تعريفه في ملف آخر كـمتغير عام

// لضبط زاوية السيرفو بين 45 و 100
void setServoPositions(uint16_t pos) {
    if (pos >= 45 && pos <= 100) {
        servoPosition = pos;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, servoPosition);
    }
}

// تنفيذ الأوامر بناءً على البيانات المستلمة من ESP8266
void control_car_wifi(char *command) {
    printf("Received command: %s\n", command);  // لطباعة الأمر للتصحيح

    if (strcasecmp(command, "FORWARD") == 0) {
        forward();  // تحرك للأمام
    } else if (strcasecmp(command, "BACKWARD") == 0) {
        Back();     // تحرك للخلف
    } else if (strcasecmp(command, "LEFT") == 0) {
        if (servoPosition + 5 <= 100) {
            setServoPositions(servoPosition + 5);  // دوران يسار
        }
    } else if (strcasecmp(command, "RIGHT") == 0) {
        if (servoPosition - 5 >= 45) {
            setServoPositions(servoPosition - 5);  // دوران يمين
        }
    } else if (strcasecmp(command, "STOP") == 0) {
        stop();     // توقف
    } else if (strcasecmp(command, "G") == 0) {
        forward();  // أمام مع يسار
        if (servoPosition + 5 <= 100) setServoPositions(servoPosition + 5);
    } else if (strcasecmp(command, "I") == 0) {
        forward();  // أمام مع يمين
        if (servoPosition - 5 >= 45) setServoPositions(servoPosition - 5);
    } else if (strcasecmp(command, "H") == 0) {
        Back();     // خلف مع يسار
        if (servoPosition + 5 <= 100) setServoPositions(servoPosition + 5);
    } else if (strcasecmp(command, "J") == 0) {
        Back();     // خلف مع يمين
        if (servoPosition - 5 >= 45) setServoPositions(servoPosition - 5);
    } else {
        printf("Unknown command: %s\n", command);  // أمر غير معروف
    }
}

// دالة البدء التي تقرأ من UART وتوجه الأوامر
void start_ESP8266_WIFI(void) {
    if (HAL_UART_Receive(&huart3, rxBuffer, sizeof(rxBuffer) - 1, HAL_MAX_DELAY) == HAL_OK) {
        // إزالة \r أو \n من نهاية الأمر
        rxBuffer[strcspn((char *)rxBuffer, "\r\n")] = '\0';
        control_car_wifi((char *)rxBuffer);  // تنفيذ الأمر
        memset(rxBuffer, 0, sizeof(rxBuffer));  // تنظيف البوفر
    } else {
        printf("Error: Failed to receive data from ESP8266\n");
    }
}


/*

#include <stdio.h>
#include <string.h>
#include "ESP8266_WIFI.h"
#include "motor.h"
#include "stepper.h"

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart3;
uint8_t rxBuffer[10] = {0};  // تهيئة المخزن المؤقت
//uint16_t servoPosition = 75; // الوضع الافتراضي للسيرفو
extern uint16_t servoPosition;

void setServoPositions(uint16_t pos) {
    if (pos >= 45 && pos <= 100) {
        servoPosition = pos;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, servoPosition);
    }
}

void control_car_wifi(char *command) {
    printf("Received command: %s\n", command);  // طباعة الأمر المستلم للتصحيح

    if (strcasecmp(command, "FORWARD") == 0) {
        forward();
    } else if (strcasecmp(command, "BACKWARD") == 0) {
        Back();
    } else if (strcasecmp(command, "LEFT") == 0) {
        if (servoPosition + 5 <= 100) setServoPositions(servoPosition + 5);
    } else if (strcasecmp(command, "RIGHT") == 0) {
        if (servoPosition - 5 >= 45) setServoPositions(servoPosition - 5);
    } else if (strcasecmp(command, "STOP") == 0) {
        stop();
    } else if (strcasecmp(command, "G") == 0) {
        forward();
        if (servoPosition + 5 <= 100) setServoPositions(servoPosition + 5);
    } else if (strcasecmp(command, "I") == 0) {
        forward();
        if (servoPosition - 5 >= 45) setServoPositions(servoPosition - 5);
    } else if (strcasecmp(command, "H") == 0) {
        Back();
        if (servoPosition + 5 <= 100) setServoPositions(servoPosition + 5);
    } else if (strcasecmp(command, "J") == 0) {
        Back();
        if (servoPosition - 5 >= 45) setServoPositions(servoPosition - 5);
    } else {
        printf("Unknown command: %s\n", command);
    }
}

void start_ESP8266_WIFI(void) {
    if (HAL_UART_Receive(&huart3, rxBuffer, sizeof(rxBuffer) - 1, HAL_MAX_DELAY) == HAL_OK) {
        rxBuffer[strcspn((char *)rxBuffer, "\r\n")] = '\0';  // إزالة أي أحرف `\r\n`
        control_car_wifi((char *)rxBuffer);
        memset(rxBuffer, 0, sizeof(rxBuffer));  // تنظيف المخزن المؤقت بعد الاستخدام
    } else {
        printf("Error: Failed to receive data from ESP8266\n");
    }
}


*/















































/**********************************************************************************************************/
/*
#include <stdio.h>
#include <string.h>
#include "ESP8266_WIFI.h"
#include "motor.h"
#include "stepper.h"

//extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart3;
uint8_t rxBuffer[10];
extern servoPosition;

void forward(void);
void Back(void);
void stop(void);
void start_ESP8266_WIFI(void)
{
	if (HAL_UART_Receive(&huart3, (uint8_t*)rxBuffer, sizeof(rxBuffer) - 1, HAL_MAX_DELAY) == HAL_OK) {
		rxBuffer[sizeof(rxBuffer) - 1] = '\0';  // Ensure null-terminated string

		if (strstr((const char *)rxBuffer, "FORWARD") != NULL) {
			MOTOR__CW();
		}else if (strstr((const char *)rxBuffer, "BACKWARD") != NULL) {
			MOTOR__CCW();
		}
		else if (strstr((const char *)rxBuffer, "LEFT") != NULL) {
			while (HAL_UART_Receive(&huart3, (uint8_t*)rxBuffer, sizeof(rxBuffer) - 1, 100) == HAL_OK &&
					strstr((const char *)rxBuffer, "LEFT") != NULL) {
				stepper_move(1, 0, 2); // خطوة واحدة عكس عقارب الساعة
			}
		} else if (strstr((const char *)rxBuffer, "RIGHT") != NULL) {
			while (HAL_UART_Receive(&huart3, (uint8_t*)rxBuffer, sizeof(rxBuffer) - 1, 100) == HAL_OK &&
					strstr((const char *)rxBuffer, "RIGHT") != NULL) {
				stepper_move(1, 1, 2); // خطوة واحدة مع عقارب الساعة
			}
		}else if (strstr((const char *)rxBuffer, "STOP") != NULL) {
			stop();
		}else {
            stopStepper();  // عند عدم وجود ضغط على أي زر، يتوقف المحرك
        }


		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, servoPosition);
		// Clear buffer
		memset(rxBuffer, 0, sizeof(rxBuffer));
	}
}*/
/**********************************************************************************************************/
/*void start_ESP8266_WIFI(void)
{
	if (HAL_UART_Receive(&huart3, (uint8_t*)rxBuffer, sizeof(rxBuffer) - 1, HAL_MAX_DELAY) == HAL_OK) {
		rxBuffer[sizeof(rxBuffer) - 1] = '\0';  // Ensure null-terminated string

		if (strstr(rxBuffer, "OPEN") != NULL) {
			// Code to open the hand
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);  // Example pin control
		} else if (strstr(rxBuffer, "CLOSE") != NULL) {
			// Code to close the hand
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);  // Example pin control
		}

		// Clear buffer
		memset(rxBuffer, 0, sizeof(rxBuffer));
	}
}*/
/*void start_ESP8266_WIFI(void)
{
	if (HAL_UART_Receive(&huart3, (uint8_t*)rxBuffer, sizeof(rxBuffer) - 1, HAL_MAX_DELAY) == HAL_OK) {
		rxBuffer[sizeof(rxBuffer) - 1] = '\0';  // Ensure null-terminated string

		if (strstr((const char *)rxBuffer, "FORWARD") != NULL) {
			MOTOR__CW();
		}else if (strstr((const char *)rxBuffer, "BACKWARD") != NULL) {
			MOTOR__CCW();
		}
		else if (strstr((const char *)rxBuffer, "LEFT") != NULL) {
			servoPosition -= 6; // Decrement position
			if (servoPosition < 25) { // Limit to 0°
				servoPosition = 25;
			}

		}else if (strstr((const char *)rxBuffer, "RIGHT") != NULL) {
			servoPosition += 6; // Increment position
			if (servoPosition > 125) { // Limit to 180°
				servoPosition = 125;}
			// Code to close the hand
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);  // Example pin control
		}else if (strstr((const char *)rxBuffer, "STOP") != NULL) {
			stop();
		}


		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, servoPosition);
		// Clear buffer
		memset(rxBuffer, 0, sizeof(rxBuffer));
	}*/

