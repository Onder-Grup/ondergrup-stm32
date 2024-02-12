#ifndef INC_ESPDATALOGGER_H_
#define INC_ESPDATALOGGER_H_

#include "stm32f1xx_hal.h"

#include "stdio.h"
#include "string.h"

void sendATCommand(UART_HandleTypeDef *huart, const char *cmd, uint16_t delayAfter);

void ESP8266_Init(UART_HandleTypeDef *huart, const char *wifiSS, const char *wifiPA);
void sendMachineData(UART_HandleTypeDef *huart, const char *machineID, const char *machineData);
int checkMachineID(UART_HandleTypeDef *huart1, const char *machineID);

#endif /* INC_ESPDATALOGGER_H_ */
