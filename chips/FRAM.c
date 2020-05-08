#include "System.h"
#include "FRAM.h"

//This is a simple driver for the FM25CL series



#define WIP                     0x01


//Internal SPI Registers
#define WREN                    0x06
#define WRDI                    0x04
#define RDSR                    0x05
#define WRSR                    0x01
#define READ                    0x03
#define WRITE                   0x02


#define FRAM_EN						SSP1_CS_FRAM_ACTIVE
#define FRAM_DIS					SSP1_CS_FRAM_INACTIVE
#define FRAM_WAIT_FOR_SPI           STORAGE_SPI_WAIT_FOR_SPI
#define FRAM_SPI_WRITE_REG          STORAGE_SPI_WRITE_REG
#define FRAM_SPI_READ_REG           STORAGE_SPI_READ_REG

//This variable is used to get rid of compiler warnings of setting a variable and never reading it.
//In all the routines where I have to read back the SPI data after a write,  I have to have a location
//to dump the value that is modified somewhere else.   FRAM_SPI_Junk is used for this purpose.
static uint8_t FRAM_SPI_Junk;


//A high level function to read up to 65k bytes from the Flash device
void  FRAM_Read(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer)
{
    uint16_t k;
    FRAM_EN;
    //send out Read Instruction
    FRAM_SPI_WRITE_REG = READ;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG; //First Byte back is Junk
    //send out Address

    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>16);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>8);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk

    for(k=0; k<Length; k++)
        {
            FRAM_SPI_WRITE_REG = (uint8_t)(0xFF);
            FRAM_WAIT_FOR_SPI;
            DataBuffer[k]= FRAM_SPI_READ_REG;
        }

    FRAM_DIS;
}

//A high level function to Write up to 65k bytes from the Flash device
void  FRAM_Write(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer)
{
    uint16_t k;

    //Set the write enable ltach
    FRAM_WREN();

    FRAM_EN;
    //send out Read Instruction
    FRAM_SPI_WRITE_REG = WRITE;
    FRAM_WAIT_FOR_SPI;

    FRAM_SPI_Junk = FRAM_SPI_READ_REG;//Junk

    //send out Address
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>16);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>8);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk


    for(k=0; k<Length; k++)
        {
            FRAM_SPI_WRITE_REG = DataBuffer[k];
            FRAM_WAIT_FOR_SPI;
            FRAM_SPI_Junk = FRAM_SPI_READ_REG;
        }

    FRAM_DIS;
}


void FRAM_WREN()
{
    //First Bring chip select Low
    FRAM_EN;
    //send out Write Enable Instruction.
    FRAM_SPI_WRITE_REG = WREN;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG; //First Byte back is Junk
    FRAM_DIS;
}


void FRAM_WRDI()
{
    //First Bring chip select Low
    FRAM_EN;
    //send out Write Enable Instruction.
    FRAM_SPI_WRITE_REG = WRDI;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG; //First Byte back is Junk
    FRAM_DIS;
}


uint8_t FRAM_RDSR()
{
    uint8_t RDSRVal;
    //First Bring chip select LOw
    FRAM_EN;
    //send out read status register instruction
    FRAM_SPI_WRITE_REG = RDSR;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG; //First Byte back is Junk
    //Send out Dummy Byte to get status reg
    FRAM_SPI_WRITE_REG = 0xFF;
    FRAM_WAIT_FOR_SPI;
    RDSRVal = FRAM_SPI_READ_REG;
    FRAM_DIS;
    return RDSRVal;
}

void FRAM_WRSR(uint8_t StatusReg)
{
    FRAM_WREN();
    //First Bring chip select LOW
    FRAM_EN;
    //send out Write Status Reg instruction
    FRAM_SPI_WRITE_REG = WRSR;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG; //Junk
    //Send out New Status Register value
    FRAM_SPI_WRITE_REG = StatusReg;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG;
    FRAM_DIS;
}


