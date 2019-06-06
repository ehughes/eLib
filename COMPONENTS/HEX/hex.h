#include "stdlib.h"

#ifndef _HEX_H
#define _HEX_H

#include "hex.h"
#include "stdint.h"

uint8_t HexDigToNum (uint8_t V);

uint8_t NumToHexDigit (uint8_t V);

uint8_t HexByteStringToNum(char *S);

void HexByteArrayStringToNum(char *S,uint8_t *Bytes,uint32_t Len);

#endif