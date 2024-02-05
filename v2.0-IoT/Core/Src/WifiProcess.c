/*
 * WifiProcess.c
 *
 *  Created on: Feb 5, 2024
 *      Author: hidirektor
 */

#include "WifiProcess.h"
#include "main.h"
#include "GlobalVariables.h"
#include "ESP8266.h"
#include "i2c-lcd.h"

void takeMachineID(int state) {
	mainSection:
	lcd_cursor(1);

    int cursorPosition = 3;
    int machineIDLoc = 0;
    int writeLoc = 5;

    if(state == 0) {
    	memset(machineID, 0, sizeof(machineID));
    }
    HAL_Delay(100);

    printTemplate(1, 0);

    while (1) {
        if (HAL_GPIO_ReadPin(butonEnterIn_GPIO_Port, butonEnterIn_Pin) == 1) {
        	lcd_cursor(0);

        	if(machineID[11] == '\0') {
        		lcd_clear();
        		lcd_print(1, 1, " ID 12 KARAKTER ");
        		lcd_print(2, 1, " OLMAK ZORUNDA! ");
        		HAL_Delay(1200);
        		goto mainSection;
        	}

        	/*if(checkMachineID(&huart1, machineID) != 1) {
        		lcd_clear();
        		lcd_print(1, 1, "BU ID ILE MAKINE");
        		lcd_print(2, 1, "OLUSTURAMAZSINIZ");
        		HAL_Delay(1200);
        		goto mainSection;
        	} else {
        		eepromData[49] = 1;
        	}*/

        	memcpy(&eepromData[idStartPos], machineID, 12);
        	HAL_Delay(200);

        	HAL_I2C_Mem_Write(&hi2c1, 0xA0, 0, 110, eepromData, 110, 3000);
        	HAL_Delay(500);

            break;
        }

        if (HAL_GPIO_ReadPin(butonIleriIn_GPIO_Port, butonIleriIn_Pin) == 1) {
        	if(cursorPosition == 7) {
        		cursorPosition = 10;
        	} else if (cursorPosition == 14) {
                cursorPosition = 3;
            } else {
            	cursorPosition++;
            }

        	HAL_Delay(350);
        }

        if (HAL_GPIO_ReadPin(butonGeriIn_GPIO_Port, butonGeriIn_Pin) == 1) {
            if (cursorPosition == 3) {
                cursorPosition = 14;
            } else if(cursorPosition == 10) {
            	cursorPosition = 7;
            } else {
            	cursorPosition--;
            }

            HAL_Delay(350);
        }

        if (HAL_GPIO_ReadPin(butonYukariIn_GPIO_Port, butonYukariIn_Pin) == 1) {
        	if(cursorPosition == 3) {
        		machineID[machineIDLoc] = '0';
        	} else if(cursorPosition == 4) {
        		machineID[machineIDLoc] = '1';
        	} else if(cursorPosition == 5) {
        		machineID[machineIDLoc] = '2';
        	} else if(cursorPosition == 6) {
        		machineID[machineIDLoc] = '3';
        	} else if(cursorPosition == 7) {
        		machineID[machineIDLoc] = '4';
        	} else if(cursorPosition == 10) {
        		machineID[machineIDLoc] = '5';
        	} else if(cursorPosition == 11) {
        		machineID[machineIDLoc] = '6';
        	} else if(cursorPosition == 12) {
        		machineID[machineIDLoc] = '7';
        	} else if(cursorPosition == 13) {
        		machineID[machineIDLoc] = '8';
        	} else if(cursorPosition == 14) {
        		machineID[machineIDLoc] = '9';
        	}

        	lcd_print_char(1, writeLoc, machineID[machineIDLoc]);

        	writeLoc++;
        	machineIDLoc++;

        	HAL_Delay(450);
        }

        if(HAL_GPIO_ReadPin(butonAsagiIn_GPIO_Port, butonAsagiIn_Pin) == 1) {
            if(strlen(machineID) >= 1) {

                machineID[machineIDLoc] = '\0';

                lcd_delete_char(1, 4+machineIDLoc);
                HAL_Delay(50);

                if(writeLoc > 5) {
                	writeLoc--;
                } else if(writeLoc < 5) {
                	writeLoc = 5;
                }

                if(machineIDLoc > 0) {
                	machineIDLoc--;
                } else if(machineIDLoc < 0) {
                	machineIDLoc = 0;
                }
            }

            HAL_Delay(350);
        }

        lcd_gotoxy(2, cursorPosition);
    }
}

void takeWifiSSID(int state) {
	mainSSIDSection:
    lcd_cursor(1);

    if(state == 0) {
    	memset(wifiSSID, 0, sizeof(wifiSSID));
    }

    int realCharPos = 1;
    cursorPosition = 1;
    page = 1;
    int wifiNameLoc = 0;
    int writeLoc = 7;

    printTemplate(2, 1);

    while (1) {
        if (HAL_GPIO_ReadPin(butonEnterIn_GPIO_Port, butonEnterIn_Pin) == 1) {
            lcd_cursor(0);

            if(strlen(wifiSSID) > wifiCharacterLimit) {
                lcd_clear();
                lcd_print(1, 1, " 20 KARAKTERDEN ");
                lcd_print(2, 1, "FAZLA SSID OLMAZ");
                HAL_Delay(1200);
                goto mainSSIDSection;
            }

            memcpy(&eepromData[ssidStartPos], wifiSSIDLoc, wifiCharacterLimit);
            HAL_Delay(250);
            //memcpy(&eepromData[ssidStartPos], (uint8_t *)wifiSSID, strlen(wifiSSID));

            HAL_I2C_Mem_Write(&hi2c1, 0xA0, 0, 110, eepromData, 110, 3000);
            HAL_Delay(500);

            break;
        }

        if (HAL_GPIO_ReadPin(butonIleriIn_GPIO_Port, butonIleriIn_Pin) == 1) {
        	realCharPos++;
        	if(realCharPos > 80) {
        		realCharPos = 1;
        	}
            if (cursorPosition == 16) {
            	if(page == 1) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(2, 2);
            	} else if(page == 2) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(2, 3);
            	} else if(page == 3) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(2, 4);
            	} else if(page == 4) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(2, 5);
            	} else if(page == 5) {
            		cursorPosition = 1;
            		page = 1;
            		printTemplate(2, 1);
            	}
            } else {
            	cursorPosition++;
            }

            HAL_Delay(250);
        }

        if (HAL_GPIO_ReadPin(butonGeriIn_GPIO_Port, butonGeriIn_Pin) == 1) {
        	realCharPos--;
        	if(realCharPos < 1) {
        		realCharPos = 80;
        	}
            if(cursorPosition == 1) {
            	if(page == 1) {
            		cursorPosition = 16;
            		page = 5;
            		printTemplate(2, 5);
            	} else if(page == 2) {
            		cursorPosition = 16;
            		page = 1;
            		printTemplate(2, 1);
            	} else if(page == 3) {
            		cursorPosition = 16;
            		page = 2;
            		printTemplate(2, 2);
            	} else if(page == 4) {
            		cursorPosition = 16;
            		page = 3;
            		printTemplate(2, 3);
            	} else if(page == 5) {
            		cursorPosition = 16;
            		page = 4;
            		printTemplate(2, 4);
            	}
            } else {
            	cursorPosition--;
            }

            HAL_Delay(250);
        }

        if (HAL_GPIO_ReadPin(butonYukariIn_GPIO_Port, butonYukariIn_Pin) == 1) {
            wifiSSID[wifiNameLoc] = getCharFromCursorPosition(realCharPos - 1);
            wifiSSIDLoc[wifiNameLoc] = realCharPos - 1;

            lcd_print_char(1, writeLoc, wifiSSID[wifiNameLoc]);

            writeLoc++;
            wifiNameLoc++;

            HAL_Delay(250);
        }

        if(HAL_GPIO_ReadPin(butonAsagiIn_GPIO_Port, butonAsagiIn_Pin) == 1) {
        	if(strlen(wifiSSID) >= 1) {
        		wifiSSID[wifiNameLoc] = '\0';
        		wifiSSIDLoc[wifiNameLoc] = '\0';

        		lcd_delete_char(1, 6+wifiNameLoc);
        		HAL_Delay(50);
        		if(writeLoc > 7) {
        			writeLoc--;
        		} else if(writeLoc < 7) {
        			writeLoc = 7;
        		}

        		if(wifiNameLoc > 0) {
        			wifiNameLoc--;
        		} else if(wifiNameLoc < 0) {
        			wifiNameLoc = 0;
        		}
        	}

        	HAL_Delay(250);
        }

        lcd_gotoxy(2, cursorPosition);
    }
}

void takeWifiPass(int state) {
	mainPASSSection:
    lcd_cursor(1);

    if(state == 0) {
    	memset(wifiPass, 0, sizeof(wifiPass));
    }

    int realCharPos = 1;
    cursorPosition = 1;
    page = 1;
    int wifiPassLoc = 0;
    int writeLoc = 7;

    printTemplate(3, 1);

    while (1) {
        if (HAL_GPIO_ReadPin(butonEnterIn_GPIO_Port, butonEnterIn_Pin) == 1) {
            lcd_cursor(0);

            if(strlen(wifiPass) > wifiCharacterLimit) {
                lcd_clear();
                lcd_print(1, 1, " 20 KARAKTERDEN ");
                lcd_print(2, 1, "FAZLA PASS OLMAZ");
                HAL_Delay(1200);
                goto mainPASSSection;
            }

            memcpy(&eepromData[passStartPos], wifiPassLocArr, wifiCharacterLimit);
            HAL_Delay(250);

            HAL_I2C_Mem_Write(&hi2c1, 0xA0, 0, 110, eepromData, 110, 3000);
            HAL_Delay(500);

            break;
        }

        if (HAL_GPIO_ReadPin(butonIleriIn_GPIO_Port, butonIleriIn_Pin) == 1) {
        	realCharPos++;
        	if(realCharPos > 80) {
        	    realCharPos = 1;
        	}
            if (cursorPosition == 16) {
            	if(page == 1) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(3, 2);
            	} else if(page == 2) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(3, 3);
            	} else if(page == 3) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(3, 4);
            	} else if(page == 4) {
            		cursorPosition = 1;
            		page++;
            		printTemplate(3, 5);
            	} else if(page == 5) {
            		cursorPosition = 1;
            		page = 1;
            		printTemplate(3, 1);
            	}
            } else {
            	cursorPosition++;
            }

            HAL_Delay(250);
        }

        if (HAL_GPIO_ReadPin(butonGeriIn_GPIO_Port, butonGeriIn_Pin) == 1) {
        	realCharPos--;
        	if(realCharPos < 1) {
        	    realCharPos = 80;
        	}
            if(cursorPosition == 1) {
            	if(page == 1) {
            		cursorPosition = 16;
            		page = 5;
            		printTemplate(3, 5);
            	} else if(page == 2) {
            		cursorPosition = 16;
            		page = 1;
            		printTemplate(3, 1);
            	} else if(page == 3) {
            		cursorPosition = 16;
            		page = 2;
            		printTemplate(3, 2);
            	} else if(page == 4) {
            		cursorPosition = 16;
            		page = 3;
            		printTemplate(3, 3);
            	} else if(page == 5) {
            		cursorPosition = 16;
            		page = 4;
            		printTemplate(3, 4);
            	}
            } else {
            	cursorPosition--;
            }

            HAL_Delay(250);
        }

        if (HAL_GPIO_ReadPin(butonYukariIn_GPIO_Port, butonYukariIn_Pin) == 1) {
        	wifiPass[wifiPassLoc] = getCharFromCursorPosition(realCharPos - 1);
        	wifiPassLocArr[wifiPassLoc] = realCharPos - 1;

            lcd_print_char(1, writeLoc, wifiPass[wifiPassLoc]);

            writeLoc++;
            wifiPassLoc++;

            HAL_Delay(250);
        }

        if(HAL_GPIO_ReadPin(butonAsagiIn_GPIO_Port, butonAsagiIn_Pin) == 1) {
            if(strlen(wifiPass) >= 1) {
            	wifiPass[wifiPassLoc] = '\0';
            	wifiPassLocArr[wifiPassLoc] = '\0';

            	lcd_delete_char(1, 6+wifiPassLoc);
            	HAL_Delay(50);

            	if(writeLoc > 7) {
            		writeLoc--;
            	} else if(writeLoc < 7) {
            		writeLoc = 7;
            	}

            	if(wifiPassLoc > 0) {
            		wifiPassLoc--;
            	} else if(wifiPassLoc < 0) {
            		wifiPassLoc = 0;
            	}
            }

            HAL_Delay(250);
        }

        lcd_gotoxy(2, cursorPosition);
    }
}

char* mergeData() {
	char combinedString[44] = "";
	char temp[10];

	uint8_t uintVariables[] = {
			devirmeYuruyusSecim,
			calismaSekli,
			emniyetCercevesi,
			yavaslamaLimit,
			altLimit,
			kapiTablaAcKonum,
			basincSalteri,
			kapiSecimleri,
			kapiAcTipi,
			kapi1Tip,
			kapi1AcSure,
			kapi2Tip,
			kapi2AcSure,
			kapitablaTip,
			kapiTablaAcSure,
			yukariYavasLimit,
			devirmeYukariIleriLimit,
			devirmeAsagiGeriLimit,
			devirmeSilindirTipi,
			platformSilindirTipi,
			yukariValfTmr,
			asagiValfTmr,
			devirmeYukariIleriTmr,
			devirmeAsagiGeriTmr,
			makineCalismaTmr,
			buzzer,
			demoMode,
			calismaSayisi1,
			calismaSayisi10,
			calismaSayisi100,
			calismaSayisi1000,
			calismaSayisi10000,
			dilSecim,
			eepromData[38],
			eepromData[39],
			eepromData[40],
			eepromData[41],
			eepromData[42],
			eepromData[43],
			eepromData[44],
			eepromData[45],
			eepromData[46],
			eepromData[47],
			lcdBacklightSure
	};

	for (int i = 0; i < sizeof(uintVariables) / sizeof(uintVariables[0]); ++i) {
	    sprintf(temp, "%u", uintVariables[i]);
	    strcat(combinedString, temp);
	}

	char* result = malloc(strlen(combinedString) + 1);
	strcpy(result, combinedString);
	return result;
}

void convertAndSendData() {
	lcd_print(1, 1, "Veri Esleme");
	lcd_print(2, 1, "Baslatildi...");
	for(int i=0; i<2; i++) {
		sendMachineData(&huart1, machineID, mergeData());
	}
	HAL_Delay(500);
	lcd_clear();
	HAL_Delay(500);
}

void iotSetup() {
	if(iotMode != 0) {
		if(machineID[machineIDCharacterLimit-1] == '\0') {
			takeMachineID(0);
		}

		if(wifiSSID[0] != '\0') {
			takeWifiSSID(0);
		}

		if(wifiPass[0] != '\0') {
			takeWifiPass(0);
		}
	}

	//ESP8266_Init(&huart1, "iPhone", "ipek123456");
	ESP8266_Init(&huart1, wifiSSID, wifiPass);
	HAL_Delay(500);
	convertAndSendData();
}