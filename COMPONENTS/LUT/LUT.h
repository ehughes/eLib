
#ifndef LUT_H
#define LUT_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>


typedef struct
{

    uint32_t TableLength;
    float *X;
    float *Y;

} LUT_F;


float Interplotate_LUT_F(LUT_F *LUT, float X);

/*
#define     LUT_MAX_LINE_LENGTH               64
#define     LUT_FILE_NO_ERROR                (0)
#define     LUT_FILE_OPEN_ERROR              (1<<0)
#define     LUT_FILE_HEADER1_READ_ERROR      (1<<1)
#define     LUT_FILE_HEADER2_READ_ERROR      (1<<2)
#define     LUT_FILE_COLUMN_NAME_READ_ERROR  (1<<3)
#define     LUT_FILE_DATA_READ_ERROR         (1<<4)

uint32_t LoadLUT_F_FromCSV(   char *FileName,         //FileName To open
                              char *HeaderLine1,      //Header String read from file. Send a NULL if you don't want it
                              char *HeaderLine2,      //SecondHeader String to read from file. Send a NULL if you don't want it
                              char *ColumnX_Name,     //Name of the Column X Data. Send a NULL if you don't want it
                              char *ColumnY_Name,     //Name of the Column y Data. Send a NULL if you don't want it
                              LUT_F *MyLUT_F,          //LUT Table to Dump Data Intp
                              uint32_t MaxLines           //Maximum Number of LUT Entries to read in

                          );
*/
#ifdef  __cplusplus
}
#endif

#endif  /* LUT_H */

