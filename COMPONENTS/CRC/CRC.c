#include "CRC.h"

uint32_t Generate32bitCRC(char* data, int16_t len, uint32_t seed){
//code to generate a 32-bit CRC on any length of data(char below is equivalent to 8-bit integer)
//the "seed" you pass to the function is: 0xEDB88320

  static uint32_t crc = 0xFFFFFFFF;
  static uint32_t temp;
  static int j;

      crc = 0xFFFFFFFF;

      while (len--)
      {
        temp = (uint32_t)((crc & 0xFF) ^ *data);
        data++;
        for (j = 0; j < 8; j++)
        {
          if (temp & 0x1)
            temp = (temp >> 1) ^ seed;
          else
            temp >>= 1;
        }
        crc = (crc >> 8) ^ temp;
      }

  return crc ^ 0xFFFFFFFF;

}


uint16_t CRC16_CCIT_Step(uint16_t CRC_In, uint8_t Data)
{
        uint16_t CRC_Temp;

        CRC_Temp = (uint16_t)((uint8_t)(CRC_In >> 8) | (CRC_In << 8));
        CRC_Temp ^= Data;
        CRC_Temp ^= (uint8_t)(CRC_Temp & 0xff) >> 4;
        CRC_Temp ^= CRC_Temp << 12;
        CRC_Temp ^= (CRC_Temp & 0xff) << 5;
        return CRC_Temp;

}

uint16_t CRC16_CCIT(uint8_t *Data, uint32_t Length)
{
        uint16_t CRC = 0xFFFF;

        for(int i=0;i<Length;i++)
        {
                CRC = CRC16_CCIT_Step(CRC, Data[i]);
        }

        return CRC;
}
