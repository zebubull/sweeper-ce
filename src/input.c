#include "input.h"

#include <graphx.h>
#include <string.h>

void ReadKeypad()
{
    for (int i = 0; i < 8; i++)
        kb_Prev[i] = kb_Data[i];
    kb_Scan();
}

bool kb_FallingEdge(int offset, int bit)
{
    return !(kb_Data[offset] & bit) && kb_Prev[offset] & bit;
}

bool kb_RisingEdge(int offset, int bit)
{
    return kb_Data[offset] & bit && !(kb_Prev[offset] & bit);
}