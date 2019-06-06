#include "nrf.h"
#include "Shell.h"
#include "SHELL_Commands.h"
#include "SHELL_Commands__Application.h"
#include "SHELL_Commands__FileSystem.h"
#include "Shell_Commands__Configuration.h"
#include "SHELL_Commands__Cell.h"
#include "SHELL_Commands__Bridge.h"

#include "System.h"
#include "Board.h"
#include "nrf_mesh_defines.h"
#include "SEGGER_RTT.h"
#include "system_nrf52840.h"
#include "Drivers/SC16IS741A/SC16IS741A.h"

#define SHELL_SERIAL_QUEUE_SIZE 2048

shell_context_struct MySerialShell;


ByteQueue ShellInputQueue;
ByteQueue ShellOutputQueue;

uint8_t ShellInputQueueStorage[SHELL_SERIAL_QUEUE_SIZE];
uint8_t ShellOutputQueueStorage[SHELL_SERIAL_QUEUE_SIZE];

ByteQueue Shell_Uart_OutputQueue;
uint8_t Shell_Uart_OutputQueueStorage[SHELL_SERIAL_QUEUE_SIZE];

uint32_t ShellHasBeenInitialized = 0;

uint8_t ShellTempOutputBuffer[SHELL_SERIAL_QUEUE_SIZE];


void Init_Shell()
{

        Init_Shell_IO();

        InitByteQueue(&ShellInputQueue, SHELL_SERIAL_QUEUE_SIZE, ShellInputQueueStorage);
        InitByteQueue(&ShellOutputQueue, SHELL_SERIAL_QUEUE_SIZE, ShellOutputQueueStorage);
        InitByteQueue(&Shell_Uart_OutputQueue, SHELL_SERIAL_QUEUE_SIZE, Shell_Uart_OutputQueueStorage);

        MySerialShell.ShellInQueue = &ShellInputQueue;
        MySerialShell.ShellOutQueue = &ShellOutputQueue;

        MySerialShell.echo = 0;
        MySerialShell.Description = "";
        MySerialShell.prompt =">";
        MySerialShell.CurrentPrivilegeLevel = 0;



        SHELL_RegisterCommand(&MySerialShell,
                                                                  "help", 
                                                                  "List all available Commands",    
                                                                  (cmd_function_t)HelpCommand ,
                                                                  0
                                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                                  "cow", 
                                                                  "it moos",    
                                                                  (cmd_function_t)(cow) 
                                                                  ,0
                                                                  );


        SHELL_RegisterCommand(&MySerialShell,
                                                  "i", 
                                                  "Enter interactive mode",    
                                                  (cmd_function_t)(i) 
                                                  ,0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "q", 
                                                  "Enter quiet mode",    
                                                  (cmd_function_t)(q) 
                                                  ,0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "reboot", 
                                                  "software reset",    
                                                  (cmd_function_t)(reset),
                                                  0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "debug", 
                                                  "sets the current debug message level",    
                                                  (cmd_function_t)(debug),
                                                  0
                                                  );


        SHELL_RegisterCommand(&MySerialShell,
                                                  "wipe", 
                                                  "Resets the mesh node",    
                                                  (cmd_function_t)(wipe), 
                                                  0
                                                  );

#ifdef eli
        SHELL_RegisterCommand(&MySerialShell,
                                                                  "shell_test", 
                                                                  "Use this to show the parsed arguments.",    
                                                                  (cmd_function_t)(shell_test)
                                                                        ,0                                                                
                                                                  );


        SHELL_RegisterCommand(&MySerialShell,
                                                                  "flash_info", 
                                                                  "Gets the JEDEC Flash codes",    
                                                                                                                                        
                                                                  (cmd_function_t)(flash_info) ,
                                                                  0
                                                                  );


                                                                        SHELL_RegisterCommand(&MySerialShell,
                                                  "erase_flash", 
                                                  "erases the entire flash",    
                                                                                                                
                                                  (cmd_function_t)(erase_flash) ,
                                                  0
                                                  );
#endif

        SHELL_RegisterCommand(&MySerialShell,
                                                                  "flash_read", 
                                                                  "reads a flash sector",    
                                                                                                                                        
                                                                  (cmd_function_t)(flash_sector_read),
                                                                  0
                                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "test_flash", 
                                                  "tests the flash array",    
                                                                                                                
                                                  (cmd_function_t)(test_flash),
                                                  0
                                                  );



        SHELL_RegisterCommand(&MySerialShell,
                                                  "format", 
                                                  "formats the internal flash",    
                                                                                                                
                                                  (cmd_function_t)(format) ,
                                                  0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "ls", 
                                                  "directory listing",    
                                                  (cmd_function_t)(ls), 
                                                  0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "touch", 
                                                  "creates a file",    
                                                  (cmd_function_t)(touch),
                                                  0
                                                  );


        SHELL_RegisterCommand(&MySerialShell,
                                                  "rm", 
                                                  "deletes a file",    
                                                                                                                
                                                  (cmd_function_t)(touch) 
                                                  ,0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "test_fs", 
                                                  "tests the file system",    
                                                                                                                
                                                  (cmd_function_t)(test_fs) 
                                                  ,0
                                                  );


        SHELL_RegisterCommand(&MySerialShell,
                                                  "more", 
                                                  "dumps a text file",    
                                                  (cmd_function_t)(more)
                                                  ,0                                                
                                                  );


        SHELL_RegisterCommand(&MySerialShell,
                                                  "check_fs", 
                                                  "checks the flash file system",    
                                                                                                                
                                                  (cmd_function_t)(check_fs) 
                                                  ,0
                                                  );

      SHELL_RegisterCommand(&MySerialShell,
                                                  "rename", 
                                                  "renames a file",    
                                                                                                                
                                                  (cmd_function_t)(file_rename) 
                                                  ,0
                                                  );


        SHELL_RegisterCommand(&MySerialShell,
                                                  "get",
                                                  "gets a parameter",
                                                  (cmd_function_t)(get),
                                                  0
                                                  
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "set",
                                                  "sets a parameter",
                                                  (cmd_function_t)(set),
                                                  0
                                                  
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "save",
                                                  "saves the current configuration parameters",
                                                  (cmd_function_t)(save_config),
                                                  0
                                                  
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "load",
                                                  "forces a load of configuration parameters from configuration file",
                                                  (cmd_function_t)(load_config),
                                                  0
                                                  
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "mstat",
                                                  "Gets the current mesh status",
                                                  (cmd_function_t)(mstat),
                                                  0
                                                  );


         SHELL_RegisterCommand(&MySerialShell,
                                                  "relay",
                                                  "Temporarily enables or disables the relay feature",
                                                  (cmd_function_t)(relay),
                                                  0
                                                  );

         SHELL_RegisterCommand(&MySerialShell,
                                                  "rstat",
                                                  "Gets relay statistics",
                                                  (cmd_function_t)(rstat),
                                                  0
                                                  );

         SHELL_RegisterCommand(&MySerialShell,
                                                  "at",
                                                  "sends an AT command to the cell modem",
                                                  (cmd_function_t)(cell_at),
                                                  0
                                                  );

         SHELL_RegisterCommand(&MySerialShell,
                                                  "AT",
                                                  "sends an AT command to the cell modem",
                                                  (cmd_function_t)(cell_at),
                                                  0
                                                  );


          SHELL_RegisterCommand(&MySerialShell,
                                                  "txt",
                                                  "sends an SMS text",
                                                  (cmd_function_t)(cell_txt),
                                                  0
                                                  );

          SHELL_RegisterCommand(&MySerialShell,
                                                  "cell",
                                                  "gets cell information",
                                                  (cmd_function_t)(cell),
                                                  0
                                                  );

          SHELL_RegisterCommand(&MySerialShell,
                                                  "wolftest",
                                                  "test the wolfcrypt backend",
                                                  (cmd_function_t)(wolf_test),
                                                   0
                                                  );

      



      #ifdef eli

        SHELL_RegisterCommand(&MySerialShell,
                                                  "fopen", 
                                                  "opens a file",    
                                                                                                                
                                                  (cmd_function_t)(Remote_fopen),
                                                  0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "fclose", 
                                                  "closes the open file",    
                                                                                                                
                                                  (cmd_function_t)(Remote_fclose),
                                                  0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "fwrite", 
                                                  "write a line of text to a file",    
                                                                                                                
                                                  (cmd_function_t)(Remote_fwrite),
                                                  0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "fwriteline", 
                                                  "write a line of text to a file",    
                                                  (cmd_function_t)(Remote_fwriteline) ,
                                                  0
                                                  );



        SHELL_RegisterCommand(&MySerialShell,
                                                  "md5sum", 
                                                  "computes the md5 digest of a file",    
                                                  (cmd_function_t)(md5check) 
                                                  ,0
                                                  );

        SHELL_RegisterCommand(&MySerialShell,
                                                  "validate", 
                                                  "validate a boot image",    
                                                                                                                
                                                  (cmd_function_t)(validate_boot_file) 
                                                  );


                                                  );

#endif

    SHELL_printf(&MySerialShell,"\r\n");
    SHELL_printf(&MySerialShell,MySerialShell.prompt);

    ShellHasBeenInitialized = 1;
}

uint32_t ShellUART_TxActive = 0 ;

uint8_t UartInByte = 0;

#if ENABLE_SHELL_TO_SPI_SERIAL == 1

    #define SPI_UART_TX_QUEUE_SIZE  4096

    uint8_t SPI_UART_QueueStorage[SPI_UART_TX_QUEUE_SIZE];

    ByteQueue SPI_UART_TX_Q;

#endif

void Init_Shell_IO()
{
    ShellUART_TxActive = 0 ;

    #if ENABLE_SHELL_TO_SPI_SERIAL == 1

    InitByteQueue(&SPI_UART_TX_Q, SPI_UART_TX_QUEUE_SIZE, SPI_UART_QueueStorage);

    SC16IS741A_Init();

    #endif 

    #if ENABLE_SHELL_TO_UART == 1

    /* Initialize UART hardware: */
    NRF_UARTE1->PSELTXD = NRF_GPIO_PIN_MAP(SERIAL_IF_TXD_PORT_NUMBER,SERIAL_IF_TXD_PIN_NUMBER);
    NRF_UARTE1->PSELRXD = NRF_GPIO_PIN_MAP(SERIAL_IF_RXD_PORT_NUMBER,SERIAL_IF_RXD_PIN_NUMBER);
  
    NRF_UARTE1->CONFIG  = (HWFC ? UARTE_CONFIG_HWFC_Enabled : UARTE_CONFIG_HWFC_Disabled) << UART_CONFIG_HWFC_Pos;
    NRF_UARTE1->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud115200 << UARTE_BAUDRATE_BAUDRATE_Pos;
    NRF_UARTE1->ENABLE = UARTE_ENABLE_ENABLE_Enabled << UARTE_ENABLE_ENABLE_Pos;
    
 
    NRF_UARTE1->INTEN = UARTE_INTEN_ENDRX_Msk | UARTE_INTEN_ENDTX_Msk;


    NRF_UARTE1->TASKS_FLUSHRX = 1;
    NRF_UARTE1->RXD.MAXCNT = 1;

    NRF_UARTE1->RXD.PTR = &UartInByte;
    NRF_UARTE1->TASKS_STARTRX = 1;

    NVIC_SetPriority(UARTE1_IRQn, NRF_MESH_IRQ_PRIORITY_LOWEST);
    NVIC_EnableIRQ(UARTE1_IRQn);

    #endif

}

#if ENABLE_SHELL_TO_UART == 1


uint8_t  TxOutBuf[256];

uint32_t TxChunk()
{
    uint32_t TxScheduled = 0;

    uint32_t BytesInQ = BytesInQueue(&Shell_Uart_OutputQueue);

    if(BytesInQ)
    {
       if(BytesInQ>255)
            BytesInQ = 255;

        for(int i=0;i<BytesInQ;i++)
        {
            ByteDequeue(&Shell_Uart_OutputQueue,&TxOutBuf[i]);
        }

         NRF_UARTE1->TXD.MAXCNT = BytesInQ;
         NRF_UARTE1->TXD.PTR = &TxOutBuf[0];
         NRF_UARTE1->TASKS_STARTTX = 1;
         TxScheduled = 1;
    }
    
    return TxScheduled;
 }


void UARTE1_IRQHandler(void)
{

    if(NRF_UARTE1->EVENTS_ENDRX)
    {
        NRF_UARTE1->EVENTS_ENDRX = 0;
        NRF_UARTE1->TASKS_STARTRX = 1;
        if(NRF_UARTE1->RXD.AMOUNT == 1)
        {
             ByteEnqueue(&ShellInputQueue,UartInByte);
        }
    }

    if(NRF_UARTE1->EVENTS_ENDTX)
    {
        NRF_UARTE1->EVENTS_ENDTX = 0;
        NRF_UARTE1->TASKS_STOPTX = 1;
        ShellUART_TxActive = TxChunk();
    }

    /* transmit any pending bytes */
    //if (NRF_UART0->EVENTS_TXDRDY)
   // {
    //    NRF_UART0->EVENTS_TXDRDY = 0;
     //   (void) NRF_UART0->EVENTS_TXDRDY;

       // if(BytesInQueue(&Shell_Uart_OutputQueue) == 0)
       // {
        //    NRF_UART0->TASKS_STOPTX = 1;
           // ShellUART_TxActive = 0;
     //   }
      //  else
      //  { 
        //     uint8_t DataOut;

        //     ByteDequeue(&Shell_Uart_OutputQueue,&DataOut);

            // NRF_UART0->TXD = DataOut;
      //  }

  //  }
}

#endif



void Shell_MoveQueues()
{

    if(ShellHasBeenInitialized == 0)
        return;

   uint32_t NumBytesInOutputQueue = BytesInQueue(&ShellOutputQueue);
   
   uint8_t DataOut = 0;
   
   if(NumBytesInOutputQueue > 0)
    {
    
        for(int i=0;i<NumBytesInOutputQueue;i++)
        {

            ByteDequeue(&ShellOutputQueue,&DataOut);

            #if ENABLE_SHELL_TO_UART == 1
                ByteEnqueue(&Shell_Uart_OutputQueue,DataOut);
            #endif


            #if ENABLE_SHELL_TO_RTT == 1

                ShellTempOutputBuffer[i] = DataOut;

            #endif


            ByteEnqueue(&SPI_UART_TX_Q,DataOut);

        }

  
       #if ENABLE_SHELL_TO_RTT == 1  
            SEGGER_RTT_Write(0,&ShellTempOutputBuffer[0],NumBytesInOutputQueue);
       #endif

    }


      #if ENABLE_SHELL_TO_RTT == 1
          while(SEGGER_RTT_HasKey())
          {
                ByteEnqueue(&ShellInputQueue, SEGGER_RTT_GetKey());
          }
      #endif

    
       
        #if ENABLE_SHELL_TO_UART == 1
            if (ShellUART_TxActive == 0)
              {
                    ShellUART_TxActive = TxChunk();
              }
       
        #endif


    #if ENABLE_SHELL_TO_SPI_SERIAL == 1

    uint32_t BytesToTx;

    uint32_t MaxtoTx;
   
    BytesToTx = BytesInQueue(&SPI_UART_TX_Q);

    if(BytesToTx)
    {

        MaxtoTx = SC16IS741A_ReadRegister(SC16IS741A__TXLVL);

        if(BytesToTx>MaxtoTx)
            BytesToTx = MaxtoTx;

        if(BytesToTx>64)
            BytesToTx = 64;

        uint8_t Buf[BytesToTx];

        ByteArrayDequeue(&SPI_UART_TX_Q,Buf,BytesToTx);

        SC16IS741A_WriteTxFIFO(Buf,BytesToTx);
    
    }


    uint32_t BytesToRx = SC16IS741A_ReadRegister(SC16IS741A__RXLVL);

    if(BytesToRx)
    {
        if(BytesToRx>64)
            BytesToRx = 64;

        uint8_t Buf[BytesToRx];

        SC16IS741A_ReadRxFIFO(Buf,BytesToRx);

        for(int i=0;i<BytesToRx;i++)
        {
             ByteEnqueue(&ShellInputQueue,Buf[i]);
        }

    }
        
    #endif

}
