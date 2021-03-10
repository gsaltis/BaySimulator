/*****************************************************************************
 * FILE NAME    : SIMThread.c
 * DATE         : August 01 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertic Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "SIMThread.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Function : SIMThread
 *****************************************************************************/
void*
SIMThread
(
 void*                                  InParameters
)
{
  (void)InParameters;
  while (1)
  {
    sleep(1);
  }
}