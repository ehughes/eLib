#ifndef SYSTEM_DRIVERS_MCP9800_MCP9800_H_
#define SYSTEM_DRIVERS_MCP9800_MCP9800_H_

#define MCP9800_ADDRESS (0x4F)

void MCP9800_Init(uint8_t MCP9800_Address);

float MCP9800_GetTemperature(uint8_t MCP9800_Address);

#endif
