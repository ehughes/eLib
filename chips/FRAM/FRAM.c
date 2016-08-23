#include "HardwareProfile.h"
#include <stdint.h>
#include "FRAM.h"
#include <plib.h>

//#define FM25V10

#define WIP                     0x01
#define WREN                    0x06
#define WRDI                    0x04
#define RDSR                    0x05
#define WRSR                    0x01
#define READ                    0x03
#define FAST_READ               0x0B
#define WRITE                   0x02
#define SLEEP			0xB9
#define RDID                    0x9f
#define SNR                     0x9f




#define FRAM_BIT_RATE             2000000
#define FRAM_WAIT_FOR_SPI            while(SPI1STATbits.SPIRBF == 0){}
#define FRAM_SPI_WRITE_REG           SPI1BUF
#define FRAM_SPI_READ_REG           SPI1BUF

//This variable is used to get rid of compiler warnings of setting a variable and never reading it.
//In all the routines where I have to read back the SPI data after a write,  I have to have a location
//to dump the value that is modified somewhere else.   FRAM_SPI_Junk is used for this purpose.
static uint8_t FRAM_SPI_Junk;

//Hward dependent part.  Init the SPI controller here
void InitFRAM()
{
    uint8_t Junk;
    int32_t BRG;

    FRAM_EN_PIN_CONFIG;

    FRAM_DIS;
    SPI1CON = 0; //reset everything... (This will get us 8 bit with no automated slave select)
    SPI1CONbits.MSTEN = 1; //Select Master Mode
    SPI1CONbits.CKP = 0;
    SPI1CONbits.CKE = 1;
    SPI1CONbits.SMP =0;
    SPI1STATbits.SPIROV = 0; //Clear recieve overflow (per datasheet recommendation)
  /* SPI Already setup by SD Card or M25PEFLASH
    BRG = (GetPeripheralClock()/(2*FRAM_BIT_RATE))-1;

    if(BRG<0)
        BRG = 0;

    SPI1BRG = BRG & 0x1FFF;
    Junk = SPI2BUF; //Clear the Recieve buffer
    SPI1CONbits.ON = 1;  //Enable the SPI port
    */
}


//A high level function to read up to 65k bytes from the Flash device
void  FRAM_Read(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer)
{
    uint16_t k;
    FRAM_EN;
    //send out Read Instruction
    FRAM_SPI_WRITE_REG = READ;
    FRAM_WAIT_FOR_SPI;
    DataBuffer[0] = FRAM_SPI_READ_REG; //First Byte back is Junk
    //send out Address
    #ifdef FM25V10
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>16);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>8);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    #endif
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG; //Junk


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
    //DataBuffer[0] = FRAM_SPI_READ_REG; //First Byte back is Junk //First Byte back is Junk <- why are you over writing the data buffer?1?!?
    FRAM_SPI_Junk = FRAM_SPI_READ_REG;//Junk
    //send out Address
    #ifdef FM25V10
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>16);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>8);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    #endif
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG; //Junk


    for(k=0; k<Length; k++)
        {
            FRAM_SPI_WRITE_REG = DataBuffer[k];
            FRAM_WAIT_FOR_SPI;
            FRAM_SPI_Junk = FRAM_SPI_READ_REG;
        }

    FRAM_DIS;
}

//A high level function to Write up to 65k bytes of a single value to an area of the FRAM
void  FRAM_WriteConst(uint32_t StartAddress, uint16_t Length, uint8_t Data)
{
    uint16_t k;
 //Set the write enable ltach
    FRAM_WREN();
    FRAM_EN;
    //send out Read Instruction
    FRAM_SPI_WRITE_REG = WRITE;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG; //First Byte back is Junk
    //send out Address
    #ifdef FM25V10
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>16);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress>>8);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG;//Junk
    #endif
    FRAM_SPI_WRITE_REG = (uint8_t)(StartAddress);
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk=FRAM_SPI_READ_REG; //Junk

    for(k=0; k<Length; k++)
        {
            FRAM_SPI_WRITE_REG = Data;
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

//This function acquires the identification from the M25PX device (Blocking SPI)
void FRAM_RDID(uint8_t *ID)
{
    uint8_t i;
    //First Bring chip select Low
    FRAM_EN;
    //send out ID instruction
    FRAM_SPI_WRITE_REG = RDID;
    FRAM_WAIT_FOR_SPI;
    FRAM_SPI_Junk = FRAM_SPI_READ_REG; //is Junk

    //Now, send three dummy bytes

    for(i=0; i<3; i++)
        {
            FRAM_SPI_WRITE_REG = 0xff;
            FRAM_WAIT_FOR_SPI;
            ID[i] = FRAM_SPI_READ_REG; //Junk
        }

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


