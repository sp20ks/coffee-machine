#include "coffee_machine.h"

// Переменные для отслеживания текущего этапа и процесса
CoffeeStage current_stage = STAGE_GRIND; // Начальный этап
uint8_t in_progress = 0;                   // Флаг для отслеживания состояния процесса

// Настройка кнопок
void setup_buttons() {
    DDRD &= ~((1 << ACTION_BUTTON) | (1 << RESET_BUTTON)); // Устанавливаем пины кнопок на вход
    PORTD |= (1 << ACTION_BUTTON) | (1 << RESET_BUTTON);   // Включаем подтягивающие резисторы
}

// Настройка индикаторов
void setup_lights() {
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);          // Устанавливаем пины индикаторов на выход
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));      // Выключаем все индикаторы
}

// Отображение текущего этапа на ЖК-дисплее
void display_stage(CoffeeStage stage) {
    LCD_Clear();                                         
    switch (stage) {
        case STAGE_GRIND:
            LCD_String(in_progress ? "Grinding. A to stop" : "A - Grind");
            break;
        case STAGE_BREW:
            LCD_String(in_progress ? "Brewing. A to stop" : "A - Brew");
            break;
        case STAGE_STEAM:
            LCD_String(in_progress ? "Steaming. A to stop" : "A - Steam");
            break;
        case STAGE_DONE:
            LCD_String("Done! R - Reset");
            break;
    }
}


// Активировать индикатор для текущего этапа
void activate_light(CoffeeStage stage) {
    switch (stage) {
        case STAGE_GRIND:
            PORTB |= (1 << PB0);  // Включить индикатор помола
            break;
        case STAGE_BREW:
            PORTB |= (1 << PB1);  // Включить индикатор заваривания
            break;
        case STAGE_STEAM:
            PORTB |= (1 << PB2);  // Включить индикатор взбивания
            break;
        case STAGE_DONE:
            PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2)); // Выключить все индикаторы
            break;
    }
}

// Переход к следующему этапу
void advance_stage() {
    if (in_progress) {                             // Если процесс в процессе
        in_progress = 0;                           // Завершаем процесс
        activate_light(current_stage);             // Активируем индикатор для текущего этапа
        if (current_stage == STAGE_STEAM) {
            current_stage = STAGE_DONE;            // Переход к этапу завершения
        } else if (current_stage != STAGE_DONE) {
            current_stage = (current_stage + 1);   // Переход к следующему этапу
        }
    } else {
        in_progress = 1;                           // Начинаем процесс
    }
    display_stage(current_stage);                   // Обновление дисплея
}

// Сброс устройства
void reset_device() {
    current_stage = STAGE_GRIND;
    in_progress = 0;
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
    display_stage(current_stage);
}
