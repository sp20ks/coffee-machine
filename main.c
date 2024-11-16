#include "coffee_machine.h"
#include "lcd.h"
#include "keypad.h"

int main() {
    LCD_Init();           // Инициализация ЖК-дисплея
    keypad_init();        // Настройка клавы
    setup_lights();       // Настройка индикаторов
    display_stage(current_stage);  // Отображение текущего этапа

    char key;
    uint8_t action_button_pressed = 0;  // Флаг для отслеживания состояния кнопки действия

    while (1) {
        key = scan_keypad();  // Получаем символ нажатой кнопки с клавиатуры

        // Проверка нажатия кнопки действия
        if (key == START_BUTTON && !action_button_pressed) {  // Если кнопка еще не была нажата
            _delay_ms(50); // Антидребезг
            advance_stage();  // Переход к следующему этапу
            action_button_pressed = 1;  // Устанавливаем флаг, что кнопка была нажата
        } 

        // Проверка нажатия кнопки сброса
        if (key == RESET_BUTTON_KEY) {
            _delay_ms(50); // Антидребезг
            reset_device();  // Сброс устройства
            _delay_ms(50);  // Антидребезг
        }

        // Сбрасываем флаг после отпускания кнопки
        if (key != START_BUTTON) {
            action_button_pressed = 0;
        }
    }
}
