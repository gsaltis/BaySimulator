/******************************************************************************
 * FILE NAME    : ShuntType.h
 * DATE         : March 14 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 ******************************************************************************/
#ifndef _shunttype_h_
#define _shunttype_h_

/******************************************************************************!
 * Global Headers
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <StringUtils.h>

/******************************************************************************!
 * Local Headers
 ******************************************************************************/

/******************************************************************************!
 * Exported Macros
 ******************************************************************************/

/******************************************************************************!
 * Exported Type : ShuntType
 ******************************************************************************/
enum _ShuntType
{
  SHUNT_TYPE_NONE                       = 0,
  SHUNT_TYPE_FUSE,
  SHUNT_TYPE_BREAKER,
  SHUNT_TYPE_BULLET
};
typedef enum _ShuntType ShuntType;

/******************************************************************************!
 * Exported Status : ShuntStatus
 ******************************************************************************/
enum _ShuntStatus
{
  SHUNT_STATUS_NONE                       = 0,
  SHUNT_STATUS_OVER_CURRENT,
  SHUNT_STATUS_UNKNOWN,
  SHUNT_STATUS_REMOVED
};
typedef enum _ShuntStatus ShuntStatus;

/******************************************************************************!
 * Exported Type : Shunt
 ******************************************************************************/
enum _ShuntPowerStatus {
  SHUNT_POWER_OPENED                    = 0,
  SHUNT_POWER_CLOSED
};
typedef enum _ShuntPowerStatus ShuntPowerStatus;

/******************************************************************************!
 * Exported Type : Shunt
 ******************************************************************************/
struct _Shunt
{
  uint8_t                               contactWidth;
  ShuntType                             type;
  uint16_t                              maxCurrent;
  uint16_t                              current;
  bool                                  occupied;
  bool                                  alarm;
  ShuntStatus                           status;
  ShuntPowerStatus                      power;
  bool                                  removed;
};
typedef struct _Shunt Shunt;

/******************************************************************************!
 * Exported Data
 ******************************************************************************/

/******************************************************************************!
 * Exported Functions
 ******************************************************************************/
void
SetShuntCurrent
(Shunt* InShunt, uint32_t InCurrent);

uint8_t
GetShuntIDBits
(Shunt* InShunt);

void
ShuntClear
(Shunt* InShunt);

string
ShuntTypeToString
(ShuntType InType);

#endif /* _shunttype_h_ */
