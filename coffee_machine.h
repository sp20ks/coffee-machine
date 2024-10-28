#ifndef COFFEE_MACHINE_H
#define COFFEE_MACHINE_H

#include <avr/io.h>
#include "lcd.h"

#define ACTION_BUTTON PD0
#define RESET_BUTTON PD1
// ����������� ������ ������������� ����
typedef enum {
    STAGE_GRIND,    // ���� ������
    STAGE_BREW,     // ���� �����������
    STAGE_STEAM,    // ���� ���������
    STAGE_DONE      // ���������� ��������
} CoffeeStage;


void setup_buttons(void);                // ��������� ������
void setup_lights(void);                 // ��������� ����������� ����������� �������
void advance_stage(void);                 // ������� � ���������� �����
void reset_device(void);                  // ����� ����������
void display_stage(CoffeeStage stage);    // ����������� �������� ����� �� ��-�������
void activate_light(CoffeeStage stage);    // ������������ ��������� ��� �������� �����

extern CoffeeStage current_stage;
extern uint8_t in_progress;

#endif
