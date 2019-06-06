#include "hex.h"
#include "stdint.h"

uint8_t HexDigToNum (uint8_t V)
{
  uint8_t R = 0;

  if (V >= 0x30 && V <= 0x39)
  {
        R = V - 0x30;
  }
  else if (V >= 'A' && V <= 'F')
  {
        R = V - 'A' + 10;
  }
  else if (V >= 'a' && V <= 'f')
  {
        R = V - 'a' + 10;
  }

  return R;
}

uint8_t NumToHexDigit (uint8_t V)
{
    V &= 0xF;

    if (V < 10)
      return 0x30 + V;
    else
      return 0x41 + (V - 10);
}

uint8_t HexByteStringToNum(char *S)
{
    uint8_t R;
    R  = HexDigToNum(S[0])<<4;// + HexDigToNum(S[1]);
    R += HexDigToNum(S[1]);
    return R;
}

void HexByteArrayStringToNum(char *S,uint8_t *Bytes,uint32_t Len)
{ 
    for(int i=0;i<Len;i++)
    {   
        Bytes[i] = HexByteStringToNum(&S[i*2]);
    }
}