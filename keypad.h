#ifndef KEYPAD_H
#define KEYPAD_H

// Инициализация клавиатуры
void keypad_init(void);

// Сканирование клавиатуры, возвращает символ нажатой кнопки
char scan_keypad(void);

// Карта символов клавиатуры 4x4
extern char keypad_map[4][4];

#endif
