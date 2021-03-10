/*****************************************************************************
 * FILE NAME    : DeviceInterface.h
 * DATE         : May 24 2019
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2019 by Gregory R Saltis
 *****************************************************************************/
#ifndef _deviceinterface_h_
#define _deviceinterface_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <StringUtils.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "DSMType.h"
#include "ShuntType.h"
#include "BayType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
bool
DeviceDSMAdd
(string InDSMUnitNumberString, string InSerialNumber,
 string InProductNumber, string InMaxPointsPerBus);

bool
DeviceDSMRemove
(string InUnitNumber);

bool
DeviceRectifierRemove
(string InUnitNumber);

bool
DeviceDSMExists
(string InUnitNumber);

bool
DeviceDistAdd
(string InDSMUnitNumberString,
 string InDistSlotNumberString,
 string InDistType,
 string InDistWidth,
 string InDistMaxCurrent);

bool
DeviceDistRemove
(string InDSMUnitNumberString,
 string InDistSlotNumberString);

bool
DeviceDistSetOpen
(string InDSMUnitNumberString,
 string InDistSlotNumberString);
 
bool
DeviceDistSetClose
(string InDSMUnitNumberString,
 string InDistSlotNumberString);
 
bool
DeviceDistSetCurrent
(string InDSMUnitNumberString,
 string InDistSlotNumberString,
 string InDistCurrent);
 
#endif /* _deviceinterface_h_*/
