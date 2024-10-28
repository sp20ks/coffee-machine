#include "coffee_machine.h"
#include "lcd.h"

int main() {
    LCD_Init();          // Инициализация ЖК-дисплея
    setup_buttons();     // Настройка кнопок
    setup_lights();      // Настройка лампочек
    display_stage(current_stage);  // Отображение текущего этапа

    while (1) {
        // Проверка нажатия кнопки действия
        if (!(PIND & (1 << ACTION_BUTTON))) {
            _delay_ms(50);  // Антидребезг
            while (!(PIND & (1 << ACTION_BUTTON)));  // Ожидаем отпускания кнопки
            advance_stage();  // Переход к следующему этапу
            _delay_ms(50);  // Антидребезг
        }
        // Проверка нажатия кнопки сброса
        if (!(PIND & (1 << RESET_BUTTON))) {
            _delay_ms(50);  // Антидребезг
            while (!(PIND & (1 << RESET_BUTTON)));  // Ожидаем отпускания кнопки
            reset_device();  // Сброс устройства
            _delay_ms(50);  // Антидребезг
        }
    }
}
