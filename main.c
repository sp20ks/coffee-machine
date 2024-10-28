#include "coffee_machine.h"
#include "lcd.h"

int main() {
    LCD_Init();          // ������������� ��-�������
    setup_buttons();     // ��������� ������
    setup_lights();      // ��������� ��������
    display_stage(current_stage);  // ����������� �������� �����

    while (1) {
        // �������� ������� ������ ��������
        if (!(PIND & (1 << ACTION_BUTTON))) {
            _delay_ms(50);  // �����������
            while (!(PIND & (1 << ACTION_BUTTON)));  // ������� ���������� ������
            advance_stage();  // ������� � ���������� �����
            _delay_ms(50);  // �����������
        }
        // �������� ������� ������ ������
        if (!(PIND & (1 << RESET_BUTTON))) {
            _delay_ms(50);  // �����������
            while (!(PIND & (1 << RESET_BUTTON)));  // ������� ���������� ������
            reset_device();  // ����� ����������
            _delay_ms(50);  // �����������
        }
    }
}
