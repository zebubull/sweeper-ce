#ifndef INPUT_H
#define INPUT_H

#include <keypadc.h>
#include <tice.h>
#include <stdbool.h>

#pragma region keypad_offsets
#define okb_Graph 1
#define okb_Trace 1
#define okb_Zoom 1
#define okb_Window 1
#define okb_Yequ 1
#define okb_2nd 1
#define okb_Mode 1
#define okb_Del 1

#define okb_Sto 2
#define okb_Ln 2
#define okb_Log 2
#define okb_Square 2
#define okb_Recip 2
#define okb_Math 2
#define okb_Alpha 2

#define okb_0 3
#define okb_1 3
#define okb_4 3
#define okb_7 3
#define okb_Comma 3
#define okb_Sin 3
#define okb_Apps 3
#define okb_GraphVar 3

#define okb_DecPnt 4
#define okb_2 4
#define okb_5 4
#define okb_8 4
#define okb_LParen 4
#define okb_Cos 4
#define okb_Prgm 4
#define okb_Stat 4

#define okb_Chs 5
#define okb_3 5
#define okb_6 5
#define okb_9 5
#define okb_RParen 5
#define okb_Tan 5
#define okb_Vars 5

#define okb_Enter 6
#define okb_Add 6
#define okb_Sub 6
#define okb_Mul 6
#define okb_Div 6
#define okb_Power 6
#define okb_Clear 6

#define okb_Down 7
#define okb_Left 7
#define okb_Right 7
#define okb_Up 7
#pragma endregion

typedef struct {
    int offset;
    int bit;
    char value;
} KeypadInputDataMap;


const KeypadInputDataMap DigitDataMap[11] = {
    {okb_0, kb_0, '0'},
    {okb_1, kb_1, '1'},
    {okb_2, kb_2, '2'},
    {okb_3, kb_3, '3'},
    {okb_4, kb_4, '4'},
    {okb_5, kb_5, '5'},
    {okb_6, kb_6, '6'},
    {okb_7, kb_7, '7'},
    {okb_8, kb_8, '8'},
    {okb_9, kb_9, '9'},
    {okb_DecPnt, kb_DecPnt, '.'},
};

const KeypadInputDataMap LetterDataMap[26] = {
    {okb_Math, kb_Math, 'A'},
    {okb_Apps, kb_Apps, 'B'},
    {okb_Prgm, kb_Prgm, 'C'},
    {okb_Recip, kb_Recip, 'D'},
    {okb_Sin, kb_Sin, 'E'},
    {okb_Cos, kb_Cos, 'F'},
    {okb_Tan, kb_Tan, 'G'},
    {okb_Power, kb_Power, 'H'},
    {okb_Square, kb_Square, 'I'},
    {okb_Comma, kb_Comma, 'J'},
    {okb_LParen, kb_LParen, 'K'},
    {okb_RParen, kb_RParen, 'L'},
    {okb_Div, kb_Div, 'M'},
    {okb_Log, kb_Log, 'N'},
    {okb_7, kb_7, 'O'},
    {okb_8, kb_8, 'P'},
    {okb_9, kb_9, 'Q'},
    {okb_Mul, kb_Mul, 'R'},
    {okb_Ln, kb_Ln, 'S'},
    {okb_4, kb_4, 'T'},
    {okb_5, kb_5, 'U'},
    {okb_6, kb_6, 'V'},
    {okb_Sub, kb_Sub, 'W'},
    {okb_Sto, kb_Sto, 'X'},
    {okb_1, kb_1, 'Y'},
    {okb_2, kb_2, 'Z'}
};

// Stores previous keypad state before latest call to ReadKeypad();
unsigned char kb_Prev[8];

void ReadKeypad(void);
bool kb_FallingEdge(int offset, int bit);
bool kb_RisingEdge(int offset, int bit);
const char* GetUserString(unsigned char maxlen);

#endif