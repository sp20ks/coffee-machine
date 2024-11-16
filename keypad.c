#include <avr/io.h>
#include <util/delay.h>
#include "keypad.h"

// Карта символов клавиатуры 4x4
char keypad_map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Инициализация порта для клавиатуры
void keypad_init() {
    DDRA = 0x0F;     // PA0-PA3 выходы (строки), PA4-PA7 входы (столбцы)
    PORTA = 0xF0;    // Включаем подтягивающие резисторы для столбцов
}

// Функция для считывания нажатой кнопки
char scan_keypad() {
    static char last_key = '\0';  // Хранит предыдущую нажатую клавишу
    char key = '\0';

    for (int row = 0; row < 4; row++) {
        // Устанавливаем низкий уровень на одной строке
        PORTA = ~(1 << row);
        _delay_ms(1);  // Небольшая задержка

        // Проверяем состояние каждого столбца
        for (int col = 0; col < 4; col++) {
            if (!(PINA & (1 << (col + 4)))) {  // Если на столбце низкий уровень
                key = keypad_map[row][col];
                break;  // Найдена нажатая клавиша
            }
        }

        // Если нашли нажатую кнопку, прекращаем проверку
        if (key != '\0') {
            break;
        }
    }

    // Антидребезг
    if (key != '\0' && key == last_key) {
        return '\0';  // Игнорируем повторное нажатие той же кнопки
    }

    // Сохраняем текущее состояние
    last_key = key;

    // Если кнопка была отпущена, сбрасываем last_key
    if (key == '\0') {
        last_key = '\0';
    }

    return key;
}

// Функция для считывания числа с клавы
uint16_t get_number_from_keypad() {
    uint16_t number = 0;
    char key;

    LCD_Command(0xC0); // Перемещаем курсор на вторую строку

    while (1) {
        key = scan_keypad();  // Считываем символ с клавиатуры

        if (key != '\0') {  // Проверяем, что кнопка была нажата
            if (key >= '0' && key <= '9') {  // Если это цифра
                number = number * 10 + (key - '0');  // Добавляем цифру в число
                LCD_Char(key);  // Отображаем на экране
            } else if (key == '#') {  // Завершение ввода
                break;
            } else if (key == '*') {  // Удаление последней цифры
                number /= 10;
                LCD_Clear();
                LCD_String("Enter value:");
                char buffer[16];
                itoa(number, buffer, 10);
                LCD_String(buffer);
            }
        }

        _delay_ms(50);  // Небольшая задержка между циклами
    }

    return number;
}
