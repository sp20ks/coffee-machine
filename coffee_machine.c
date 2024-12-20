#include "coffee_machine.h"

// Переменные для отслеживания текущего этапа и процесса
CoffeeStage current_stage = STAGE_GRIND; // Начальный этап
uint8_t in_progress = 0;                   // Флаг для отслеживания состояния процесса
CoffeeConfig coffee_config = {0, 0, 0, 0, 0}; // Настройки
uint16_t start_time = 0; // Начало этапа
uint16_t end_time = 0; // Конец этапа
uint16_t current_time = 0; // Счетчик миллисекунд

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
            LCD_String("Done! B - Reset");
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
    if (!coffee_config.configured) {               // Если конфигурация не задана
        LCD_Clear();
        LCD_String("Set config....");           // Сообщение об ошибке
        _delay_ms(2000);                           // Задержка для отображения сообщения
        display_stage(current_stage);              // Возвращаем предыдущее сообщение
        return;
    }
    
    if (in_progress) {                             // Если процесс идет
        end_time = get_current_time_ms();              // Фиксируем конец этапа

        data[current_stage] = (end_time - start_time + 500) / 1000;  // Записываем длительность в массив data

        in_progress = 0;                           // Завершаем процесс
        activate_light(current_stage);             // Включаем индикатор текущего этапа
        if (current_stage == STAGE_STEAM) {        // Если текущий этап - вспенивание
            current_stage = STAGE_DONE;            // Переход к завершению
        } else if (current_stage != STAGE_DONE) {  // Для других этапов
            current_stage = (current_stage + 1);   // Переход к следующему этапу
        }
    } else {                                       // Если процесс еще не начался
        in_progress = 1;                           // Начинаем процесс
        
        if (current_stage == STAGE_BREW) {         // Если этап - заваривание
            LCD_Clear();
            LCD_String("Brewing...");              // Сообщение на экране
            for (uint16_t i = 0; i < coffee_config.brew_duration; i++) {
                _delay_ms(1000);                   // Ждем 1 секунду в цикле
            }
            data[current_stage] = coffee_config.brew_duration;  // Записываем длительность в массив data
      
            in_progress = 0;                       // Этап завершен
            activate_light(current_stage);             // Включаем индикатор текущего этапа
            current_stage = STAGE_STEAM;           // Переходим к следующему этапу
        }
    }
    display_stage(current_stage);                  // Обновление дисплея
}

// Сброс устройства
void reset_device() {
    current_stage = STAGE_GRIND;
    in_progress = 0;
    start_time = 0;
    end_time = 0;
    coffee_config.configured = 0;
    for (uint8_t i = 0; i < STAGE_DONE; i++) {
        data[i] = 0;
    }
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
    display_stage(current_stage);
}

// Установка параметров приготовления кофе
void configure_coffee() {
    LCD_Clear();
    LCD_String("Grind Size:");
    coffee_config.grind_size = get_number_from_keypad(); // Пользователь вводит число
    LCD_Clear();
    LCD_String("Water Pressure:");
    coffee_config.water_pressure = get_number_from_keypad();
    LCD_Clear();
    LCD_String("Steam Pressure:");
    coffee_config.steam_pressure = get_number_from_keypad();
    LCD_Clear();
    LCD_String("Brew Duration:");
    coffee_config.brew_duration = get_number_from_keypad();
    coffee_config.configured = 1; // Параметры заданы
    LCD_Clear();
    LCD_String("Config Saved!");
}

// Таймер для измерения времени
uint16_t get_current_time_ms() {
    cli(); // Запрещаем прерывания
    uint16_t ms = current_time;
    sei(); // Разрешаем прерывания
    return ms;
}

// Настройка таймера
void setup_timer() {
    // WGM - Waveform Generation Mode
    TCCR0 = (1 << WGM01); // Включаем режим CTC (Clear Timer on Compare Match)
    OCR0 = 62;            // Устанавливаем значение сравнения (для 1 мс при 4 МГц с делителем 64)
    TIMSK |= (1 << OCIE0); // Разрешаем прерывания по совпадению
    TCCR0 |= (1 << CS01) | (1 << CS00); // Устанавливаем делитель 64
    sei(); // Разрешаем глобальные прерывания
}

ISR(TIMER0_COMP_vect) {
    current_time++; // Увеличиваем счетчик миллисекунд
}
