/*--------------------------------------------------------------------*/
/*
/* File Name: GPS.c
/* Author: Robert J. Ceschini
/* Classification: UNCLASSIFIED
/* Revision: 1.0.0
/* Date: 05/20/13
/*
/* Copyright 2013, The Pennsylvania State University,
/* Applied Research Laboratory ALL RIGHTS RESERVED.
/* Restricted license valid for internal evaluation purposes only.
/*  Software provided "AS IS". Additional rights available through separate license.
/* This software was developed through work supported by the United States
/* Army Logistics Innovation Agency under Contract N00024-12-D-6402
/*  delivery order number 0042.
/*
/* The US Government's rights in and to this copyrighted data
/* are as specified in DFARS 252.227-7013
/*
/* WARNING - THIS DOCUMENT CONTAINS TECHNICAL DATA WHOSE EXPORT IS
/* RESTRICTED BY THE ARMS EXPORTS CONTROL ACT (TITLE 22 U.S.C. SEC 2751 ET
/* SEQ.) OR THE EXPORT ADMINISTRATION ACT OF 1979. AS AMENDED, TITLE 50,
/* DISSEMINATE IN ACCORDANCE WITH PROVISIONS OF DOD DIRECTIVE 5230.25.
/*
/* Revision   Date   Initials        Description
/*  1.0.0    1/14/14    rjc       Initial revision
/*--------------------------------------------------------------------*/


#define THIS_IS_GPS


#include "..\System.h"
#include "GPS.h"
#include "Status.h"
#include <ctype.h>
#include <math.h>
uint8_t GPSCommand[100];
uint8_t TestBuff[100];
float NMEAToDecimalDegrees(float pNMEA){
    float iDeg;
    float tmpDeg;
    float tDeg=pNMEA/100;
    float ret;
    ret=modff(tDeg,&iDeg);
    tmpDeg=(iDeg*100);
    tDeg=(pNMEA - tmpDeg);
    tDeg=tDeg/60;
    ret=iDeg + tDeg;//((pNMEA - (tDeg*100))/60);
    return ret;
}
void GPSInit(void){

    GPS_ENABLE_TRIS=0;
    IsGPSOn=FALSE;
    PowerOnGPS();

    GPS_PPS_TRIS=1;
}
void PowerOffGPS(void){
    if(!USB_ACTIVE){
        IsGPSOn=FALSE;
        AIB_DebugPrint(DEBUG_GPS,"G-Turned off GPS\r\n");
        GPSOFF;        
    }

}
void PowerOnGPS(void){
    uint8_t i;
    uint8_t crc;
    if(!IsGPSOn){
        GPSStatusByte=0;
        GPSSendNow = FALSE;
        CurrentGPSInfo.GPSLock = FALSE;
        GPSGot1PPS = FALSE;
        GPSGotTimeMessage = FALSE;
        GPSNumSats = 0;
        GPSPPSTimeoutCounter = 0;
        GPSON;
        IsGPSOn = TRUE;
        DelayMs(1000);
            //              Setup for ZDA and GGA only
        sprintf(&GPSCommand[0],"$PTNLSNM,0021,01");
        crc=0;
        for(i=1;i<strlen(GPSCommand);i++){
            crc=crc ^ GPSCommand[i];
        }
        sprintf(&TestBuff[0],"%s*%02X\r\n",&GPSCommand[0],crc);
        AIB_DebugPrint(DEBUG_GPS,"G-Turned on GPS\r\n");
        qprintf(GPSQUEUE,"%s",&TestBuff[0]);   
    }
}
void GPSProcess(void){
    if(CheckInQue(GPSQUEUE)){   //we have uint8_tacter from GPS
        GPSInputHandler(ReadInQue(GPSQUEUE));
    }
}


void GPSInputHandler(uint8_t sData){


static unsigned int  i=0;
int                  j;
int                  cmdValue = 255;
int                  isCommand;
uint8_t                 *string;
uint8_t                 term[]={","};


   if (sData == 0x0D || sData == 0x0A){

      CurrentCommand[i] = 0x00;
      i = 0;

      strcpy(WholeCommand, CurrentCommand);

      string = strtok(&CurrentCommand[0], term);
      if(string !=NULL){
          if(strlen(string) > 1){

             for(j=0; j<strlen(string); j++){
                string[j] = toupper(string[j]);
             }
             string[j] = 0x00;
             AIB_DebugPrint(DEBUG_GPS,"G-%s\r\n",&WholeCommand[0]);
             for(j=0; j<NUM_GPS_COMMANDS; j++){
                strcpy(TestCommand, GPSCommands[j]);
                isCommand = strcmp(string, TestCommand);
                if(isCommand == 0){
                   cmdValue = j;
                   j = 255;
                   break;                                    //Break the for loop
                }
             }

             ProcessGPSCommand(cmdValue, &WholeCommand[0]);
             memset(WholeCommand, 0, MAX_GPS_CMD_LENGTH);    //Clear the WholeCommand variable
             memset(CurrentCommand, 0, MAX_GPS_CMD_LENGTH);

          }          
      }


   }else if(sData == 27){                              //Escape Key

      i=0;

   }else if(sData == 8){                              //Backspace Key

      if(i > 0) i--; else i = 0;

   }else if(sData <= 127 && i < MAX_GPS_CMD_LENGTH-1){

      CurrentCommand[i++] = sData;

   }

   return;


}
uint16_t TwoByteAsciiToBinary(char *pByte){
    char iBuf[4];
    uint16_t ret;
    iBuf[0]=pByte[0];
    iBuf[1]=pByte[1];
    iBuf[2]=0;
    ret =atoi(iBuf);
    return ret;
}
uint16_t FourByteAsciiToBinary(char *pByte){
    char iBuf[5];
    uint16_t ret;
    iBuf[0]=pByte[0];
    iBuf[1]=pByte[1];
    iBuf[2]=pByte[2];
    iBuf[3]=pByte[3];
    iBuf[4]=0;
    ret=atoi(iBuf);
    return ret;
}
void ProcessGPSCommand(int cmdNumber, uint8_t *theData){
    BOOL ShouldUpdateDate=FALSE;
    int8_t      iTemp = 0;
    uint32_t    iTemp32 = 0;
    int32_t     tempTime = 0;
    uint8_t     term[]={","};
    uint8_t     *sTemp;
    time_t      TimeStampTemp;
    int tHour,tHour2;
    struct timeval tv;
    struct tm TempTM;

   switch(cmdNumber){

      case GPS_GPGGA:  //Standard GPS Fix Data
         iTemp=SplitString(&theData[0],&term[0]);
         TempGPSInfo=CurrentGPSInfo;
         strcpy(&TempGPSInfo.FixTime[0],&parms[1].pData[0]);
         TempGPSInfo.Latitude=atof(&parms[2].pData[0]);
         TempGPSInfo.LatitudeD=NMEAToDecimalDegrees(TempGPSInfo.Latitude);
         TempGPSInfo.North_South=parms[3].pData[0];
         TempGPSInfo.Longitude=atof(&parms[4].pData[0]);
         TempGPSInfo.LongitudeD=NMEAToDecimalDegrees(TempGPSInfo.Longitude);
         TempGPSInfo.West_East=parms[5].pData[0];
         TempGPSInfo.GPSLock=atoi(&parms[6].pData[0]);
         TempGPSInfo.GPSNumSats=atoi(&parms[7].pData[0]);
         TempGPSInfo.Altitude=atof(&parms[9].pData[0]);
         switch(TempGPSInfo.GPSLock){
             case 1:
             case 2:
                 CurrentGPSInfo=TempGPSInfo;
                 TimeStampTemp = time(NULL); //read the current time stamp
                //convert to tm struct
                TempTM = *localtime(&TimeStampTemp);
                //dump in our new values
                TempTM.tm_sec = TwoByteAsciiToBinary(&CurrentGPSInfo.FixTime[4]);
                TempTM.tm_min = TwoByteAsciiToBinary(&CurrentGPSInfo.FixTime[2]);
                TempTM.tm_hour =TwoByteAsciiToBinary(&CurrentGPSInfo.FixTime[0]);

                //set the new timestamp
                tv.tv_sec = mktime(&TempTM);

                settimeofday(&tv,NULL);    
                if(!CurrentGPSInfo.GPSIsLocked )AIB_DebugPrint(DEBUG_RESULTS,"GPS Is Locked\r\n");
                CurrentGPSInfo.GPSIsLocked=TRUE;
                break;
             case 7:    //Last good fix so use it
                CurrentGPSInfo.Latitude= TempGPSInfo.Latitude;
                CurrentGPSInfo.LatitudeD=   TempGPSInfo.LatitudeD;
                CurrentGPSInfo.North_South=  TempGPSInfo.North_South;
                CurrentGPSInfo.Longitude=  TempGPSInfo.Longitude;
                CurrentGPSInfo.LongitudeD=TempGPSInfo.LongitudeD;
                CurrentGPSInfo.West_East=TempGPSInfo.West_East;
                if(CurrentGPSInfo.GPSIsLocked ){
                    CurrentGPSInfo.GPSIsLocked=FALSE;
                    AIB_DebugPrint(DEBUG_RESULTS,"GPS Lost\r\n");
                }
             default:
                if(CurrentGPSInfo.GPSIsLocked ){
                    CurrentGPSInfo.GPSIsLocked=FALSE;
                    AIB_DebugPrint(DEBUG_RESULTS,"GPS Lost\r\n");
                }
                break;
                 
         }
         break;

      case GPS_GPGSA:
         break;

      case GPS_GPGSV:
         break;

      case GPS_GPGLL:
         break;

     case GPS_PSRF103:
         break;

      case GPS_GPRMC:
         break;

      case GPS_GPVTG:
         break;

      case GPS_GPZDA:
         // if(CurrentGPSInfo.GPSLock){
             iTemp=SplitString(&theData[0],&term[0]);
             TempGPSInfo=CurrentGPSInfo;
             TempGPSInfo.FixDay=TwoByteAsciiToBinary(&parms[2].pData[0]);
             TempGPSInfo.FixMonth=TwoByteAsciiToBinary(&parms[3].pData[0]);
             TempGPSInfo.FixYear=FourByteAsciiToBinary(&parms[4].pData[0]);
             strcpy(&TempGPSInfo.FixTime[0],&parms[1].pData[0]);
             if((TempGPSInfo.FixYear !=0) && (TempGPSInfo.FixMonth !=0)){
                 tHour=TwoByteAsciiToBinary(&TempGPSInfo.FixTime[0]);
                 tHour2=TwoByteAsciiToBinary(&CurrentGPSInfo.FixTime[0]);
                 if(tHour != tHour2)ShouldUpdateDate=TRUE;  //Update on the hour
                 if(TempGPSInfo.FixYear != CurrentGPSInfo.FixYear)ShouldUpdateDate=TRUE;
                 if(TempGPSInfo.FixMonth != CurrentGPSInfo.FixMonth)ShouldUpdateDate=TRUE;
                 if(TempGPSInfo.FixDay != CurrentGPSInfo.FixDay)ShouldUpdateDate=TRUE;
                 if(ShouldUpdateDate){
                    CurrentGPSInfo=TempGPSInfo;
                    TimeStampTemp = time(NULL); //read the current time stamp
                    //convert to tm struct
                    TempTM = *localtime(&TimeStampTemp);
                    //dump in our new values
                    TempTM.tm_sec = TwoByteAsciiToBinary(&CurrentGPSInfo.FixTime[4]);
                    TempTM.tm_min = TwoByteAsciiToBinary(&CurrentGPSInfo.FixTime[2]);
                    TempTM.tm_hour =TwoByteAsciiToBinary(&CurrentGPSInfo.FixTime[0]);
                    TempTM.tm_year=CurrentGPSInfo.FixYear-1900;
                    TempTM.tm_mon=CurrentGPSInfo.FixMonth-1;
                    TempTM.tm_mday=CurrentGPSInfo.FixDay;

                    //set the new timestamp
                    tv.tv_sec = mktime(&TempTM);

                    settimeofday(&tv,NULL);
                 }
             }//if fixday <> 0
          //if lock}
         break;


      default:
         break;

   }

}


float GetSingleTokenFLOAT(uint8_t *theData){

uint8_t      term[] = {","};
uint8_t      *sTemp;

   sTemp = strtok(&theData[0], term);
   sTemp = strtok(0, term);

   return atof(sTemp);

}


void GetStringToken(uint8_t *theData, uint8_t *returnString){

uint8_t      term[] = {","};
uint8_t      *sTemp;

   sTemp = strtok(&theData[0], term);
   sTemp = strtok(0, term);

   if(strlen(sTemp) > 0 && sTemp != 0x0000){
      sprintf(returnString,"%s", sTemp);
   }
   return;

}



//******************************************************************************/
//   GetToken - Same as standard library strtok, except
//             we don't span accross null parameters
//               This is to allow smarter parsing of strings
//               from other devices
uint8_t *GetToken(uint8_t *str1, uint8_t *str2){
   static uint8_t *saves;
   uint8_t *beg, *end;


   beg = (str1)? str1: saves;
//   beg += strspn(beg, str2);
   if(beg!=0){
       if (*beg == '\0'){
          *saves = ' ';
          return(0);
       }       
   }
   else{
       return(0);
   }
   end = strpbrk(beg, str2);
   if(end != 0){         //saftey net
       if (*end != '\0'){
          *end = '\0';
          end++;
       }
       saves = end;
   }
   else{
       saves=end=0;
   }
   return(beg);
}