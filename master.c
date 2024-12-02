#include "lcd.h"
#include "spi.h"

// тут не используются
uint8_t data[ARRAY_SIZE] = {0};
uint8_t index_data = 0;

int main(void) {
    LCD_Init();
    _delay_ms(1000);
    SPI_MasterInit();

    uint8_t received_data[ARRAY_SIZE] = {0};

    while (1) {
        LCD_Clear();

        SPI_RequestArray(received_data); // Запрашиваем данные от слейва
        LCD_PrintArray(received_data, ARRAY_SIZE); // Выводим данные на LCD

        _delay_ms(1000);
    }
}