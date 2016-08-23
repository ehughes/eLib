/* 
 * File:   GPS.h
 * Author: rjc19
 *
 * Created on January 22, 2014, 5:33 PM
 */

#ifndef GPS_H
#define	GPS_H

#ifdef THIS_IS_GPS
#define EXTERNGPS
#else
#define EXTERNGPS extern
#endif

#define GPSQUEUE AIB_UART1
#define GPSON GPS_ENABLE_PIN=0;
#define GPSOFF GPS_ENABLE_PIN=1;

#define        GPS_GPGGA                     0
#define        GPS_GPGSA                     1
#define        GPS_GPGLL                     2
#define        GPS_PSRF103                   3
#define        GPS_GPZDA                     4
#define        GPS_GPGSV                     5
#define        GPS_GPRMC                     6
#define        GPS_GPVTG                     7

#define        NUM_GPS_COMMANDS              8
#define        MAX_GPS_CMD_TXT_LEN           10
#define        MAX_GPS_CMD_LENGTH            100

#ifdef THIS_IS_GPS
char           GPSCommands[NUM_GPS_COMMANDS][MAX_GPS_CMD_TXT_LEN] = {"$GPGGA",
                                                                     "$GPGSA",
                                                                     "$GPGLL",
                                                                     "$PSRF103",
                                                                     "$GPZDA",
                                                                     "$GPGSV",
                                                                     "$GPRMC",
                                                                     "$GPVTG"};
#else
extern char           GPSCommands[NUM_GPS_COMMANDS][MAX_GPS_CMD_TXT_LEN];
#endif

typedef struct{
    uint16_t FixDay;
    uint16_t FixMonth;
    uint16_t FixYear;
    char    FixTime[10];
    float   Latitude;
    float   LatitudeD;
    int8_t  North_South;
    float   Longitude;
    float   LongitudeD;
    int8_t  West_East;
    int8_t  GPSLock;
    int8_t  GPSNumSats;
    float   HDOP;
    float   Altitude;
    int8_t  M1;
    float   Height_WGS84;
    int8_t  M2;
    int8_t  GPSIsLocked;
}GPS_GGA_STRUCT;
EXTERNGPS GPS_GGA_STRUCT CurrentGPSInfo;
EXTERNGPS GPS_GGA_STRUCT TempGPSInfo;
EXTERNGPS char           CurrentCommand[MAX_GPS_CMD_LENGTH];
EXTERNGPS char           WholeCommand[MAX_GPS_CMD_LENGTH];
EXTERNGPS char           TestCommand[MAX_GPS_CMD_TXT_LEN];
EXTERNGPS int8_t       GPSStatusByte;

EXTERNGPS int8_t IsGPSOn;
EXTERNGPS int8_t GPSSendNow;
EXTERNGPS int8_t           GPSLock;
EXTERNGPS int8_t           GPSGot1PPS;
EXTERNGPS int8_t           GPSGotTimeMessage;
EXTERNGPS int8_t           GPSNumSats;
EXTERNGPS int8_t          GPSPPSTimeoutCounter;
EXTERNGPS    int32_t NextGPSTime;
EXTERNGPS int32_t GPSParsePos;
EXTERNGPS int32_t GPSTimeOutCounter;

void GPSInit(void);
void GPSProcess(void);

void GPSInputHandler(uint8_t sData);
void ProcessGPSCommand(int cmdNumber, uint8_t *theData);
float GetSingleTokenFLOAT(uint8_t *theData);
void GetStringToken(uint8_t *theData, uint8_t *returnString);
void PowerOnGPS(void);
void PowerOffGPS(void);
//******************************************************************************/
//   GetToken - Same as standard library strtok, except
//             we don't span accross null parameters
//               This is to allow smarter parsing of strings
//               from other devices
uint8_t *GetToken(uint8_t *str1, uint8_t *str2);





#endif	/* GPS_H */

