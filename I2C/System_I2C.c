#include "System.h"
#include "nrfx_twi.h"

#define TWI_INSTANCE_ID     0

/* TWI instance. */
static const nrfx_twi_t m_twi = NRFX_TWI_INSTANCE(TWI_INSTANCE_ID);

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

volatile nrfx_twi_evt_type_t LastEvent = 0;

void twi_handler(nrfx_twi_evt_t const * p_event, void * p_context)
{
    m_xfer_done = true;

    LastEvent = p_event->type;

}

#ifdef SYSTEM_I2C_RTOSIFY
        SemaphoreHandle_t System_I2C_Mutex;
#endif

I2C_Result System_I2C_Init()
{
      #ifdef SYSTEM_I2C_RTOSIFY
          System_I2C_Mutex = xSemaphoreCreateMutex();
      #endif


    const nrfx_twi_config_t twi_config = {
       .scl                = NRF_GPIO_PIN_MAP(BOARD_SCL_PORT_NUMBER,BOARD_SCL_PIN),
       .sda                = NRF_GPIO_PIN_MAP(BOARD_SDA_PORT_NUMBER,BOARD_SDA_PIN),
       .frequency          = TWI_FREQUENCY_FREQUENCY_K100,
       .interrupt_priority = _PRIO_APP_LOW,
       .hold_bus_uninit     = 1 
    };


    nrfx_twi_init(&m_twi, &twi_config, twi_handler, NULL);

    nrfx_twi_enable(&m_twi);

    return I2C_OK;
        
}


I2C_Result System_I2C_WriteRaw(uint8_t DeviceI2C_Address, uint8_t *RawData, uint8_t Length)
{
    I2C_Result RetVal;

     nrfx_err_t Result;

    #ifdef SYSTEM_I2C_RTOSIFY
                xSemaphoreTake(System_I2C_Mutex,portMAX_DELAY);
    #endif
   
     m_xfer_done = false;

     Result = nrfx_twi_tx(&m_twi,
                           DeviceI2C_Address,
                           RawData,
                           Length,
                           false);

    if(Result != NRF_SUCCESS)
    {
        RetVal = I2C_TX_ERROR;
    }
    else
    {
          while(m_xfer_done == false){}
  
          if(LastEvent == NRFX_TWI_EVT_ADDRESS_NACK ||
             LastEvent == NRFX_TWI_EVT_DATA_NACK)
          {
              RetVal = I2C_NACK;
          }
          else
          {
              RetVal = I2C_OK;
          }
    }

    #ifdef SYSTEM_I2C_RTOSIFY
        xSemaphoreGive(System_I2C_Mutex);
    #endif

        return Result;
}

I2C_Result System_I2C_ReadRaw(uint8_t DeviceI2C_Address, uint8_t *RawData, uint8_t Length)
{
     I2C_Result RetVal;

     nrfx_err_t Result;

    #ifdef SYSTEM_I2C_RTOSIFY
                xSemaphoreTake(System_I2C_Mutex,portMAX_DELAY);
    #endif
   
     m_xfer_done = false;

      
     Result = nrfx_twi_rx(&m_twi,
                           DeviceI2C_Address,
                           RawData,
                           Length
                          );

    if(Result != NRF_SUCCESS)
    {
        RetVal = I2C_RX_ERROR;
    }
    else
    {
          while(m_xfer_done == false){}
  
          if(LastEvent == NRFX_TWI_EVT_ADDRESS_NACK ||
             LastEvent == NRFX_TWI_EVT_DATA_NACK)
          {
              RetVal = I2C_NACK;
          }
          else
          {
              RetVal = I2C_OK;
          }
    }

    #ifdef SYSTEM_I2C_RTOSIFY
        xSemaphoreGive(System_I2C_Mutex);
    #endif

    return Result;
}

I2C_Result System_I2C_TxRxRaw(uint8_t DeviceI2C_Address, 
                                   uint8_t *TX_RawData, 
                                   uint8_t TX_Length,
                                   uint8_t *RX_RawData, 
                                   uint8_t RX_Length
                                   )
{
     I2C_Result RetVal;

     nrfx_err_t Result;

    #ifdef SYSTEM_I2C_RTOSIFY
                xSemaphoreTake(System_I2C_Mutex,portMAX_DELAY);
    #endif
   
    m_xfer_done = false;

    nrfx_twi_xfer_desc_t Transfer;
    
    Transfer.type = NRFX_TWI_XFER_TXRX;
    Transfer.address = DeviceI2C_Address;
    Transfer.p_primary_buf = TX_RawData;
    Transfer.primary_length = TX_Length;
    Transfer.p_secondary_buf = RX_RawData;
    Transfer.secondary_length = RX_Length;
  
   Result = nrfx_twi_xfer(&m_twi,
                          &Transfer,
                          0);

    if(Result != NRF_SUCCESS)
    {
        RetVal = I2C_TXRX_ERROR;
    }
    else
    {
          while(m_xfer_done == false){}
  
          if(LastEvent == NRFX_TWI_EVT_ADDRESS_NACK ||
             LastEvent == NRFX_TWI_EVT_DATA_NACK)
          {
              RetVal = I2C_NACK;
          }
          else
          {
              RetVal = I2C_OK;
          }
    }

    #ifdef SYSTEM_I2C_RTOSIFY
        xSemaphoreGive(System_I2C_Mutex);
    #endif

    return Result;
}



I2C_Result System_I2C_WriteRegister_8Bit(uint8_t DeviceI2C_Address,uint8_t Register, uint8_t Value_8Bit)
{
    uint8_t Buffer[2];
 
    Buffer[0] = Register;
    Buffer[1] = Value_8Bit;

    return  System_I2C_WriteRaw(DeviceI2C_Address,Buffer,sizeof(Buffer));
}

I2C_Result System_I2C_WriteRegister_16Bit(uint8_t DeviceI2C_Address, uint8_t Register, uint16_t Value_16Bit)
{
    uint8_t Buffer[3];
        
    Buffer[0] = Register;
    Buffer[1] = (Value_16Bit >> 8U) & 0xFFU;
    Buffer[2] = Value_16Bit & 0xFFU;

    return  System_I2C_WriteRaw(DeviceI2C_Address,Buffer,sizeof(Buffer));
}

I2C_Result System_I2C_ReadRegister_8Bit(uint8_t DeviceI2C_Address,uint8_t Register, uint8_t *Value_8Bit)
{

   return System_I2C_TxRxRaw(DeviceI2C_Address,&Register,1,Value_8Bit,1);

}


I2C_Result System_I2C_ReadRegister_16Bit(uint8_t DeviceI2C_Address,uint8_t Register, uint16_t *Value_16Bit)
{
    I2C_Result Result;
   
    uint8_t Buffer[2];
   
    Result =  System_I2C_TxRxRaw(DeviceI2C_Address,&Register,1,&Buffer[0],2);
   
    *Value_16Bit = (uint16_t)((((uint16_t)Buffer[0]) << 8U) | ((uint16_t)Buffer[1]));

    return Result;
}
