#include "lcd.h"

// Отправка команды на ЖК-дисплей
void LCD_Command(unsigned char cmnd) {
    LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); // Отправляем старшие 4 бита
    LCD_Port &= ~(1<<RS);                         // Устанавливаем режим команды
    LCD_Port |= (1<<EN);                         
    _delay_us(1);
    LCD_Port &= ~(1<<EN);                         
    _delay_us(200);
    LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  // Отправляем младшие 4 бита
    LCD_Port |= (1<<EN);                          
    _delay_us(1);
    LCD_Port &= ~(1<<EN);                         
    _delay_ms(2);                                 // На всякий
}

// Отправка символа на ЖК-дисплей
void LCD_Char(unsigned char data) {
    LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); // Отправляем старшие 4 бита
    LCD_Port |= (1<<RS);                          // Устанавливаем режим данных
    LCD_Port |= (1<<EN);                          
    _delay_us(1);
    LCD_Port &= ~(1<<EN);                         
    _delay_us(200);
    LCD_Port = (LCD_Port & 0x0F) | (data << 4);  // Отправляем младшие 4 бита
    LCD_Port |= (1<<EN);                          
    _delay_us(1);
    LCD_Port &= ~(1<<EN);                         
    _delay_ms(2);                                 // И тут на всякий
}

// Инициализация ЖК-дисплея
void LCD_Init(void) {
    LCD_Dir = 0xFF;                               // Устанавливаем порт на выход
    _delay_ms(20);                                // Задержка для стабилизации питания
    LCD_Command(0x02);                            // Переключение в 4-битный режим
    LCD_Command(0x28);                            // Настройка 2 строки, 5x8 точек
    LCD_Command(0x0c);                            // Включение дисплея, отключение курсора
    LCD_Command(0x06);                            // Увеличение адреса
    LCD_Command(0x01);                            // Очистка дисплея
    _delay_ms(2);                                 // Задержка
}

// Отправка строки на ЖК-дисплей
void LCD_String(char *str) {
    int i;
    for (i = 0; str[i] != 0; i++) {              // Проходим по строке
        LCD_Char(str[i]);                        // Отправляем каждый символ
    }
}

// Очистка ЖК-дисплея
void LCD_Clear() {
    LCD_Command(0x01);                            // Команда очистки дисплея
    _delay_ms(2);                                 // Задержка
    LCD_Command(0x80);                            // Установка курсора на первую строку
}
