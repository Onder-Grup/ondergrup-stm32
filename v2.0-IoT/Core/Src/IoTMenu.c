/*
 * IoTMenu.c
 *
 *  Created on: Feb 5, 2024
 *      Author: hidirektor
 */

#include "IoTMenu.h"
#include "TextVariables.h"

void printTemplate(int type, int page) {
	HAL_Delay(200);
	lcd_clear();
	if(type == 1 && page == 0) {
		lcd_print(1, 1, "ID: ");
		lcd_print(2, 3, "0");
		lcd_print(2, 4, "1");
		lcd_print(2, 5, "2");
		lcd_print(2, 6, "3");
		lcd_print(2, 7, "4");
		lcd_print(2, 10, "5");
		lcd_print(2, 11, "6");
		lcd_print(2, 12, "7");
		lcd_print(2, 13, "8");
		lcd_print(2, 14, "9");
		lcd_gotoxy(2, 3);
	} else if(type == 2) {
		if(page == 1) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "SSID: ");
			lcd_print(2, 1, "abcdefghijklmnop");
		} else if(page == 2) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "SSID: ");
			lcd_print(2, 1, "qrstuvwxyzABCDEF");
		} else if(page == 3) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "SSID: ");
			lcd_print(2, 1, "GHIİJKLMNOPQRSTU");
		} else if(page == 4) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "SSID: ");
			lcd_print(2, 1, "VWXYZ0123456789!");
		} else if(page == 5) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "SSID: ");
			lcd_print(2, 1, "@#$%^&*()-_+=<>?");
		}
	} else if(type == 3) {
		if(page == 1) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "PASS: ");
			lcd_print(2, 1, "abcdefghijklmnop");
		} else if(page == 2) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "PASS: ");
			lcd_print(2, 1, "qrstuvwxyzABCDEF");
		} else if(page == 3) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "PASS: ");
			lcd_print(2, 1, "GHIİJKLMNOPQRSTU");
		} else if(page == 4) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "PASS: ");
			lcd_print(2, 1, "VWXYZ0123456789!");
		} else if(page == 5) {
			lcd_clear();
			HAL_Delay(100);
			lcd_print(1, 1, "PASS: ");
			lcd_print(2, 1, "@#$%^&*()-_+=<>?");
		}
	}
}

int checkSlideVal(int state) {
	if(state == 0) { //ssid (33)
		if(HAL_GPIO_ReadPin(butonIleriIn_GPIO_Port, butonIleriIn_Pin) == 1) {
			return 34;
		}

		if(HAL_GPIO_ReadPin(butonGeriIn_GPIO_Port, butonGeriIn_Pin) == 1) {
			return 32;
		}
	} else { //pass (34)
		if(HAL_GPIO_ReadPin(butonIleriIn_GPIO_Port, butonIleriIn_Pin) == 1) {
			return 35;
		}

		if(HAL_GPIO_ReadPin(butonGeriIn_GPIO_Port, butonGeriIn_Pin) == 1) {
			return 33;
		}
	}
	return 0;
}

void slideText(const char* text, int startPos, int startLine, int state) {
	uint8_t uzunluk = strlen(text);
	int mainLoopVal = 1;

	while(mainLoopVal == 1) {
		if(checkSlideVal(state) > 0) {
			menuSayac = checkSlideVal(state);
			break;
		}
		checkEditMode();
		if(editMode == 1) {
			break;
		}

		for(int z=0; z < uzunluk; z++) {
			if(checkSlideVal(state) > 0) {
				menuSayac = checkSlideVal(state);
				mainLoopVal = 0;
				break;
			}
			checkEditMode();
			if(editMode == 1) {
				break;
			}
			for(int i=startPos + z; i < uzunluk + startPos; i++) {
				if(checkSlideVal(state) > 0) {
					menuSayac = checkSlideVal(state);
					z = uzunluk;
					mainLoopVal = 0;
					break;
				}
				checkEditMode();
				if(editMode == 1) {
					break;
				}
				lcd_print_char(startLine, i-z, text[i-startPos]);
				HAL_Delay(30);
			}

			for (int j = uzunluk - 1; j >= uzunluk - z; j--) {
				if(checkSlideVal(state) > 0) {
					menuSayac = checkSlideVal(state);
					z = uzunluk;
					mainLoopVal = 0;
					break;
				}
				checkEditMode();
				if(editMode == 1) {
					break;
				}
			    if(j >= startPos) {
			    	lcd_print_char(startLine, j, ' ');
			    	HAL_Delay(30);
			    }
			}
		}
	}
}

void printCredentials(int type) {
	if(type == 1) {
		//machine id yazdır
		lcd_print(1, 1, machineIDText);
		lcd_print(2, 1, machineID);
		lcd_print(2, 13, "    ");
	} else if(type == 2) {
		//wifi ssid yazdır
		int ssidLength = strlen(wifiSSID);

		lcd_print(1, 1, "WIFI SSID       ");
		if(ssidLength == 16) {
			lcd_print(2, 1, wifiSSID);
		} else if(ssidLength < 16) {
			lcd_print(2, 1, "                ");
			lcd_print(2, 1, wifiSSID);
		} else {
			//Eğer ssid 16'dan büyükse kayarak yazdır
			slideText(wifiSSID, 1, 2, 0);
		}
	} else {
		//wifi pass yazdır
		int passLength = strlen(wifiPass);

		lcd_print(1, 1, "WIFI PASS       ");
		if(passLength == 16) {
			lcd_print(2, 1, wifiPass);
		} else if(passLength < 16) {
			lcd_print(2, 1, "                ");
			lcd_print(2, 1, wifiPass);
		} else {
			//Eğer pass 16'dan büyükse kayarak yazdır
			slideText(wifiPass, 1, 2, 1);
		}
	}
}

void checkEditMode(void) {
	if ((HAL_GPIO_ReadPin(butonYukariIn_GPIO_Port,butonYukariIn_Pin) == 1) && (HAL_GPIO_ReadPin(butonAsagiIn_GPIO_Port,butonAsagiIn_Pin) == 1)) {
		editMode = 1;
	}
}
