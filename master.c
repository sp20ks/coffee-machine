#include "lcd.h"
#include "spi.h"

#define BUTTON_PIN PD2  // Пин для кнопки
#define BUTTON_PRESSED !(PIND & (1 << BUTTON_PIN))  // Нажата ли кнопка

int main(void) {
    LCD_Init(); // Инициализация ЖК-дисплея
    LCD_String("Press button for logs");
    _delay_ms(1000);
    SPI_MasterInit(); // Настройка SPI

    DDRD &= ~(1 << BUTTON_PIN);  // PD2 как вход
    PORTD |= (1 << BUTTON_PIN);  // Подтягивающий резистор

    uint16_t received_data[ARRAY_SIZE] = {0};  // Массив для получения данных

    while (1) {
        if (BUTTON_PRESSED) {  // Проверяем, нажата ли кнопка
            _delay_ms(50);  // Антидребезг
            SPI_RequestArray(received_data);  // Запрашиваем данные от слейва

            for (uint8_t i = 0; i < ARRAY_SIZE; ++i) {
                LCD_Clear();

                switch (i) {
                    case 0:
                        LCD_String("Grind: ");
                        LCD_Number(received_data[i]);
                        break;
                    case 1:
                        LCD_String("Brew: ");
                        LCD_Number(received_data[i]);
                        break;
                    case 2:
                        LCD_String("Steam: ");
                        LCD_Number(received_data[i]);
                        break;
                    case 3:
                        LCD_String("Done!");
                        break;
                    default:
                        break;
                }
                _delay_ms(1000);
            }
            LCD_Clear();
            LCD_String("Press button for logs");
        }
    }
}
