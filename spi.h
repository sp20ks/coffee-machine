#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ARRAY_SIZE 4  // Размер массива
#define SCK_BIT  7
#define MISO_BIT 6
#define MOSI_BIT 5
#define SS_BIT   4
#define START_COMMAND 0x10

void SPI_MasterInit(void); // Инициализация SPI как мастер
void SPI_SlaveInit(void); // Инициализация SPI как слейв
uint8_t SPI_Transfer(uint8_t data); // Функция передачи данных по SPI
void SPI_RequestArray(uint8_t *data); // Процедура запроса массива данных от слейва
ISR(SPI_STC_vect); // Прерывание для отправки данных

extern uint8_t data[ARRAY_SIZE]; // Массив для передачи по SPI
extern uint8_t index_data; // Индекс текущего значения в массиве для передачи по SPI

#endif
