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
    uint8_t showing_config = 0;         // Флаг для отслеживания состояния показа настроек

    while (1) {
        key = scan_keypad();  // Получаем символ нажатой кнопки с клавиатуры

        // Если пользователь просматривает настройки
        if (showing_config) {
            if (key == RESET_BUTTON) {  // Если нажата кнопка сброса
                _delay_ms(50); // Антидребезг
                reset_device();  // Сбрасываем устройство
                showing_config = 0;  // Выходим из режима просмотра
            } else if (key == START_BUTTON) {  // Если нажата кнопка остановки
                _delay_ms(50); // Антидребезг
                showing_config = 0;  // Выходим из режима просмотра
                display_stage(current_stage);  // Возвращаем прошлый экран
            }
            continue;  // Пропускаем остальную обработку
        }

        // Проверка нажатия кнопки действия
        if (key == START_BUTTON && !action_button_pressed) {  // Если кнопка еще не была нажата
            _delay_ms(50); // Антидребезг
            advance_stage();  // Переход к следующему этапу
            action_button_pressed = 1;  // Устанавливаем флаг, что кнопка была нажата
        }

        // Проверка нажатия кнопки настройки
        if (key == CONFIG_BUTTON) {  // Если нажата кнопка настройки
            _delay_ms(50); // Антидребезг
            configure_coffee();  // Вызов функции настройки
            _delay_ms(2000); // Ждем 2 секунды, чтобы пользователь увидел сообщение
            display_stage(current_stage);  // Обновляем экран с текущим этапом
        }

        // Проверка нажатия кнопки просмотра настроек
        if (key == VIEW_CONFIG_BUTTON) {  // Если нажата кнопка просмотра настроек
            _delay_ms(50); // Антидребезг
            showing_config = 1;  // Включаем режим просмотра настроек
            LCD_Clear();
            char config_info[16];
            snprintf(config_info, 16, "G:%d W:%d S:%d T:%d", 
                     coffee_config.grind_size, 
                     coffee_config.water_pressure, 
                     coffee_config.steam_pressure, 
                     coffee_config.brew_duration);
            LCD_String(config_info);
            _delay_ms(2000); // Ждем 2 секунды, чтобы пользователь увидел сообщение
            display_stage(current_stage);  // Обновляем экран с текущим этапом
        }

        // Проверка нажатия кнопки сброса
        if (key == RESET_BUTTON) {
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
