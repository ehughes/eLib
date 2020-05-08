#include "System.h"
#include "W25Q_FLASH.h"

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
#define SE4K		0x20
#define BE 			0xC7
#define DP 			0xB9
#define RES         0xAB
#define PE			0xDB
#define PW			0x0A


#define W25Q_FLASH_EN				SSP1_CS_NOR_ACTIVE
#define W25Q_FLASH_DIS				SSP1_CS_NOR_INACTIVE
#define FRAM_WAIT_FOR_SPI           STORAGE_SPI_WAIT_FOR_SPI
#define FRAM_SPI_WRITE_REG          STORAGE_SPI_WRITE_REG
#define FRAM_SPI_READ_REG           STORAGE_SPI_READ_REG

#define WAIT_FOR_SPI	 STORAGE_SPI_WAIT_FOR_SPI
#define SPI_WRITE_REG    STORAGE_SPI_WRITE_REG
#define SPI_READ_REG     STORAGE_SPI_READ_REG

//This variable is used to get rid of compiler warnings of setting a variable and never reading it.
//In all the routines where I have to read back the SPI data after a write,  I have to have a location
//to dump the value that is modified somewhere else.   SPIJunk is used for this purpose.
static uint8_t SPIJunk;

//A high level function to read up to 65k bytes from the Flash device
void  W25Q_FlashRead(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer)
{
    uint16_t k;
    W25Q_FLASH_EN;
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

    W25Q_FLASH_DIS;
}

//A high level function to wait for the Program/Erase to complete
void W25Q_BlockUntilFlashOperationComplete()
{
    uint8_t FLASHStatusByte;
    FLASHStatusByte = W25Q_RDSR();

    while((FLASHStatusByte&WIP) > 0)
        {
            FLASHStatusByte = W25Q_RDSR();
        }
}

void W25Q_WREN()
{
    //First Bring chip select Low
    W25Q_FLASH_EN;
    //send out Write Enable Instruction.
    SPI_WRITE_REG = WREN;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    W25Q_FLASH_DIS;
}

//This function acquires the identification from the W25QX device (Blocking SPI)
void W25Q_RDID(uint8_t *ID)
{
    uint8_t i;
    //First Bring chip select Low
    W25Q_FLASH_EN;
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

    W25Q_FLASH_DIS;
}

uint8_t W25Q_RDSR()
{
    uint8_t RDSRVal;
    //First Bring chip select LOw
    W25Q_FLASH_EN;
    //send out read status register instruction
    SPI_WRITE_REG = RDSR;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //First Byte back is Junk
    //Send out Dummy Byte to get status reg
    SPI_WRITE_REG = 0xFF;
    WAIT_FOR_SPI;
    RDSRVal = SPI_READ_REG;
    W25Q_FLASH_DIS;
    return RDSRVal;
}

void W25Q_WRSR(uint8_t StatusReg)
{
    W25Q_WREN();
    //First Bring chip select LOW
    W25Q_FLASH_EN;
    //send out Write Status Reg instruction
    SPI_WRITE_REG = WRSR;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    //Send out New Status Register value
    SPI_WRITE_REG = StatusReg;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG;
    W25Q_FLASH_DIS;
}



void W25Q_READ(uint32_t SectorAddress, uint8_t *ReadBuffer)
{
    uint16_t k;
    W25Q_FLASH_EN;
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

    W25Q_FLASH_DIS;
}



void W25Q_4KSE(uint32_t SectorAddress)
{
    W25Q_WREN();
    W25Q_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = SE4K;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    //send out Address
    SPI_WRITE_REG = (uint8_t)(SectorAddress>>16);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //MB must be sector aligned
    SPI_WRITE_REG = (uint8_t)(SectorAddress>>8);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    //LSB must be sector Aligned
    SPI_WRITE_REG = (uint8_t)(SectorAddress);
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //junk
    W25Q_FLASH_DIS;
}


void W25Q_PP(uint32_t PageAddress, uint8_t *WriteBuffer)
{
    uint16_t k;
    W25Q_WREN();
    W25Q_FLASH_EN;
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

    W25Q_FLASH_DIS;
}


void W25Q_PW(uint32_t PageAddress, uint8_t *WriteBuffer)
{
    uint16_t k;
    W25Q_WREN();
    W25Q_FLASH_EN;
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

    W25Q_FLASH_DIS;
}

void W25Q_PE(uint32_t PageAddress)
{

    W25Q_WREN();
    W25Q_FLASH_EN;
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
    W25Q_FLASH_DIS;
}
void W25Q_BE()
{
    W25Q_WREN();
    W25Q_FLASH_EN;
    //send out Read Instruction
    SPI_WRITE_REG = BE;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    W25Q_FLASH_DIS;
}

//Puts the device into DEEP power save Mode
void W25Q_DP()
{
    //First Bring chips select LOw
    W25Q_FLASH_EN;
    SPI_WRITE_REG = DP;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    W25Q_FLASH_DIS;
}

//Brings the Device out of Low Power Mode.
void W25Q_RES()
{
    //First Bring chips select LOw
    W25Q_FLASH_EN;
    //send out resume instruction
    SPI_WRITE_REG = RES;
    WAIT_FOR_SPI;
    SPIJunk = SPI_READ_REG; //Junk
    W25Q_FLASH_DIS;
}



