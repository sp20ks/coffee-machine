#include "spi.h"

uint8_t data[ARRAY_SIZE] = {0};
uint8_t index_data = 0;

// Инициализация SPI как мастер
void SPI_MasterInit(void) {
    DDRB |= (1 << SCK_BIT) | (1 << MOSI_BIT) | (1 << SS_BIT); // SCK, MOSI, SS - выходы
    DDRB &= ~(1 << MISO_BIT);                                // MISO - вход
    PORTB |= (1 << SS_BIT);                                  // Деактивируем слейва (SS высокий уровень)
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);           // SPI включен, мастер, f/16
}

// Инициализация SPI как слейв
void SPI_SlaveInit(void) {
    DDRB |= (1 << MISO_BIT);  // MISO - выход
    DDRB &= ~((1 << SCK_BIT) | (1 << MOSI_BIT) | (1 << SS_BIT)); // SCK, MOSI, SS - входы
    SPCR = (1 << SPE) | (1 << SPIE); // Включаем SPI и прерывания
    sei(); // Разрешаем прерывания
}

// Функция передачи данных по SPI
uint8_t SPI_Transfer(uint8_t data) {
    SPDR = data;                            // Запускаем передачу данных
    while (!(SPSR & (1 << SPIF)));          // Ждем завершения
    return SPDR;                            // Возвращаем принятый байт
}

// Процедура запроса массива данных от слейва
void SPI_RequestArray(uint8_t *data) {
    PORTB &= ~(1 << SS_BIT); // Активируем слейва
    SPI_Transfer(START_COMMAND); // Запрос на получение данных
    _delay_ms(10);

    for (uint8_t i = 0; i < ARRAY_SIZE; ++i) {
        _delay_ms(10); // Небольшая задержка для стабильности
        data[i] = SPI_Transfer(START_COMMAND); // Сохраняем данные
    }

    PORTB |= (1 << SS_BIT); // Деактивируем слейва
}

// Прерывание для отправки данных
ISR(SPI_STC_vect) {
    if (SPDR == START_COMMAND) {
        if (index_data >= ARRAY_SIZE) {
            index_data = 0;
            return;
        }
        SPDR = data[index_data];
        ++index_data;
    }
}
