#include "coffee_machine.h"

// ���������� ��� ������������ �������� ����� � ��������
CoffeeStage current_stage = STAGE_GRIND; // ��������� ����
uint8_t in_progress = 0;                   // ���� ��� ������������ ��������� ��������

// ��������� ������
void setup_buttons() {
    DDRD &= ~((1 << ACTION_BUTTON) | (1 << RESET_BUTTON)); // ������������� ���� ������ �� ����
    PORTD |= (1 << ACTION_BUTTON) | (1 << RESET_BUTTON);   // �������� ������������� ���������
}

// ��������� �����������
void setup_lights() {
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);          // ������������� ���� ����������� �� �����
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));      // ��������� ��� ����������
}

// ����������� �������� ����� �� ��-�������
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
            LCD_String("Done! R - Reset");
            break;
    }
}


// ������������ ��������� ��� �������� �����
void activate_light(CoffeeStage stage) {
    switch (stage) {
        case STAGE_GRIND:
            PORTB |= (1 << PB0);  // �������� ��������� ������
            break;
        case STAGE_BREW:
            PORTB |= (1 << PB1);  // �������� ��������� �����������
            break;
        case STAGE_STEAM:
            PORTB |= (1 << PB2);  // �������� ��������� ���������
            break;
        case STAGE_DONE:
            PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2)); // ��������� ��� ����������
            break;
    }
}

// ������� � ���������� �����
void advance_stage() {
    if (in_progress) {                             // ���� ������� � ��������
        in_progress = 0;                           // ��������� �������
        activate_light(current_stage);             // ���������� ��������� ��� �������� �����
        if (current_stage == STAGE_STEAM) {
            current_stage = STAGE_DONE;            // ������� � ����� ����������
        } else if (current_stage != STAGE_DONE) {
            current_stage = (current_stage + 1);   // ������� � ���������� �����
        }
    } else {
        in_progress = 1;                           // �������� �������
    }
    display_stage(current_stage);                   // ���������� �������
}

// ����� ����������
void reset_device() {
    current_stage = STAGE_GRIND;
    in_progress = 0;
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
    display_stage(current_stage);
}
