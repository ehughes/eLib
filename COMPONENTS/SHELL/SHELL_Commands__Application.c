#include "System.h"

extern tzero_sensor_collector_server_t SensorServer;

cmd_function_t wipe(p_shell_context_t Shell,int32_t argc, char **argv)
{

      SHELL_printf(Shell,"Wiping mesh provisioning data\r\n");

        mesh_stack_config_clear();
        Delay_mS(500);
        mesh_stack_device_reset();

        return 0;
}

cmd_function_t relay(p_shell_context_t Shell,int32_t argc, char **argv)
{
    if(argc<2)
    {
       SHELL_printf(Shell,"I need an argument of 1 or 0\r\n");
    }
    else
    {
        int Arg = 0;

        sscanf(argv[1],"%i",&Arg);

        if(Arg == 0)
        {
              SHELL_printf(Shell,"Relay is disabled \r\n");

              Mesh_RelayDisable();
        }
        else
        {
              SHELL_printf(Shell,"Relay is enabled\r\n");

              Mesh_RelayEnable();
        }

    }

    return 0;
}



cmd_function_t rstat(p_shell_context_t Shell,int32_t argc, char **argv)
{
      SHELL_printf(Shell,"\r\nMesh Relay Status\r\n");
      SHELL_printf(Shell,"----------------------------------------------\r\n\r\n"); 
  
      SHELL_printf(Shell,"Messages Relayed : %d\r\n\r\n",MyMeshRelayStatus.TotalMessagesRelayed); 

      SHELL_printf(Shell,"Node      Time Since Last Relay\r\n");
  

      for(int i=0;i<RELAY_STAT_ENTRIES;i++)
      {
          if(MyMeshRelayStatus.StatEntries[i].SecondsSinceLast != MESH_RELAY_PRUNE_TIME)
          {
                  SHELL_printf(Shell,"0x%04x    %d\r\n",
                                MyMeshRelayStatus.StatEntries[i].SrcAddress,
                                MyMeshRelayStatus.StatEntries[i].SecondsSinceLast);
          }
      
      }

      SHELL_printf(Shell,"\r\n");
}

cmd_function_t mstat(p_shell_context_t Shell,int32_t argc, char **argv)
{
        dsm_handle_t MyPublishAddressHandle;
         
        dsm_handle_t MyAppKeyHandles[DSM_APP_MAX];

        uint16_t MyAppKeyCount;

        nrf_mesh_address_t MyPublishAddress;

        nrf_mesh_secmat_t MySecMat;

        dsm_handle_t  devkey_handle;

        uint8_t SubNetkey[16];

        mesh_key_index_t SubnetKeyIndex[DSM_SUBNET_MAX];

        uint32_t  SubnetKeyCount;

        SHELL_printf(Shell,"\r\nMesh Status and Configuration\r\n");
        SHELL_printf(Shell,"----------------------------------------------\r\n\r\n");


     //   if(Mesh_IsNodeProvisioned() == FALSE)
      //  {
       //       SHELL_printf(Shell,"This node has not been provisioned.\r\n\r\n");
        //}
        //else
        {
            uint32_t AddressCount = 0;
            uint32_t BaseAddress = 0;


            AddressCount =  Mesh_GetUnicastAddress(&BaseAddress);

            SHELL_printf(Shell,"Node Address(es) : ");

            for(int i=0;i<AddressCount;i++)
            {
                   SHELL_printf(Shell,"0x%04x  ",i+BaseAddress);
            }
      
            SHELL_printf(Shell,"\r\n");

            SubnetKeyCount = DSM_SUBNET_MAX;

            dsm_subnet_get_all(&SubnetKeyIndex[0],&SubnetKeyCount);

            SHELL_printf(Shell,"Subnets : %d\r\n",SubnetKeyCount);

            for(int j=0;j<SubnetKeyCount;j++)
            {
              dsm_subnet_key_get(j, SubNetkey);

              SHELL_printf(Shell,"    Subnet Key: ");
              for(int i=0;i<16;i++)
              {
                  SHELL_printf(Shell,"%02x", SubNetkey[i]);
              }
          
              SHELL_printf(Shell,"\r\n");
            }
         
            access_model_publish_address_get(SensorServer.model_handle, &MyPublishAddressHandle);

            dsm_address_get(0, &MyPublishAddress);

            switch(MyPublishAddress.type)
            {
                 case  NRF_MESH_ADDRESS_TYPE_INVALID:

                  SHELL_printf(Shell,"Publish Address: Invalid\r\n");
                 break;
                 
                 case NRF_MESH_ADDRESS_TYPE_UNICAST:

                 SHELL_printf(Shell,"Publish Address (Unicast): 0x%0x4\r\n",MyPublishAddress.value);

                 break;

                 case NRF_MESH_ADDRESS_TYPE_VIRTUAL:

                 SHELL_printf(Shell,"Publish Address (Virtual): 0x%0x4\r\n",MyPublishAddress.value);

                 for(int i=0;i<16;i++)
                 {
                     SHELL_printf(Shell,"%02X",MyPublishAddress.p_virtual_uuid[i]);
                 }

                 SHELL_printf(Shell,"\r\n");

                 break;

                 case NRF_MESH_ADDRESS_TYPE_GROUP:

                 SHELL_printf(Shell,"Publish Address (Group): 0x%04x\r\n",MyPublishAddress.value);

                 break;

            }

            MyAppKeyCount = DSM_APP_MAX;
            access_model_applications_get(SensorServer.model_handle,
                                                   MyAppKeyHandles,
                                                   &MyAppKeyCount);


           SHELL_printf(Shell,"Application Keys Stored: %d\r\n",MyAppKeyCount);

           for(int j=0;j<MyAppKeyCount;j++)
           {
             dsm_tx_secmat_get(0xFFFF,MyAppKeyHandles[j],&MySecMat);

              SHELL_printf(Shell,"    App Key : ");
              
              for(int i=0;i<16;i++)
              {
                  SHELL_printf(Shell,"%02x", MySecMat.p_app->key[i]);
              }
          
             SHELL_printf(Shell,"\r\n");
             }

             if(Mesh_IsNodeRelay())
             {
                SHELL_printf(Shell,"\r\nThis node is a relay. \r\n");
             }
            else
             {
                SHELL_printf(Shell,"\r\nThis node is not a relay. \r\n");
             }
        }
        return 0;
}

#if ENABLE_CONDUCTIVITY

cmd_function_t ec_check(p_shell_context_t Shell,int32_t argc, char **argv)
{

  EC_OEM_DeviceInfo EC;
  EC_OEM_GetDeviceInfo(&EC);

  SHELL_printf(Shell,"\r\nEC-OEM Device Info:\r\n");
  SHELL_printf(Shell,"------------------------------\r\n");
  SHELL_printf(Shell,"Device Type : 0x%02X\r\n",EC.DeviceType);
  SHELL_printf(Shell,"Firmware Version : 0x%02X\r\n",EC.FirmwareVersion);
  SHELL_printf(Shell,"ProbeType : %d   K=%.02f\r\n",EC.ProbeType.U_Int16,EC.K);


  SHELL_printf(Shell,"Calibration: ");

   if(EC.CalibrationConfirmation == 0x0)
   {
       SHELL_printf(Shell,"None\r\n");
   }
   else
   {
      if(EC.CalibrationConfirmation & 0x01)
        SHELL_printf(Shell," [Dry]");
      if(EC.CalibrationConfirmation & 0x02)
        SHELL_printf(Shell," [Single]");
      if(EC.CalibrationConfirmation & 0x04)
        SHELL_printf(Shell," [Low]");
      if(EC.CalibrationConfirmation & 0x08)
        SHELL_printf(Shell," [High]");

      SHELL_printf(Shell,"\r\n");
   }

  SHELL_printf(Shell,"Temperature Compensation: %.02fC\r\n",EC.TemperatureConfirmation);

  SHELL_printf(Shell,"LastReading: %.1f uS ",(float)(EC.LastReading.U_Int32)/100.0f);

  
  SHELL_printf(Shell,"\r\n\r\n",EC.ProbeType);

  return 0;
}

cmd_function_t ec_start(p_shell_context_t Shell,int32_t argc, char **argv)
{
  EC_StartStop(TRUE);
}

cmd_function_t ec_stop(p_shell_context_t Shell,int32_t argc, char **argv)
{
  EC_StartStop(FALSE);
}

cmd_function_t ec_cal_clear(p_shell_context_t Shell,int32_t argc, char **argv)
{
  EC_SendCalibration(0 , EC_OEM_CAL_TYPE__CLEAR);
}

cmd_function_t ec_cal_dry(p_shell_context_t Shell,int32_t argc, char **argv)
{
  EC_SendCalibration(0 , EC_OEM_CAL_TYPE__DRY);
}

cmd_function_t ec_cal_single(p_shell_context_t Shell,int32_t argc, char **argv)
{
   float CalValue;

   if(argc < 2)
   {
        SHELL_printf(Shell,"I need 1 argument with the cal value in uS.\r\n");
        return 0;
   }

   sscanf(argv[1],"%f",&CalValue);

   SHELL_printf(Shell,"Sending single calibration of %.1f\r\n",CalValue);

   EC_SendCalibration(CalValue , EC_OEM_CAL_TYPE__SINGLE);
}

cmd_function_t ec_cal_low(p_shell_context_t Shell,int32_t argc, char **argv)
{
   float CalValue;

   if(argc < 2)
   {
        SHELL_printf(Shell,"I need 1 argument with the cal value in uS.\r\n");
        return 0;
   }

   sscanf(argv[1],"%f",&CalValue);

   SHELL_printf(Shell,"Sending dual low calibration of %.1f\r\n",CalValue);

   EC_SendCalibration(CalValue , EC_OEM_CAL_TYPE__DUAL_LOW);
}

cmd_function_t ec_cal_high(p_shell_context_t Shell,int32_t argc, char **argv)
{
  float CalValue;

   if(argc < 2)
   {
        SHELL_printf(Shell,"I need 1 argument with the cal value in uS.\r\n");
        return 0;
   }

   sscanf(argv[1],"%f",&CalValue);

   SHELL_printf(Shell,"Sending dual high calibration of %.1f\r\n",CalValue);

   EC_SendCalibration(CalValue , EC_OEM_CAL_TYPE__DUAL_HIGH);
}

cmd_function_t ec_read(p_shell_context_t Shell,int32_t argc, char **argv)
{
   float Value = (float)EC_OEM_GetRawReading()/100.0f;

   SHELL_printf(Shell,"Conductivity : %.1fuS\r\n",Value);

}

cmd_function_t ec_temp(p_shell_context_t Shell,int32_t argc, char **argv)
{
   float Temp;

   if(argc < 2)
   {
        SHELL_printf(Shell,"I need 1 argument with the temp value in degrees C.\r\n");
        return 0;
   }

   sscanf(argv[1],"%f",&Temp);

   SHELL_printf(Shell,"Sending a temperature value  of %.02fC\r\n",Temp);

   EC_SendTemperature(Temp);
}
#endif

#if ENABLE_BME280 == 1
cmd_function_t bme(p_shell_context_t Shell,int32_t argc, char **argv)
{
  // uint8_t ID = BME280_ID();
   
   //SHELL_printf(Shell,"\r\nBME280 Device Info:\r\n");
   //SHELL_printf(Shell,"------------------------------\r\n");

//   SHELL_printf(Shell,"ID : 0x%02x\r\n",ID);

    struct bme280_data comp_data;

    bme280_get_sensor_data(BME280_ALL, &comp_data, &MyBME280);

        #ifdef BME280_FLOAT_ENABLE
                printf("%0.2f, %0.2f, %0.2f\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
        #else
                SHELL_printf(Shell,"%.2ff, %.1f, %.1f\r\n",comp_data.temperature/100.0f, comp_data.pressure/100.0f, comp_data.humidity/1024.0f);
        #endif

}
#endif


#if ENABLE_MAX3185 == 1
cmd_function_t rtd(p_shell_context_t Shell,int32_t argc, char **argv)
{
        
       SHELL_printf(Shell,"r: %.2f t: %.2f\r\n",MAX31865_GetResistance(),MAX31865_GetTemperature());
 
       return 0;

}
#endif