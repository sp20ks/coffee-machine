#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define LCD_Dir  DDRC   
#define LCD_Port PORTC     // Порт для ЖК-дисплея
#define RS PC0            // Пин для регистра выбора
#define EN PC1            // Пин для разрешающего сигнала

// Функции для работы с ЖК-дисплеем
void LCD_Init(void);            // Инициализация ЖК-дисплея
void LCD_Command(unsigned char cmnd);  // Отправка команды на ЖК-дисплей
void LCD_Char(unsigned char data);     // Отправка символа на ЖК-дисплей
void LCD_String(char *str);            // Отправка строки на ЖК-дисплей
void LCD_Clear();                      // Очистка ЖК-дисплея

#endif
