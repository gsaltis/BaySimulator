/*****************************************************************************
 * FILE NAME    : SystemTime.h
 * DATE         : February 21 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _systemtime_h_
#define _systemtime_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type
 *****************************************************************************/
struct _SystemTime
{
    uint8_t			hour;
    uint8_t			minute;
    uint8_t			second;
    uint8_t			millisecond;
    uint8_t			day;
    uint8_t			month;
    uint16_t			year;
};
typedef struct _SystemTime SystemTime;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
SystemGetTime
(SystemTime* InTime);

void
SystemConvertTime
(SystemTime* InTime, uint64_t InMSecTime);

uint64_t
SystemGetTimeInMilliseconds
();

char*
SystemTimeGetString
(char* InString);

#endif /* _systemtime_h_*/
