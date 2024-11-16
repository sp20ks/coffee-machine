#ifndef KEYPAD_H
#define KEYPAD_H

#include "lcd.h"

void keypad_init(void); // Инициализация клавиатуры
char scan_keypad(void); // Сканирование клавиатуры, возвращает символ нажатой кнопки
uint16_t get_number_from_keypad(void); // Функция для считывания числа с клавы

// Карта символов клавиатуры 4x4
extern char keypad_map[4][4];

#endif
