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
    _delay_ms(2);                                 // Задержка
}

// Инициализация ЖК-дисплея
void LCD_Init(void) {
    LCD_Dir = 0xFF;                               // Устанавливаем порт на выход
    _delay_ms(20);                                // Задержка для стабилизации питания
    LCD_Command(0x02);                            // Переключение в 4-битный режим
    LCD_Command(0x28);                            // Настройка 2 строки, 5x8 точек
    LCD_Command(0x0c);                            // Включение дисплея, отключение курсора
    LCD_Command(0x06);                            // Увеличение адреса, не сдвигать
    LCD_Command(0x01);                            // Очистка дисплея
    _delay_ms(2);                                 // Задержка
}

// Отправка строки на ЖК-дисплей
void LCD_String(char *str) {
    int i = 0;                 // Индекс символа в строке
    uint8_t cursor_pos = 0;    // Позиция курсора (от 0 до 31 для 16x2)

    while (str[i] != '\0') {   // Пока строка не закончилась
        if (cursor_pos == 16) {       // Если достигнут конец первой строки
            LCD_Command(0xC0);        // Переход на начало второй строки (адрес 0x40)
        } else if (cursor_pos >= 32) { // Если превышены обе строки
            LCD_Clear();              // Очищаем дисплей
            LCD_Command(0x80);        // Возвращаемся на начало первой строки
            cursor_pos = 0;           // Сбрасываем позицию
        }

        LCD_Char(str[i]); // Отправляем текущий символ на дисплей
        i++;              // Переходим к следующему символу
        cursor_pos++;      // Увеличиваем позицию курсора
    }
}

// Очистка ЖК-дисплея
void LCD_Clear() {
    LCD_Command(0x01);                            // Команда очистки дисплея
    _delay_ms(2);                                 // Задержка для выполнения команды
    LCD_Command(0x80);                            // Установка курсора на первую строку
}

// Печать массива на ЖК-дисплей
void LCD_PrintArray(uint8_t *data, uint8_t size) {
    for (uint8_t i = 0; i < size; ++i) {
        if (data[i] >= 100) {
            LCD_Char((data[i] / 100) + '0');
            data[i] %= 100;
        }
        if (data[i] >= 10) {
            LCD_Char((data[i] / 10) + '0');
            data[i] %= 10;
        }
        LCD_Char(data[i] + '0');
        LCD_Char(' ');
    }
}