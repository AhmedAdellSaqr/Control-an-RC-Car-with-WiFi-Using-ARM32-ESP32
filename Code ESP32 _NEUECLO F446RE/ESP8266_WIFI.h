/*
 * ESP8266_WIFI.h
 *
 *  Created on: Nov 5, 2024
 *      Author: Ahmed Adel
 */

#ifndef INC_ESP8266_WIFI_H_
#define INC_ESP8266_WIFI_H_
#include "stm32f4xx_hal.h"
#include "main.h"


void setServoPositions(uint16_t pos);
void control_car_wifi(char *command);
void start_ESP8266_WIFI(void);

#endif /* INC_ESP8266_WIFI_H_ */
