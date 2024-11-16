#ifndef COFFEE_MACHINE_H
#define COFFEE_MACHINE_H

#include <avr/io.h>
#include "lcd.h"

#define START_BUTTON '*'  // Кнопка для запуска цикла
#define RESET_BUTTON_KEY '#'  // Кнопка для сброса устройства
// Определение этапов приготовления кофе
typedef enum {
    STAGE_GRIND,    // Этап помола
    STAGE_BREW,     // Этап заваривания
    STAGE_STEAM,    // Этап взбивания
    STAGE_DONE      // Завершение процесса
} CoffeeStage;

void setup_lights(void);                 // Настройка индикаторов
void advance_stage(void);                 // Переход к следующему этапу
void reset_device(void);                  // Сброс устройства
void display_stage(CoffeeStage stage);    // Отображение текущего этапа на ЖК-дисплее
void activate_light(CoffeeStage stage);    // Активировать индикатор для текущего этапа

extern CoffeeStage current_stage;
extern uint8_t in_progress;

#endif
