#ifndef COFFEE_MACHINE_H
#define COFFEE_MACHINE_H

#include <avr/io.h>
#include "lcd.h"
#include "keypad.h"

// Коды кнопок
#define START_BUTTON 'A'         // Кнопка для запуска цикла
#define RESET_BUTTON 'B'         // Кнопка для сброса устройства
#define CONFIG_BUTTON 'C'        // Кнопка для ввода настроек
#define VIEW_CONFIG_BUTTON 'D'   // Кнопка для просмотра настроек

// Определение этапов приготовления кофе
typedef enum {
    STAGE_GRIND,    // Этап помола
    STAGE_BREW,     // Этап заваривания
    STAGE_STEAM,    // Этап взбивания
    STAGE_DONE      // Завершение процесса
} CoffeeStage;

// Определение параметров приготовления кофе
typedef struct {
    uint8_t grind_size;      // Размер помола
    uint8_t water_pressure;  // Давление воды
    uint8_t steam_pressure;  // Давление пара
    uint16_t brew_duration;  // Длительность пролива (в миллисекундах)
    uint8_t configured;      // Параметры заданы
} CoffeeConfig;

// Объявление функций
void setup_lights(void);                  // Настройка индикаторов
void advance_stage(void);                 // Переход к следующему этапу
void reset_device(void);                  // Сброс устройства
void display_stage(CoffeeStage stage);    // Отображение текущего этапа на ЖК-дисплее
void activate_light(CoffeeStage stage);   // Активировать индикатор для текущего этапа
void configure_coffee(void);              // Установка параметров приготовления кофе

// Глобальные переменные
extern CoffeeStage current_stage;
extern uint8_t in_progress;
extern CoffeeConfig coffee_config;

#endif
