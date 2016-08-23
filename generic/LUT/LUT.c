#include "LUT.h"
#include "stdlib.h"
#include "FSIO.h"

float Interplotate_LUT_F(LUT_F *LUT, float X)
{
    uint32_t i;
    float Slope,dX,dY,Result;

    //See if the table is of useful length

    if( LUT->TableLength == 0)
        return 0.0;

    if( LUT->TableLength == 1)
        return LUT->Y[0];

    /* First Check the boundaries*/

    if(X <= LUT->X[0])  // if the input is below our lowest table value,  use the 1st 2 points
        i = 0;
    else if(X >= LUT->X[LUT->TableLength-1])  //if the input is higher than the maximum X value in the the table, then we will use the last 2 points to estimate
        i = LUT->TableLength-2;
    else //Scan through the table to see what pair we need
        {
            for(i=0; i<LUT->TableLength - 1; i++)
                {
                    if(X >= LUT->X[i] && X <= LUT->X[i+1])
                        {
                            break;
                        }
                }
        }

    //The variable i contains the starting index of the pair
    //Calculate the slope between the 2 points
    Slope = (LUT->Y[i + 1] - LUT->Y[i]) / (LUT->X[i + 1] - LUT->X[i]);
    dX = X - LUT->X[i];
    dY = dX * Slope;
    Result = LUT->Y[i] + dY;
    return Result;
}


uint8_t IsStringAllWhiteSpace(char *String)
{
    uint32_t i = 0;

    while(String[i]!=0 && i<LUT_MAX_LINE_LENGTH )
        {
            if(isspace(String[i++]) == 0)
                {
                    return 0;
                }
        }

    return 1;
}

uint32_t LoadLUT_F_FromCSV(   char *FileName,         //FileName To open
                              char *HeaderLine1,      //Header String read from file. Send a NULL if you don't want it
                              char *HeaderLine2,      //SecondHeader String to read from file. Send a NULL if you don't want it
                              char *ColumnX_Name,     //Name of the Column X Data. Send a NULL if you don't want it
                              char *ColumnY_Name,     //Name of the Column y Data. Send a NULL if you don't want it
                              LUT_F *MyLUT_F,          //LUT Table to Dump Data Intp
                              uint32_t MaxLines           //Maximum Number of LUT Entries to read in

                          )
{
    FSFILE *MyFile;
    char TmpBuf[ LUT_MAX_LINE_LENGTH];
    char *TmpStr;
    MyFile = FSfopen(FileName,"r");

    if(MyFile == NULL)
        {
            return  LUT_FILE_OPEN_ERROR;
        }

    if(HeaderLine1 == NULL)
        {
            HeaderLine1 = &TmpBuf[0];
        }

    //Read in the 1st header line
    if(FSfgets(HeaderLine1,LUT_MAX_LINE_LENGTH,MyFile) == NULL)
        {
            return LUT_FILE_HEADER1_READ_ERROR;
        }

    if(HeaderLine2 == NULL)
        {
            HeaderLine2 = &TmpBuf[0];
        }

    //Read in the 2nd header line
    if(FSfgets(HeaderLine2,LUT_MAX_LINE_LENGTH,MyFile) == 0)
        {
            return LUT_FILE_HEADER2_READ_ERROR;
        }

    //Read in the Column Headers
    if(FSfgets(TmpBuf,LUT_MAX_LINE_LENGTH,MyFile) == 0)
        {
            return LUT_FILE_COLUMN_NAME_READ_ERROR;
        }

    //Parse the 1st part
    TmpStr = strtok(&TmpBuf[0],",");

    if(TmpStr == NULL)
        {
            return LUT_FILE_COLUMN_NAME_READ_ERROR;
        }

    if(ColumnX_Name != NULL)
        {
            strcpy(ColumnX_Name,TmpStr);
        }

    //Parse the 2st part
    TmpStr = strtok(NULL,",");

    if(TmpStr == NULL)
        {
            return LUT_FILE_COLUMN_NAME_READ_ERROR;
        }

    if(ColumnY_Name != NULL)
        {
            strcpy(ColumnY_Name,TmpStr);
        }

    MyLUT_F->TableLength = 0;

    do
        {
            //Read in the Data Pint
            if(FSfgets(TmpBuf,LUT_MAX_LINE_LENGTH,MyFile) == NULL)
                {
                    return  LUT_FILE_DATA_READ_ERROR;
                }

            //see the line is whitespace, if so then ignore.
            if(IsStringAllWhiteSpace(TmpBuf))
                {
                    continue;
                }

            //Parse the 1st part
            TmpStr = strtok(TmpBuf,",");

            if(TmpStr == NULL)
                {
                    return  LUT_FILE_DATA_READ_ERROR;
                }

            MyLUT_F->X[MyLUT_F->TableLength] = atof(TmpStr);
            //Read the 2ns part
            TmpStr = strtok(NULL,",");

            if(TmpStr == NULL)
                {
                    return  LUT_FILE_DATA_READ_ERROR;
                }

            MyLUT_F->Y[MyLUT_F->TableLength] = atof(TmpStr);
            MyLUT_F->TableLength++;
        }
    while(FSfeof(MyFile) == 0 && (MyLUT_F->TableLength<MaxLines));

    FSfclose(MyFile);
    return 0;
}

