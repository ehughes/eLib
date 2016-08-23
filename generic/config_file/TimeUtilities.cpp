#include "System.h"


uint8_t TimeDateString_8601[MAX_TIME_DATE_STRING_LENGTH];

uint32_t MakeISO_8601_Time(tm * T,uint8_t *Buffer)
{

	return sprintf(Buffer,"%02i:%02i:%02iZ",T->tm_hour,T->tm_min,T->tm_sec);

}

uint32_t MakeISO_8601_Date(tm * T,uint8_t *Buffer)
{
    	return sprintf(Buffer,"%4i-%02i-%02i",T->tm_year + 1900,T->tm_mon + 1,T->tm_mday);
}

uint32_t MakeISO_8601_TimeDate(tm * T,uint8_t *Buffer)
{
	uint32_t i;

    	i = MakeISO_8601_Date(T,Buffer);
    	Buffer[i] = 'T';
    	i++;
    	i += MakeISO_8601_Time(T,&Buffer[i]);
    	return i;
}




