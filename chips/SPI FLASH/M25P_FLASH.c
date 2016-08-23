#include "HardwareProfile.h"
#include <stdint.h>
#include "M25P_FLASH.h"
#include <plib.h>

#define WIP 		0x01
#define WREN 		0x06
#define WRDI 		0x04
#define RDID		0x9f
#define RDSR		0x05
#define WRSR		0x01
#define READ 		0x03
#define FAST_READ 	0x0B
#define PP 			0x02
#define SE 			0xD8
#define BE 			0xC7
#define DP 			0xB9
#define RES                     0xAB
#define PE			0xDB
#define PW			0x0A

#define M25PE_BIT_RATE   2000000
#define WAIT_FOR_SPI	 while(SPI1STATbits.SPIRBF == 0){}
#define SPI_WRITE_REG    SPI1BUF
#define SPI_READ_REG     SPI1BUF

//This variable is used to get rid of compiler warnings of setting a variable and never reading it.
//In all the routines where I have to read back the SPI data after a write,  I have to have a location
//to dump the value that is modified somewhere else.   SPIJunk is used for this purpose.
static uint8_t SPIJunk;

//Hward dependent part.  Init the SPI controller here
void InitM25P_FLASH()
{
    uint8_t Junk;
    int32_t BRG;

    M25P_FLASH_EN_PIN_CONFIG;

    M25P_FLASH_DIS;
    SPI1CON = 0; //reset everything... (This will get us 8 bit with no automated slave select)
    SPI1CONbits.MSTEN = 1; //Select Master Mode
    SPI1CONbits.CKP = 0;
    SPI1CONbits.CKE = 1;
    SPI1CONbits.SMP =0;
    SPI1STATbits.SPIROV = 0; //Clear recieve overflow (per datasheet recommendation)
    BRG = (GetPeripheralClock()/(2*M25PE_BIT_RATE))-1;

    if(BRG<0)
        BRG = 0;

    SPI1BRG = BRG & 0x1FFF;
    Junk = SPI2BUF; //Clear the Recieve buffer
    SPI1CONbits.ON = 1;  //Enable the SPI port
}


//A high level function to read up to 65k bytes from the Flash device
void  FlashRead(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer)
{
    uint16_t k;
    M25P_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = READ;
    WAIT_FOR_SPI;
    DataBuffer[0] = SPI_READ_REG; //First Byte back is Junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(StartAddress>>16);
    WAIT_FOR_SPI;
    SPIJunk=SPI_READ_REG;//Junk
    SPI_WRITE_REG = (uint8_t)(StartAddress>>8);
    WAIT_FOR_SPI;
    SPIJunk=SPI_READ_REG;//Junk
    SPI_WRITE_REG = (uint8_t)(StartAddress);
    WAIT_FOR_SPI;
    SPIJunk=SPI_READ_REG; //Junk

    for(k=0; k<Length; k++)
        {
            SPI_WRITE_REG = (uint8_t)(0xFF);
            WAIT_FOR_SPI;
            DataBuffer[k]= SPI_READ_REG;
        }

    M25P_FLASH_DIS;
}

//A high level function to wait for the Program/Erase to complete
void M25PE_BlockUntilFlashOperationComplete()
{
    uint8_t FLASHStatusByte;
    FLASHStatusByte = M25PE_RDSR();

    while((FLASHStatusByte&WIP) > 0)
        {
            FLASHStatusByte = M25PE_RDSR();
        }
}

void M25PE_WREN()
{
    //First Bring chip select Low
    M25P_FLASH_EN;
    //send out Write Enable Instruction.
    SPI_WRITE_REG = WREN;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    M25P_FLASH_DIS;
}

//This function acquires the identification from the M25PX device (Blocking SPI)
void M25PE_RDID(uint8_t *ID)
{
    uint8_t i;
    //First Bring chip select Low
    M25P_FLASH_EN;
    //send out ID instruction
    SPI_WRITE_REG = RDID;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //is Junk

    //Now, send three dummy bytes

    for(i=0; i<3; i++)
        {
            SPI_WRITE_REG = 0xff;
            WAIT_FOR_SPI;
            ID[i] = SPI_READ_REG; //Junk
        }

    M25P_FLASH_DIS;
}

uint8_t M25PE_RDSR()
{
    uint8_t RDSRVal;
    //First Bring chip select LOw
    M25P_FLASH_EN;
    //send out read status register instruction
    SPI_WRITE_REG = RDSR;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    //Send out Dummy Byte to get status reg
    SPI_WRITE_REG = 0xFF;
    WAIT_FOR_SPI;
    RDSRVal = SPI_READ_REG;
    M25P_FLASH_DIS;
    return RDSRVal;
}

void M25PE_WRSR(uint8_t StatusReg)
{
    M25PE_WREN();
    //First Bring chip select LOW
    M25P_FLASH_EN;
    //send out Write Status Reg instruction
    SPI_WRITE_REG = WRSR;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    //Send out New Status Register value
    SPI_WRITE_REG = StatusReg;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG;
    M25P_FLASH_DIS;
}



void M25PE_READ(uint32_t SectorAddress, uint8_t *ReadBuffer)
{
    uint16_t k;
    M25P_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = READ;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(SectorAddress>>16);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(SectorAddress>>8);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //All reads are sector aligned
    SPI_WRITE_REG = (uint8_t)(0);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk

    for(k=0; k<256; k++)
        {
            SPI_WRITE_REG = (uint8_t)(0xFF);
            WAIT_FOR_SPI;
            ReadBuffer[k]= SPI_READ_REG;
        }

    M25P_FLASH_DIS;
}



void M25PE_SE(uint32_t SectorAddress)
{
    M25PE_WREN();
    M25P_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = SE;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(SectorAddress>>16);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //MB must be sector aligned
    SPI_WRITE_REG = 0;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //LSB must be sector Aligned
    SPI_WRITE_REG = 0;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    M25P_FLASH_DIS;
}


void M25PE_PP(uint32_t PageAddress, uint8_t *WriteBuffer)
{
    uint16_t k;
    M25PE_WREN();
    M25P_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = PP;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(PageAddress>>16);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(PageAddress>>8);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //LSB must be page aligned (ELi's Rule);
    SPI_WRITE_REG = 0;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk

    for(k=0; k<256; k++)
        {
            SPI_WRITE_REG = WriteBuffer[k];
            WAIT_FOR_SPI;
            SPIJunk= SPI_READ_REG;
        }

    M25P_FLASH_DIS;
}


void M25PE_PW(uint32_t PageAddress, uint8_t *WriteBuffer)
{
    uint16_t k;
    M25PE_WREN();
    M25P_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = PW;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(PageAddress>>16);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(PageAddress>>8);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //LSB must be page aligned (ELi's Rule);
    SPI_WRITE_REG = 0;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk

    for(k=0; k<256; k++)
        {
            SPI_WRITE_REG = WriteBuffer[k];
            WAIT_FOR_SPI;
            SPIJunk= SPI_READ_REG;
        }

    M25P_FLASH_DIS;
}

void M25PE_PE(uint32_t PageAddress)
{
    uint16_t k;
    M25PE_WREN();
    M25P_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = PE;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(PageAddress>>16);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(PageAddress>>8);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //LSB must be page aligned (ELi's Rule);
    SPI_WRITE_REG = 0;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    M25P_FLASH_DIS;
}
void M25PE_BE()
{
    M25PE_WREN();
    M25P_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = BE;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    M25P_FLASH_DIS;
}

//Puts the device into DEEP power save Mode
void M25PE_DP()
{
    //First Bring chips select LOw
    M25P_FLASH_EN;
    SPI_WRITE_REG = DP;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    M25P_FLASH_DIS;
}

//Brings the Device out of Low Power Mode.
void M25PE_RES()
{
    //First Bring chips select LOw
    M25P_FLASH_EN;
    //send out resume instruction
    SPI_WRITE_REG = RES;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    M25P_FLASH_DIS;
}


