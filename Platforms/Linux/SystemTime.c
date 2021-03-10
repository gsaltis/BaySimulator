/*****************************************************************************
 * FILE NAME    : SystemTime.c
 * DATE         : February 21 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#define _POSIX_C_SOURCE 200809L

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "SystemTime.h"

/*****************************************************************************!
 * Function : SystemGetTimeInMilliseconds
 *****************************************************************************/
uint64_t
SystemGetTimeInMilliseconds()
{
  struct timespec		spec;
  uint64_t			s, ms;

  clock_gettime(CLOCK_REALTIME, &spec);

  s = spec.tv_sec;
  s *= 1000;
  ms = round(spec.tv_nsec / 1.0E6);
  while ( ms > 999 ) {
    s += 1000;
    ms -= 1000;
  }
  return s + ms;
}

/*****************************************************************************!
 * Function : SystemGetTime
 *****************************************************************************/
void
SystemGetTime
(
 SystemTime*			InTime
)
{
  uint64_t			t = SystemGetTimeInMilliseconds();
  SystemConvertTime(InTime, t);
} 

/*****************************************************************************!
 * Function : SystemConvertTime
 *****************************************************************************/
void
SystemConvertTime
(
 SystemTime*			InTime,
 uint64_t               InMSecTime
)
{
  time_t			    t2 = InMSecTime / 1000;
  struct tm 			tm = *localtime(&t2);

  InTime->hour        = tm.tm_hour;
  InTime->minute      = tm.tm_min;
  InTime->second      = tm.tm_sec;
  InTime->millisecond = InMSecTime % 1000;
  InTime->month       = tm.tm_mon + 1;
  InTime->day         = tm.tm_mday;
  InTime->year        = tm.tm_year + 1900;
} 

/*****************************************************************************!
 * Function : SystemTimeGetString
 *****************************************************************************/
char*
SystemTimeGetString
(char* InString)
{
  SystemTime                    ti;

  SystemGetTime(&ti);

  sprintf(InString, "%02d/%02d/%04d %02d:%02d:%02d.%03d", 
          ti.month, ti.day, ti.year, ti.hour, ti.minute, ti.second, ti.millisecond);
  return InString;
} 
