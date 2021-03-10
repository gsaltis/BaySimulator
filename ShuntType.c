/*****************************************************************************
 * FILE NAME    : ShuntType.c
 * DATE         : March 14 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <StringUtils.h>
#include <MemoryManager.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ShuntType.h"

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
 * Function : SetShuntCurrent
 * Purpose  : Set the current of a shunt
 *****************************************************************************/
void
SetShuntCurrent
(
 Shunt*                                 InShunt,
 uint32_t                               InCurrent
)
{
  if ( NULL == InShunt ) {
    return;
  }

  InShunt->current = InCurrent;
}

/******************************************************************************!
 * Function : GetShuntIDBits
 ******************************************************************************/
uint8_t
GetShuntIDBits
(
 Shunt*                                 InShunt
)
{
  uint8_t                               high, low, mid;
  
  if ( NULL == InShunt ) {
    return 0;
  }

  high = mid = low = 0;
  switch (InShunt->type) {
    case SHUNT_TYPE_FUSE : {
      low = 1;
      break;
    }
    case SHUNT_TYPE_BREAKER : {
      high = mid = 1;
      break;
    }
    case SHUNT_TYPE_BULLET : {
      high = low = 1;
      break;
    }
    case SHUNT_TYPE_NONE : {
      high = mid = low = 1;
      break;
    }
  }
  return (high << 2) + (mid << 1) + low; 
}

/*****************************************************************************!
 * Function : ShuntClear
 *****************************************************************************/
void
ShuntClear
(Shunt* InShunt)
{
  if ( InShunt == NULL ) {
    return;
  }
  InShunt->contactWidth = 0;
  InShunt->type = SHUNT_TYPE_NONE;
  InShunt->maxCurrent = 0;
  InShunt->current = 0;
  InShunt->occupied = false;
  InShunt->alarm = false;
  InShunt->status = SHUNT_STATUS_NONE;
  InShunt->power = SHUNT_POWER_OPENED;
  InShunt->removed = false;
}

/*****************************************************************************!
 * Function : ShuntTypeToString
 *****************************************************************************/
string
ShuntTypeToString
(ShuntType InType)
{
  switch (InType) {
    case SHUNT_TYPE_NONE :
      return "None";
      
    case SHUNT_TYPE_FUSE : {
      return "fuse";
    }

    case SHUNT_TYPE_BREAKER : {
      return "breaker";
    }
      
    case SHUNT_TYPE_BULLET : {
      return "bullet";
    }
  }
  return "None";
}
