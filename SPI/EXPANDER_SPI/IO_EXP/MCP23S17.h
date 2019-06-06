
#ifndef SYSTEM_DRIVERS_IO_EXP_MCP23S17_H_
#define SYSTEM_DRIVERS_IO_EXP_MCP23S17_H_



#endif /* SYSTEM_DRIVERS_IO_EXP_MCP23S17_H_ */

//Registers for Bank = 0

#define    MCP23S17_BANK0_IODIRA    (0x00)      // MCP23x17 I/O Direction Register
#define    MCP23S17_BANK0_IODIRB    (0x01)      // 1 = Input (default), 0 = Output

#define    MCP23S17_BANK0_IPOLA     (0x02)      // MCP23x17 Input Polarity Register
#define    MCP23S17_BANK0_IPOLB     (0x03)      // 0 = Normal (default)(low reads as 0), 1 = Inverted (low reads as 1)

#define    MCP23S17_BANK0_GPINTENA  (0x04)      // MCP23x17 Interrupt on Change Pin Assignements
#define    MCP23S17_BANK0_GPINTENB  (0x05)      // 0 = No Interrupt on Change (default), 1 = Interrupt on Change

#define    MCP23S17_BANK0_DEFVALA   (0x06)      // MCP23x17 Default Compare Register for Interrupt on Change
#define    MCP23S17_BANK0_DEFVALB   (0x07)      // Opposite of what is here will trigger an interrupt (default = 0)

#define    MCP23S17_BANK0_INTCONA   (0x08)      // MCP23x17 Interrupt on Change Control Register
#define    MCP23S17_BANK0_INTCONB   (0x09)      // 1 = pin is compared to DEFVAL, 0 = pin is compared to previous state (default)

#define    MCP23S17_BANK0_IOCON     (0x0A)      // MCP23x17 Configuration Register
//         MCP23S17_BANK0_          (0x0B)      //     Also Configuration Register

#define    MCP23S17_BANK0_GPPUA     (0x0C)      // MCP23x17 Weak Pull-Up Resistor Register
#define    MCP23S17_BANK0_GPPUB     (0x0D)      // INPUT ONLY: 0 = No Internal 100k Pull-Up (default) 1 = Internal 100k Pull-Up

#define    MCP23S17_BANK0_INTFA     (0x0E)      // MCP23x17 Interrupt Flag Register
#define    MCP23S17_BANK0_INTFB     (0x0F)      // READ ONLY: 1 = This Pin Triggered the Interrupt

#define    MCP23S17_BANK0_INTCAPA   (0x10)      // MCP23x17 Interrupt Captured Value for Port Register
#define    MCP23S17_BANK0_INTCAPB   (0x11)      // READ ONLY: State of the Pin at the Time the Interrupt Occurred

#define    MCP23S17_BANK0_GPIOA     (0x12)      // MCP23x17 GPIO Port Register
#define    MCP23S17_BANK0_GPIOB     (0x13)      // Value on the Port - Writing Sets Bits in the Output Latch

#define    MCP23S17_BANK0_OLATA     (0x14)      // MCP23x17 Output Latch Register
#define    MCP23S17_BANK0_OLATB     (0x15)      // 1 = Latch High, 0 = Latch Low (default) Reading Returns Latch State, Not Port Value!


#define 	MCP23S17_IOCON_BIT_BANK 	(1<<7)
#define 	MCP23S17_IOCON_BIT_MIRROR	(1<<6)
#define 	MCP23S17_IOCON_BIT_SEQOP 	(1<<5)
#define 	MCP23S17_IOCON_BIT_DISSLW 	(1<<4)
#define 	MCP23S17_IOCON_BIT_HAEN 	(1<<3)
#define 	MCP23S17_IOCON_BIT_ODR 		(1<<2)
#define 	MCP23S17_IOCON_BIT_INTPOL 	(1<<1)


void MCP23S17_Init();

void MCP23S17_WriteRegister_8Bit(uint8_t CS_Channel, uint8_t HardwareChannel,uint8_t Register, uint8_t Data);

uint8_t MCP23S17_ReadRegister_8Bit(uint8_t CS_Channel, uint8_t HardwareChannel,uint8_t Register);
