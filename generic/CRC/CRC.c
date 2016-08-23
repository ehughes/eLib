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
