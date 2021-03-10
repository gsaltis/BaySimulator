/*****************************************************************************
 * FILE NAME    : DeviceInterface.c
 * DATE         : May 24 2019
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2019 by Gregory R Saltis
 *****************************************************************************/

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
#include "BayType.h"
#include "DSMType.h"
#include "ShuntType.h"
#include "main.h"
#include "ESNACANNodeType.h"
#include "NodeDataStore.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
DSMType*
DeviceFindDSM
(string InDSMUnitNumberString);

RectifierType*
DeviceFindRectifier
(string InUnitNumberString);

Bay*
DeviceFindBay
(string InBayIndexString);

ShuntType
DeviceShuntTypeFromString
(string InDeviceTypeString);

bool
DeviceCheckIndex
(uint8_t InSlotIndex, uint8_t* InIndex);

/*****************************************************************************!
 * Function : DeviceDSMAdd
 *****************************************************************************/
bool
DeviceDSMAdd
(string InDSMUnitNumberString, string InSerialNumber,
 string InProductNumber, string InMaxPointsPerBus) 
{
  uint16_t                              description;
  ESNANodeStatus                        status;
  Bay*                                  bay;
  DSMType*                              dsm;
  int                                   n;
  uint8_t                               unitNumber;

  unitNumber = atoi(InDSMUnitNumberString);
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return false;
  }
  description = atoi(InMaxPointsPerBus);
  status = ESNANodeDSMAdd(unitNumber, description | 0x800);
  if ( status != ESNA_NODE_STATUS_OK ) {
    return false;
  }
  dsm = BayFindDSM(bay, unitNumber);
  if ( NULL == dsm ) {
    return false;
  }
  dsm->parentNode->serialNumber = (uint32_t)atoi(InSerialNumber);
  n = strlen(InProductNumber);
  if ( n > NODE_PART_NUMBER_SIZE ) {
    n = NODE_PART_NUMBER_SIZE;
  }
  strncpy((char*)(dsm->parentNode->partNumber), InProductNumber, n);
  dsm->parentNode->partNumber[n] = 0x00;
  return true;
}

/*****************************************************************************!
 * Function : DeviceDSMRemove
 *****************************************************************************/
bool
DeviceDSMRemove
(string InUnitNumber)
{
  Bay*                                  bay;
  DSMType*                              dsm;

  bay = DeviceFindBay("1");
  if ( NULL == bay ) {
    return false;
  }
  dsm = DeviceFindDSM(InUnitNumber);
  if ( NULL == dsm ) {
    return false;
  }
  BayRemoveDSM(bay, dsm);
  NodeDataStoreNodeClear(dsm->parentNode->nodeNumber);
  return true;
}

/*****************************************************************************!
 * Function : DeviceRectifierRemove
 *****************************************************************************/
bool
DeviceRectifierRemove
(string InUnitNumber)
{
  Bay*                                  bay;
  RectifierType*                        rectifier;

  bay = DeviceFindBay("1");
  if ( NULL == bay ) {
    return false;
  }
  rectifier = DeviceFindRectifier(InUnitNumber);
  if ( NULL == rectifier ) {
    return false;
  }
  BayRemoveRectifier(bay, rectifier);
  NodeClear(rectifier->parentNode);
  return true;
}

/*****************************************************************************!
 * Function : DeviceDSMExists
 *****************************************************************************/
bool
DeviceDSMExists
(string InUnitNumber)
{
  return DeviceFindDSM(InUnitNumber);
}

/*****************************************************************************!
 * Funtion : DeviceDistAdd
 *****************************************************************************/
bool
DeviceDistAdd
(string InDSMUnitNumberString,
 string InDistSlotNumberString,
 string InDistType,
 string InDistWidth,
 string InDistMaxCurrent)
{
  DSMType*                              dsm;
  uint8_t                               slotIndex, width;
  uint16_t                              current;
  ShuntType                             type;
  
  dsm = DeviceFindDSM(InDSMUnitNumberString);
  if ( NULL == dsm ) {
    printf("\r\nCould not find DSM\r\n");
    return false;
  }

  type = DeviceShuntTypeFromString(InDistType);
  if ( SHUNT_TYPE_NONE == type ) {
    printf("\r\nInvalid Type\r\n");
    return false;
  }

  slotIndex = atoi(InDistSlotNumberString);
  width = atoi(InDistWidth);
  current = atoi(InDistMaxCurrent);
  if ( !DSMShuntFits(dsm, slotIndex, width) ) {
    printf("\r\nShunt won't fit : Index : %d,  Width : %d\r\n", slotIndex, width);
    return false;
  }

  DSMInsertShunt(dsm, slotIndex, width, current, type);
  return true;
}

/*****************************************************************************!
 * Function : DeviceDistRemove
 *****************************************************************************/
bool
DeviceDistRemove
(string InDSMUnitNumberString,
 string InDistSlotNumberString)
{
  DSMType*                              dsm;
  uint8_t                               slotIndex, index;

  dsm = DeviceFindDSM(InDSMUnitNumberString);
  if ( NULL == dsm ) {
    return false;
  }

  slotIndex = atoi(InDistSlotNumberString);
  if ( !DeviceCheckIndex(slotIndex, &index) ) {
    return false;
  }
  DSMShuntRemove(dsm, index);
  return true;
}

/*****************************************************************************!
 * Function : DeviceDistSetOpen
 *****************************************************************************/
bool
DeviceDistSetOpen
(string InDSMUnitNumberString,
 string InDistSlotNumberString)
{
  DSMType*                              dsm;
  uint8_t                               slotIndex, index;

  dsm = DeviceFindDSM(InDSMUnitNumberString);
  if ( NULL == dsm ) {
    return false;
  }
  slotIndex = atoi(InDistSlotNumberString);
  if ( !DeviceCheckIndex(slotIndex, &index) ) {
    return false;
  }

  if ( !DSMShuntOccupied(dsm, index) ) {
    return false;
  }

  dsm->shunts[index-1].power = SHUNT_POWER_OPENED;
  return true;
}
 
/*****************************************************************************!
 * Function : DeviceDistSetClose
 *****************************************************************************/
bool
DeviceDistSetClose
(string InDSMUnitNumberString,
 string InDistSlotNumberString)
{
  DSMType*                              dsm;
  uint8_t                               slotIndex, index;

  dsm = DeviceFindDSM(InDSMUnitNumberString);
  if ( NULL == dsm ) {
    return false;
  }
  slotIndex = atoi(InDistSlotNumberString);
  if ( !DeviceCheckIndex(slotIndex, &index) ) {
    return false;
  }
  if ( !DSMShuntOccupied(dsm, index) ) {
    return false;
  }
  dsm->shunts[index-1].power = SHUNT_POWER_CLOSED;
  return true;
}
 
/*****************************************************************************!
 * Function : DeviceDistSetCurrent
 *****************************************************************************/
bool
DeviceDistSetCurrent
(string InDSMUnitNumberString,
 string InDistSlotNumberString,
 string InDistCurrent)
{
  DSMType*                              dsm;
  uint8_t                               slotIndex, index;
  uint16_t                              current;
  
  dsm = DeviceFindDSM(InDSMUnitNumberString);
  if ( NULL == dsm ) {
    return false;
  }

  slotIndex = atoi(InDistSlotNumberString);
  if ( !DeviceCheckIndex(slotIndex, &index) ) {
    return false;
  }

  if ( !DSMShuntOccupied(dsm, index) ) {
    return false;
  }

  current = atoi(InDistCurrent);
  dsm->shunts[index-1].current = current;
  return true;
}

/*****************************************************************************!
 * Function : DeviceFindDSM 
 *****************************************************************************/
DSMType*
DeviceFindDSM
(string InDSMUnitNumberString)
{
  uint8_t                               unitNumber;
  Bay*                                  bay;
  DSMType*                              dsm;
  
  if ( NULL == InDSMUnitNumberString ) {
    return NULL;
  }

  unitNumber = atoi(InDSMUnitNumberString);

  bay = BaySetFindBayByIndex(mainBays,1);
  if ( NULL == bay ) {
    return NULL;
  }
  dsm = BayFindDSM(bay, unitNumber);
  if ( NULL == dsm ) {
    return NULL;
  }
  return dsm;
}

/*****************************************************************************!
 * Function : DeviceFindRectifier
 *****************************************************************************/
RectifierType*
DeviceFindRectifier
(string InUnitNumberString)
{
  uint8_t                               unitNumber;
  Bay*                                  bay;
  RectifierType*                        rectifier;
  
  if ( NULL == InUnitNumberString ) {
    return NULL;
  }

  unitNumber = atoi(InUnitNumberString);

  bay = BaySetFindBayByIndex(mainBays,1);
  if ( NULL == bay ) {
    return NULL;
  }
  rectifier = BayFindRectifier(bay, unitNumber);
  if ( NULL == rectifier ) {
    return NULL;
  }
  return rectifier;
}

/*****************************************************************************!
 * Function : DeviceFindBay
 *****************************************************************************/
Bay*
DeviceFindBay
(string InBayIndexString) 
{
  uint8_t                               bayIndex;
  Bay*                                  bay;
  bayIndex = atoi(InBayIndexString);
  bay = BaySetFindBayByIndex(mainBays, bayIndex);
  return bay;
}

/*****************************************************************************!
 * Function : DeviceShuntTypeFromString
 *****************************************************************************/
ShuntType
DeviceShuntTypeFromString
(string InDeviceTypeString)
{
  if ( StringEqualNoCase(InDeviceTypeString, "fuse") ) {
    return SHUNT_TYPE_FUSE;
  }
  if ( StringEqualNoCase(InDeviceTypeString, "breaker") ) {
    return SHUNT_TYPE_BREAKER;
  }
  if ( StringEqualNoCase(InDeviceTypeString, "bullet") ) {
    return SHUNT_TYPE_BULLET;
  }
  return SHUNT_TYPE_NONE;
}

/*****************************************************************************!
 * Function : DeviceCheckIndex
 *****************************************************************************/
bool
DeviceCheckIndex
(
 uint8_t                                InSlotIndex,
 uint8_t*                               InIndex
)
{
  if ( InSlotIndex < 1 || InSlotIndex > 12 ) {
    return false;
  }

  *InIndex = InSlotIndex;
  return true;
}
