#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define LCD_Dir  DDRC      // ����������� ����� (�� �����)
#define LCD_Port PORTC     // ���� ��� ��-�������
#define RS PC0            // ��� ��� �������� ������
#define EN PC1            // ��� ��� ������������ �������

// ������� ��� ������ � ��-��������
void LCD_Init(void);            // ������������� ��-�������
void LCD_Command(unsigned char cmnd);  // �������� ������� �� ��-�������
void LCD_Char(unsigned char data);     // �������� ������� �� ��-�������
void LCD_String(char *str);            // �������� ������ �� ��-�������
void LCD_Clear();                      // ������� ��-�������

#endif
