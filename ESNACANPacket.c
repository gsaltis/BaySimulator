/******************************************************************************
 * FILE NAME    : ESNACANPacket.c
 * DATE         : May 09 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 ******************************************************************************/

/******************************************************************************!
 * Global Headers
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/******************************************************************************!
 * Local Headers
 ******************************************************************************/
#include "ESNACANPacket.h"

/******************************************************************************!
 * Function : ArbitrationFieldCreate
 ******************************************************************************/
ArbitrationField*
ArbitrationFieldCreate
()
{
  ArbitrationField*             returnArbitrationField;
  returnArbitrationField = (ArbitrationField*)malloc(sizeof(ArbitrationField));
  memset(returnArbitrationField, 0x00, sizeof(ArbitrationField));
  return returnArbitrationField;
}

/******************************************************************************!
 * Function : ArbitrationFieldDestroy
 ******************************************************************************/
void
ArbitrationFieldDestroy
(ArbitrationField* InArbitrationField)
{
  if ( InArbitrationField == NULL ) {
    return;
  }
  free(InArbitrationField);
}

/******************************************************************************!
 * Function : ArbitrationFieldSet
 ******************************************************************************/
void
ArbitrationFieldSet
(ArbitrationField* InArbitrationField,
 uint32_t InFields)
{
  if ( InArbitrationField == NULL ) {
    return;
  }
  InArbitrationField->Fields = InFields;
}

/******************************************************************************!
 * Function : ArbitrationFieldClear
 ******************************************************************************/
void
ArbitrationFieldClear
(ArbitrationField* InArbitrationField)
{
  if ( InArbitrationField == NULL ) {
    return;
  }
  memset(InArbitrationField, 0x00, sizeof(ArbitrationField));
}

/******************************************************************************!
 * Function : ArbitrationFieldSetFull
 ******************************************************************************/
void
ArbitrationFieldSetFull
(ArbitrationField* InArbitrationField,
 uint32_t InCommand,
 uint32_t InNodeAddress,
 uint32_t InPriority,
 uint32_t InDirection)
{
  if ( InArbitrationField == NULL ) {
    return;
  }
  InArbitrationField->Command     = InCommand;
  InArbitrationField->NodeAddress = InNodeAddress;
  InArbitrationField->Priority    = InPriority;
  InArbitrationField->Direction   = InDirection;
}

/******************************************************************************!
 * Function : ArbitrationFieldPack
 ******************************************************************************/
void
ArbitrationFieldPack
(ArbitrationField* InArbitrationField, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InArbitrationField == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InArbitrationField->Fields) & 0xFF;
  *(buffer++) = (unsigned char)(InArbitrationField->Fields >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InArbitrationField->Fields >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InArbitrationField->Fields >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : ArbitrationFieldUnpack
 ******************************************************************************/
void
ArbitrationFieldUnpack
(ArbitrationField* InArbitrationField, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InArbitrationField->Fields = (uint32_t)*(buffer++);
  InArbitrationField->Fields |= ((uint32_t)*(buffer++) << 8);
  InArbitrationField->Fields |= ((uint32_t)*(buffer++) << 16);
  InArbitrationField->Fields |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : NodeAddressArbitrationFieldCreate
 ******************************************************************************/
NodeAddressArbitrationField*
NodeAddressArbitrationFieldCreate
()
{
  NodeAddressArbitrationField*  returnNodeAddressArbitrationField;
  returnNodeAddressArbitrationField = (NodeAddressArbitrationField*)malloc(sizeof(NodeAddressArbitrationField));
  memset(returnNodeAddressArbitrationField, 0x00, sizeof(NodeAddressArbitrationField));
  return returnNodeAddressArbitrationField;
}

/******************************************************************************!
 * Function : NodeAddressArbitrationFieldDestroy
 ******************************************************************************/
void
NodeAddressArbitrationFieldDestroy
(NodeAddressArbitrationField* InNodeAddressArbitrationField)
{
  if ( InNodeAddressArbitrationField == NULL ) {
    return;
  }
  free(InNodeAddressArbitrationField);
}

/******************************************************************************!
 * Function : NodeAddressArbitrationFieldSet
 ******************************************************************************/
void
NodeAddressArbitrationFieldSet
(NodeAddressArbitrationField* InNodeAddressArbitrationField,
 uint32_t InFields)
{
  if ( InNodeAddressArbitrationField == NULL ) {
    return;
  }
  InNodeAddressArbitrationField->Fields = InFields;
}

/******************************************************************************!
 * Function : NodeAddressArbitrationFieldClear
 ******************************************************************************/
void
NodeAddressArbitrationFieldClear
(NodeAddressArbitrationField* InNodeAddressArbitrationField)
{
  if ( InNodeAddressArbitrationField == NULL ) {
    return;
  }
  memset(InNodeAddressArbitrationField, 0x00, sizeof(NodeAddressArbitrationField));
}

/******************************************************************************!
 * Function : NodeAddressArbitrationFieldSetFull
 ******************************************************************************/
void
NodeAddressArbitrationFieldSetFull
(NodeAddressArbitrationField* InNodeAddressArbitrationField,
 uint32_t InUnitNumber,
 uint32_t InShelfNumber,
 uint32_t InNodeType,
 uint32_t InNodeAddress,
 uint32_t InPriority,
 uint32_t InDirection)
{
  if ( InNodeAddressArbitrationField == NULL ) {
    return;
  }
  InNodeAddressArbitrationField->unitNumber  = InUnitNumber;
  InNodeAddressArbitrationField->shelfNumber = InShelfNumber;
  InNodeAddressArbitrationField->NodeType    = InNodeType;
  InNodeAddressArbitrationField->NodeAddress = InNodeAddress;
  InNodeAddressArbitrationField->Priority    = InPriority;
  InNodeAddressArbitrationField->Direction   = InDirection;
}

/******************************************************************************!
 * Function : NodeAddressArbitrationFieldPack
 ******************************************************************************/
void
NodeAddressArbitrationFieldPack
(NodeAddressArbitrationField* InNodeAddressArbitrationField, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InNodeAddressArbitrationField == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InNodeAddressArbitrationField->Fields) & 0xFF;
  *(buffer++) = (unsigned char)(InNodeAddressArbitrationField->Fields >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InNodeAddressArbitrationField->Fields >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InNodeAddressArbitrationField->Fields >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : NodeAddressArbitrationFieldUnpack
 ******************************************************************************/
void
NodeAddressArbitrationFieldUnpack
(NodeAddressArbitrationField* InNodeAddressArbitrationField, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InNodeAddressArbitrationField->Fields = (uint32_t)*(buffer++);
  InNodeAddressArbitrationField->Fields |= ((uint32_t)*(buffer++) << 8);
  InNodeAddressArbitrationField->Fields |= ((uint32_t)*(buffer++) << 16);
  InNodeAddressArbitrationField->Fields |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : DSMNodeAddressArbitrationFieldCreate
 ******************************************************************************/
DSMNodeAddressArbitrationField*
DSMNodeAddressArbitrationFieldCreate
()
{
  DSMNodeAddressArbitrationField* returnDSMNodeAddressArbitrationField;
  returnDSMNodeAddressArbitrationField = (DSMNodeAddressArbitrationField*)malloc(sizeof(DSMNodeAddressArbitrationField));
  memset(returnDSMNodeAddressArbitrationField, 0x00, sizeof(DSMNodeAddressArbitrationField));
  return returnDSMNodeAddressArbitrationField;
}

/******************************************************************************!
 * Function : DSMNodeAddressArbitrationFieldDestroy
 ******************************************************************************/
void
DSMNodeAddressArbitrationFieldDestroy
(DSMNodeAddressArbitrationField* InDSMNodeAddressArbitrationField)
{
  if ( InDSMNodeAddressArbitrationField == NULL ) {
    return;
  }
  free(InDSMNodeAddressArbitrationField);
}

/******************************************************************************!
 * Function : DSMNodeAddressArbitrationFieldSet
 ******************************************************************************/
void
DSMNodeAddressArbitrationFieldSet
(DSMNodeAddressArbitrationField* InDSMNodeAddressArbitrationField,
 uint32_t InFields)
{
  if ( InDSMNodeAddressArbitrationField == NULL ) {
    return;
  }
  InDSMNodeAddressArbitrationField->Fields = InFields;
}

/******************************************************************************!
 * Function : DSMNodeAddressArbitrationFieldClear
 ******************************************************************************/
void
DSMNodeAddressArbitrationFieldClear
(DSMNodeAddressArbitrationField* InDSMNodeAddressArbitrationField)
{
  if ( InDSMNodeAddressArbitrationField == NULL ) {
    return;
  }
  memset(InDSMNodeAddressArbitrationField, 0x00, sizeof(DSMNodeAddressArbitrationField));
}

/******************************************************************************!
 * Function : DSMNodeAddressArbitrationFieldSetFull
 ******************************************************************************/
void
DSMNodeAddressArbitrationFieldSetFull
(DSMNodeAddressArbitrationField* InDSMNodeAddressArbitrationField,
 uint32_t InCommand,
 uint32_t InRatedCurrent,
 uint32_t InREZ,
 uint32_t InPriority,
 uint32_t InDirection)
{
  if ( InDSMNodeAddressArbitrationField == NULL ) {
    return;
  }
  InDSMNodeAddressArbitrationField->Command      = InCommand;
  InDSMNodeAddressArbitrationField->RatedCurrent = InRatedCurrent;
  InDSMNodeAddressArbitrationField->REZ          = InREZ;
  InDSMNodeAddressArbitrationField->Priority     = InPriority;
  InDSMNodeAddressArbitrationField->Direction    = InDirection;
}

/******************************************************************************!
 * Function : DSMNodeAddressArbitrationFieldPack
 ******************************************************************************/
void
DSMNodeAddressArbitrationFieldPack
(DSMNodeAddressArbitrationField* InDSMNodeAddressArbitrationField, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMNodeAddressArbitrationField == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMNodeAddressArbitrationField->Fields) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMNodeAddressArbitrationField->Fields >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMNodeAddressArbitrationField->Fields >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMNodeAddressArbitrationField->Fields >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMNodeAddressArbitrationFieldUnpack
 ******************************************************************************/
void
DSMNodeAddressArbitrationFieldUnpack
(DSMNodeAddressArbitrationField* InDSMNodeAddressArbitrationField, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMNodeAddressArbitrationField->Fields = (uint32_t)*(buffer++);
  InDSMNodeAddressArbitrationField->Fields |= ((uint32_t)*(buffer++) << 8);
  InDSMNodeAddressArbitrationField->Fields |= ((uint32_t)*(buffer++) << 16);
  InDSMNodeAddressArbitrationField->Fields |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : SystemTimeCreate
 ******************************************************************************/
SystemTime*
SystemTimeCreate
()
{
  SystemTime*                   returnSystemTime;
  returnSystemTime = (SystemTime*)malloc(sizeof(SystemTime));
  memset(returnSystemTime, 0x00, sizeof(SystemTime));
  return returnSystemTime;
}

/******************************************************************************!
 * Function : SystemTimeDestroy
 ******************************************************************************/
void
SystemTimeDestroy
(SystemTime* InSystemTime)
{
  if ( InSystemTime == NULL ) {
    return;
  }
  free(InSystemTime);
}

/******************************************************************************!
 * Function : SystemTimeSet
 ******************************************************************************/
void
SystemTimeSet
(SystemTime* InSystemTime,
 uint32_t InTime)
{
  if ( InSystemTime == NULL ) {
    return;
  }
  InSystemTime->Time = InTime;
}

/******************************************************************************!
 * Function : SystemTimeClear
 ******************************************************************************/
void
SystemTimeClear
(SystemTime* InSystemTime)
{
  if ( InSystemTime == NULL ) {
    return;
  }
  memset(InSystemTime, 0x00, sizeof(SystemTime));
}

/******************************************************************************!
 * Function : SystemTimeSetFull
 ******************************************************************************/
void
SystemTimeSetFull
(SystemTime* InSystemTime,
 uint32_t InTime)
{
  if ( InSystemTime == NULL ) {
    return;
  }
  InSystemTime->Time = InTime;
}

/******************************************************************************!
 * Function : SystemTimePack
 ******************************************************************************/
void
SystemTimePack
(SystemTime* InSystemTime, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InSystemTime == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InSystemTime->Time) & 0xFF;
  *(buffer++) = (unsigned char)(InSystemTime->Time >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InSystemTime->Time >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InSystemTime->Time >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : SystemTimeUnpack
 ******************************************************************************/
void
SystemTimeUnpack
(SystemTime* InSystemTime, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InSystemTime->Time = (uint32_t)*(buffer++);
  InSystemTime->Time |= ((uint32_t)*(buffer++) << 8);
  InSystemTime->Time |= ((uint32_t)*(buffer++) << 16);
  InSystemTime->Time |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : EStopTimeCreate
 ******************************************************************************/
EStopTime*
EStopTimeCreate
()
{
  EStopTime*                    returnEStopTime;
  returnEStopTime = (EStopTime*)malloc(sizeof(EStopTime));
  memset(returnEStopTime, 0x00, sizeof(EStopTime));
  return returnEStopTime;
}

/******************************************************************************!
 * Function : EStopTimeDestroy
 ******************************************************************************/
void
EStopTimeDestroy
(EStopTime* InEStopTime)
{
  if ( InEStopTime == NULL ) {
    return;
  }
  free(InEStopTime);
}

/******************************************************************************!
 * Function : EStopTimeSet
 ******************************************************************************/
void
EStopTimeSet
(EStopTime* InEStopTime,
 uint32_t InTime)
{
  if ( InEStopTime == NULL ) {
    return;
  }
  InEStopTime->Time = InTime;
}

/******************************************************************************!
 * Function : EStopTimeClear
 ******************************************************************************/
void
EStopTimeClear
(EStopTime* InEStopTime)
{
  if ( InEStopTime == NULL ) {
    return;
  }
  memset(InEStopTime, 0x00, sizeof(EStopTime));
}

/******************************************************************************!
 * Function : EStopTimeSetFull
 ******************************************************************************/
void
EStopTimeSetFull
(EStopTime* InEStopTime,
 uint32_t InTime)
{
  if ( InEStopTime == NULL ) {
    return;
  }
  InEStopTime->Time = InTime;
}

/******************************************************************************!
 * Function : EStopTimePack
 ******************************************************************************/
void
EStopTimePack
(EStopTime* InEStopTime, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InEStopTime == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InEStopTime->Time) & 0xFF;
  *(buffer++) = (unsigned char)(InEStopTime->Time >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InEStopTime->Time >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InEStopTime->Time >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : EStopTimeUnpack
 ******************************************************************************/
void
EStopTimeUnpack
(EStopTime* InEStopTime, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InEStopTime->Time = (uint32_t)*(buffer++);
  InEStopTime->Time |= ((uint32_t)*(buffer++) << 8);
  InEStopTime->Time |= ((uint32_t)*(buffer++) << 16);
  InEStopTime->Time |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : PartNumber1Create
 ******************************************************************************/
PartNumber1*
PartNumber1Create
()
{
  PartNumber1*                  returnPartNumber1;
  returnPartNumber1 = (PartNumber1*)malloc(sizeof(PartNumber1));
  memset(returnPartNumber1, 0x00, sizeof(PartNumber1));
  return returnPartNumber1;
}

/******************************************************************************!
 * Function : PartNumber1Destroy
 ******************************************************************************/
void
PartNumber1Destroy
(PartNumber1* InPartNumber1)
{
  if ( InPartNumber1 == NULL ) {
    return;
  }
  free(InPartNumber1);
}

/******************************************************************************!
 * Function : PartNumber1Set
 ******************************************************************************/
void
PartNumber1Set
(PartNumber1* InPartNumber1,
 char* InPartNumber)
{
  int					i;
  if ( InPartNumber1 == NULL ) {
    return;
  }
  memset(InPartNumber1->PartNumber, 0x00, 9);
  for (i = 0; i < 8 && InPartNumber[i] != 0x00; i++) {
    InPartNumber1->PartNumber[i] = InPartNumber[i];
  }
}

/******************************************************************************!
 * Function : PartNumber1Clear
 ******************************************************************************/
void
PartNumber1Clear
(PartNumber1* InPartNumber1)
{
  if ( InPartNumber1 == NULL ) {
    return;
  }
  memset(InPartNumber1, 0x00, sizeof(PartNumber1));
}

/******************************************************************************!
 * Function : PartNumber1SetFull
 ******************************************************************************/
void
PartNumber1SetFull
(PartNumber1* InPartNumber1,
 char* InPartNumber)
{
  int					i;
  if ( InPartNumber1 == NULL ) {
    return;
  }
  memset(InPartNumber1->PartNumber, 0x00, 9);
  for (i = 0; i < 8 && InPartNumber[i] != 0x00; i++) {
    InPartNumber1->PartNumber[i] = InPartNumber[i];
  }
}

/******************************************************************************!
 * Function : PartNumber1Pack
 ******************************************************************************/
void
PartNumber1Pack
(PartNumber1* InPartNumber1, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InPartNumber1 == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  memcpy(buffer, InPartNumber1->PartNumber, 8);
  length += 8;
  *InLength = length;
}

/******************************************************************************!
 * Function : PartNumber1Unpack
 ******************************************************************************/
void
PartNumber1Unpack
(PartNumber1* InPartNumber1, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  memcpy(InPartNumber1->PartNumber, buffer, 8);
  InPartNumber1->PartNumber[8] = 0x00;
  buffer += 8;
}

/******************************************************************************!
 * Function : RevisionNumbersCreate
 ******************************************************************************/
RevisionNumbers*
RevisionNumbersCreate
()
{
  RevisionNumbers*              returnRevisionNumbers;
  returnRevisionNumbers = (RevisionNumbers*)malloc(sizeof(RevisionNumbers));
  memset(returnRevisionNumbers, 0x00, sizeof(RevisionNumbers));
  return returnRevisionNumbers;
}

/******************************************************************************!
 * Function : RevisionNumbersDestroy
 ******************************************************************************/
void
RevisionNumbersDestroy
(RevisionNumbers* InRevisionNumbers)
{
  if ( InRevisionNumbers == NULL ) {
    return;
  }
  free(InRevisionNumbers);
}

/******************************************************************************!
 * Function : RevisionNumbersSet
 ******************************************************************************/
void
RevisionNumbersSet
(RevisionNumbers* InRevisionNumbers,
 uint8_t InBootMajor,
 uint8_t InBootMinor,
 uint8_t InBootBugFix,
 uint8_t InBootBuild,
 uint8_t InAppMajor,
 uint8_t InAppMinor,
 uint8_t InAppBugFix,
 uint8_t InAppBuild)
{
  if ( InRevisionNumbers == NULL ) {
    return;
  }
  InRevisionNumbers->BootMajor  = InBootMajor;
  InRevisionNumbers->BootMinor  = InBootMinor;
  InRevisionNumbers->BootBugFix = InBootBugFix;
  InRevisionNumbers->BootBuild  = InBootBuild;
  InRevisionNumbers->AppMajor   = InAppMajor;
  InRevisionNumbers->AppMinor   = InAppMinor;
  InRevisionNumbers->AppBugFix  = InAppBugFix;
  InRevisionNumbers->AppBuild   = InAppBuild;
}

/******************************************************************************!
 * Function : RevisionNumbersClear
 ******************************************************************************/
void
RevisionNumbersClear
(RevisionNumbers* InRevisionNumbers)
{
  if ( InRevisionNumbers == NULL ) {
    return;
  }
  memset(InRevisionNumbers, 0x00, sizeof(RevisionNumbers));
}

/******************************************************************************!
 * Function : RevisionNumbersSetFull
 ******************************************************************************/
void
RevisionNumbersSetFull
(RevisionNumbers* InRevisionNumbers,
 uint8_t InBootMajor,
 uint8_t InBootMinor,
 uint8_t InBootBugFix,
 uint8_t InBootBuild,
 uint8_t InAppMajor,
 uint8_t InAppMinor,
 uint8_t InAppBugFix,
 uint8_t InAppBuild)
{
  if ( InRevisionNumbers == NULL ) {
    return;
  }
  InRevisionNumbers->BootMajor  = InBootMajor;
  InRevisionNumbers->BootMinor  = InBootMinor;
  InRevisionNumbers->BootBugFix = InBootBugFix;
  InRevisionNumbers->BootBuild  = InBootBuild;
  InRevisionNumbers->AppMajor   = InAppMajor;
  InRevisionNumbers->AppMinor   = InAppMinor;
  InRevisionNumbers->AppBugFix  = InAppBugFix;
  InRevisionNumbers->AppBuild   = InAppBuild;
}

/******************************************************************************!
 * Function : RevisionNumbersPack
 ******************************************************************************/
void
RevisionNumbersPack
(RevisionNumbers* InRevisionNumbers, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRevisionNumbers == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)InRevisionNumbers->AppBuild;
  length++;
  *(buffer++) = (unsigned char)InRevisionNumbers->AppBugFix;
  length++;
  *(buffer++) = (unsigned char)InRevisionNumbers->AppMinor;
  length++;
  *(buffer++) = (unsigned char)InRevisionNumbers->AppMajor;
  length++;
  *(buffer++) = (unsigned char)InRevisionNumbers->BootBuild;
  length++;
  *(buffer++) = (unsigned char)InRevisionNumbers->BootBugFix;
  length++;
  *(buffer++) = (unsigned char)InRevisionNumbers->BootMinor;
  length++;
  *(buffer++) = (unsigned char)InRevisionNumbers->BootMajor;
  length++;
  *InLength = length;
}

/******************************************************************************!
 * Function : RevisionNumbersUnpack
 ******************************************************************************/
void
RevisionNumbersUnpack
(RevisionNumbers* InRevisionNumbers, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InRevisionNumbers->AppBuild = (uint8_t)*(buffer++);
  InRevisionNumbers->AppBugFix = (uint8_t)*(buffer++);
  InRevisionNumbers->AppMinor = (uint8_t)*(buffer++);
  InRevisionNumbers->AppMajor = (uint8_t)*(buffer++);
  InRevisionNumbers->BootBuild = (uint8_t)*(buffer++);
  InRevisionNumbers->BootBugFix = (uint8_t)*(buffer++);
  InRevisionNumbers->BootMinor = (uint8_t)*(buffer++);
  InRevisionNumbers->BootMajor = (uint8_t)*(buffer++);
}

/******************************************************************************!
 * Function : RectifierRatedInputVoltCreate
 ******************************************************************************/
RectifierRatedInputVolt*
RectifierRatedInputVoltCreate
()
{
  RectifierRatedInputVolt*      returnRectifierRatedInputVolt;
  returnRectifierRatedInputVolt = (RectifierRatedInputVolt*)malloc(sizeof(RectifierRatedInputVolt));
  memset(returnRectifierRatedInputVolt, 0x00, sizeof(RectifierRatedInputVolt));
  return returnRectifierRatedInputVolt;
}

/******************************************************************************!
 * Function : RectifierRatedInputVoltDestroy
 ******************************************************************************/
void
RectifierRatedInputVoltDestroy
(RectifierRatedInputVolt* InRectifierRatedInputVolt)
{
  if ( InRectifierRatedInputVolt == NULL ) {
    return;
  }
  free(InRectifierRatedInputVolt);
}

/******************************************************************************!
 * Function : RectifierRatedInputVoltSet
 ******************************************************************************/
void
RectifierRatedInputVoltSet
(RectifierRatedInputVolt* InRectifierRatedInputVolt,
 int16_t InVolts,
 char* InVoltType)
{
  int					i;

  if ( InRectifierRatedInputVolt == NULL ) {
    return;
  }
  InRectifierRatedInputVolt->volts    = InVolts;
  memset(InRectifierRatedInputVolt->voltType, 0x00, 3);
  for (i = 0; i < 2 && InVoltType[i] != 0x00; i++) {
    InRectifierRatedInputVolt->voltType[i] = InVoltType[i];
  }
}

/******************************************************************************!
 * Function : RectifierRatedInputVoltClear
 ******************************************************************************/
void
RectifierRatedInputVoltClear
(RectifierRatedInputVolt* InRectifierRatedInputVolt)
{
  if ( InRectifierRatedInputVolt == NULL ) {
    return;
  }
  memset(InRectifierRatedInputVolt, 0x00, sizeof(RectifierRatedInputVolt));
}

/******************************************************************************!
 * Function : RectifierRatedInputVoltSetFull
 ******************************************************************************/
void
RectifierRatedInputVoltSetFull
(RectifierRatedInputVolt* InRectifierRatedInputVolt,
 int16_t InVolts,
 char* InVoltType)
{
  int					i;
  if ( InRectifierRatedInputVolt == NULL ) {
    return;
  }
  InRectifierRatedInputVolt->volts    = InVolts;
  memset(InRectifierRatedInputVolt->voltType, 0x00, 3);
  for (i = 0; i < 2 && InVoltType[i] != 0x00; i++) {
    InRectifierRatedInputVolt->voltType[i] = InVoltType[i];
  }
}

/******************************************************************************!
 * Function : RectifierRatedInputVoltPack
 ******************************************************************************/
void
RectifierRatedInputVoltPack
(RectifierRatedInputVolt* InRectifierRatedInputVolt, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRectifierRatedInputVolt == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  memcpy(buffer, InRectifierRatedInputVolt->voltType, 2);
  length += 2;
  *(buffer++) = (unsigned char)(InRectifierRatedInputVolt->volts) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierRatedInputVolt->volts >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : RectifierRatedInputVoltUnpack
 ******************************************************************************/
void
RectifierRatedInputVoltUnpack
(RectifierRatedInputVolt* InRectifierRatedInputVolt, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  memcpy(InRectifierRatedInputVolt->voltType, buffer, 2);
  InRectifierRatedInputVolt->voltType[2] = 0x00;
  buffer += 2;
  InRectifierRatedInputVolt->volts = (int16_t)*(buffer++);
  InRectifierRatedInputVolt->volts |= ((int16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : RectifierRatedOutputVoltCreate
 ******************************************************************************/
RectifierRatedOutputVolt*
RectifierRatedOutputVoltCreate
()
{
  RectifierRatedOutputVolt*     returnRectifierRatedOutputVolt;
  returnRectifierRatedOutputVolt = (RectifierRatedOutputVolt*)malloc(sizeof(RectifierRatedOutputVolt));
  memset(returnRectifierRatedOutputVolt, 0x00, sizeof(RectifierRatedOutputVolt));
  return returnRectifierRatedOutputVolt;
}

/******************************************************************************!
 * Function : RectifierRatedOutputVoltDestroy
 ******************************************************************************/
void
RectifierRatedOutputVoltDestroy
(RectifierRatedOutputVolt* InRectifierRatedOutputVolt)
{
  if ( InRectifierRatedOutputVolt == NULL ) {
    return;
  }
  free(InRectifierRatedOutputVolt);
}

/******************************************************************************!
 * Function : RectifierRatedOutputVoltSet
 ******************************************************************************/
void
RectifierRatedOutputVoltSet
(RectifierRatedOutputVolt* InRectifierRatedOutputVolt,
 int16_t InVolts,
 char* InVoltType)
{
  int					i;
  if ( InRectifierRatedOutputVolt == NULL ) {
    return;
  }
  InRectifierRatedOutputVolt->volts    = InVolts;
  memset(InRectifierRatedOutputVolt->voltType, 0x00, 3);
  for (i = 0; i < 2 && InVoltType[i] != 0x00; i++) {
    InRectifierRatedOutputVolt->voltType[i] = InVoltType[i];
  }
}

/******************************************************************************!
 * Function : RectifierRatedOutputVoltClear
 ******************************************************************************/
void
RectifierRatedOutputVoltClear
(RectifierRatedOutputVolt* InRectifierRatedOutputVolt)
{
  if ( InRectifierRatedOutputVolt == NULL ) {
    return;
  }
  memset(InRectifierRatedOutputVolt, 0x00, sizeof(RectifierRatedOutputVolt));
}

/******************************************************************************!
 * Function : RectifierRatedOutputVoltSetFull
 ******************************************************************************/
void
RectifierRatedOutputVoltSetFull
(RectifierRatedOutputVolt* InRectifierRatedOutputVolt,
 int16_t InVolts,
 char* InVoltType)
{
  int					i;
  if ( InRectifierRatedOutputVolt == NULL ) {
    return;
  }
  InRectifierRatedOutputVolt->volts    = InVolts;
  memset(InRectifierRatedOutputVolt->voltType, 0x00, 3);
  for (i = 0; i < 2 && InVoltType[i] != 0x00; i++) {
    InRectifierRatedOutputVolt->voltType[i] = InVoltType[i];
  }
}

/******************************************************************************!
 * Function : RectifierRatedOutputVoltPack
 ******************************************************************************/
void
RectifierRatedOutputVoltPack
(RectifierRatedOutputVolt* InRectifierRatedOutputVolt, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRectifierRatedOutputVolt == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  memcpy(buffer, InRectifierRatedOutputVolt->voltType, 2);
  length += 2;
  *(buffer++) = (unsigned char)(InRectifierRatedOutputVolt->volts) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierRatedOutputVolt->volts >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : RectifierRatedOutputVoltUnpack
 ******************************************************************************/
void
RectifierRatedOutputVoltUnpack
(RectifierRatedOutputVolt* InRectifierRatedOutputVolt, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  memcpy(InRectifierRatedOutputVolt->voltType, buffer, 2);
  InRectifierRatedOutputVolt->voltType[2] = 0x00;
  buffer += 2;
  InRectifierRatedOutputVolt->volts = (int16_t)*(buffer++);
  InRectifierRatedOutputVolt->volts |= ((int16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : RectifierRatedOutputCurrentCreate
 ******************************************************************************/
RectifierRatedOutputCurrent*
RectifierRatedOutputCurrentCreate
()
{
  RectifierRatedOutputCurrent*  returnRectifierRatedOutputCurrent;
  returnRectifierRatedOutputCurrent = (RectifierRatedOutputCurrent*)malloc(sizeof(RectifierRatedOutputCurrent));
  memset(returnRectifierRatedOutputCurrent, 0x00, sizeof(RectifierRatedOutputCurrent));
  return returnRectifierRatedOutputCurrent;
}

/******************************************************************************!
 * Function : RectifierRatedOutputCurrentDestroy
 ******************************************************************************/
void
RectifierRatedOutputCurrentDestroy
(RectifierRatedOutputCurrent* InRectifierRatedOutputCurrent)
{
  if ( InRectifierRatedOutputCurrent == NULL ) {
    return;
  }
  free(InRectifierRatedOutputCurrent);
}

/******************************************************************************!
 * Function : RectifierRatedOutputCurrentSet
 ******************************************************************************/
void
RectifierRatedOutputCurrentSet
(RectifierRatedOutputCurrent* InRectifierRatedOutputCurrent,
 int16_t InCurrent,
 char* InVoltType,
 int16_t InMaxCurrentCapacity)
{
  int					i;
  if ( InRectifierRatedOutputCurrent == NULL ) {
    return;
  }
  InRectifierRatedOutputCurrent->current            = InCurrent;
  memset(InRectifierRatedOutputCurrent->voltType, 0x00, 3);
  for (i = 0; i < 2 && InVoltType[i] != 0x00; i++) {
    InRectifierRatedOutputCurrent->voltType[i] = InVoltType[i];
  }
  InRectifierRatedOutputCurrent->maxCurrentCapacity = InMaxCurrentCapacity;
}

/******************************************************************************!
 * Function : RectifierRatedOutputCurrentClear
 ******************************************************************************/
void
RectifierRatedOutputCurrentClear
(RectifierRatedOutputCurrent* InRectifierRatedOutputCurrent)
{
  if ( InRectifierRatedOutputCurrent == NULL ) {
    return;
  }
  memset(InRectifierRatedOutputCurrent, 0x00, sizeof(RectifierRatedOutputCurrent));
}

/******************************************************************************!
 * Function : RectifierRatedOutputCurrentSetFull
 ******************************************************************************/
void
RectifierRatedOutputCurrentSetFull
(RectifierRatedOutputCurrent* InRectifierRatedOutputCurrent,
 int16_t InCurrent,
 char* InVoltType,
 int16_t InMaxCurrentCapacity)
{
  int					i;
  if ( InRectifierRatedOutputCurrent == NULL ) {
    return;
  }
  InRectifierRatedOutputCurrent->current            = InCurrent;
  memset(InRectifierRatedOutputCurrent->voltType, 0x00, 3);
  for (i = 0; i < 2 && InVoltType[i] != 0x00; i++) {
    InRectifierRatedOutputCurrent->voltType[i] = InVoltType[i];
  }
  InRectifierRatedOutputCurrent->maxCurrentCapacity = InMaxCurrentCapacity;
}

/******************************************************************************!
 * Function : RectifierRatedOutputCurrentPack
 ******************************************************************************/
void
RectifierRatedOutputCurrentPack
(RectifierRatedOutputCurrent* InRectifierRatedOutputCurrent, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRectifierRatedOutputCurrent == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InRectifierRatedOutputCurrent->maxCurrentCapacity) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierRatedOutputCurrent->maxCurrentCapacity >> 8) & 0xFF;
  length += 2;
  memcpy(buffer, InRectifierRatedOutputCurrent->voltType, 2);
  length += 2;
  *(buffer++) = (unsigned char)(InRectifierRatedOutputCurrent->current) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierRatedOutputCurrent->current >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : RectifierRatedOutputCurrentUnpack
 ******************************************************************************/
void
RectifierRatedOutputCurrentUnpack
(RectifierRatedOutputCurrent* InRectifierRatedOutputCurrent, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InRectifierRatedOutputCurrent->maxCurrentCapacity = (int16_t)*(buffer++);
  InRectifierRatedOutputCurrent->maxCurrentCapacity |= ((int16_t)*(buffer++) << 8);
  memcpy(InRectifierRatedOutputCurrent->voltType, buffer, 2);
  InRectifierRatedOutputCurrent->voltType[2] = 0x00;
  buffer += 2;
  InRectifierRatedOutputCurrent->current = (int16_t)*(buffer++);
  InRectifierRatedOutputCurrent->current |= ((int16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : RectifierDefaultParametersCreate
 ******************************************************************************/
RectifierDefaultParameters*
RectifierDefaultParametersCreate
()
{
  RectifierDefaultParameters*   returnRectifierDefaultParameters;
  returnRectifierDefaultParameters = (RectifierDefaultParameters*)malloc(sizeof(RectifierDefaultParameters));
  memset(returnRectifierDefaultParameters, 0x00, sizeof(RectifierDefaultParameters));
  return returnRectifierDefaultParameters;
}

/******************************************************************************!
 * Function : RectifierDefaultParametersDestroy
 ******************************************************************************/
void
RectifierDefaultParametersDestroy
(RectifierDefaultParameters* InRectifierDefaultParameters)
{
  if ( InRectifierDefaultParameters == NULL ) {
    return;
  }
  free(InRectifierDefaultParameters);
}

/******************************************************************************!
 * Function : RectifierDefaultParametersSet
 ******************************************************************************/
void
RectifierDefaultParametersSet
(RectifierDefaultParameters* InRectifierDefaultParameters,
 uint16_t InVoltageOut,
 uint16_t InHVSOut)
{
  if ( InRectifierDefaultParameters == NULL ) {
    return;
  }
  InRectifierDefaultParameters->VoltageOut = InVoltageOut;
  InRectifierDefaultParameters->HVSOut     = InHVSOut;
}

/******************************************************************************!
 * Function : RectifierDefaultParametersClear
 ******************************************************************************/
void
RectifierDefaultParametersClear
(RectifierDefaultParameters* InRectifierDefaultParameters)
{
  if ( InRectifierDefaultParameters == NULL ) {
    return;
  }
  memset(InRectifierDefaultParameters, 0x00, sizeof(RectifierDefaultParameters));
}

/******************************************************************************!
 * Function : RectifierDefaultParametersSetFull
 ******************************************************************************/
void
RectifierDefaultParametersSetFull
(RectifierDefaultParameters* InRectifierDefaultParameters,
 uint16_t InVoltageOut,
 uint16_t InHVSOut)
{
  if ( InRectifierDefaultParameters == NULL ) {
    return;
  }
  InRectifierDefaultParameters->VoltageOut = InVoltageOut;
  InRectifierDefaultParameters->HVSOut     = InHVSOut;
}

/******************************************************************************!
 * Function : RectifierDefaultParametersPack
 ******************************************************************************/
void
RectifierDefaultParametersPack
(RectifierDefaultParameters* InRectifierDefaultParameters, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRectifierDefaultParameters == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InRectifierDefaultParameters->HVSOut) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierDefaultParameters->HVSOut >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InRectifierDefaultParameters->VoltageOut) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierDefaultParameters->VoltageOut >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : RectifierDefaultParametersUnpack
 ******************************************************************************/
void
RectifierDefaultParametersUnpack
(RectifierDefaultParameters* InRectifierDefaultParameters, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InRectifierDefaultParameters->HVSOut = (uint16_t)*(buffer++);
  InRectifierDefaultParameters->HVSOut |= ((uint16_t)*(buffer++) << 8);
  InRectifierDefaultParameters->VoltageOut = (uint16_t)*(buffer++);
  InRectifierDefaultParameters->VoltageOut |= ((uint16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : RectifierNormalParametersCreate
 ******************************************************************************/
RectifierNormalParameters*
RectifierNormalParametersCreate
()
{
  RectifierNormalParameters*    returnRectifierNormalParameters;
  returnRectifierNormalParameters = (RectifierNormalParameters*)malloc(sizeof(RectifierNormalParameters));
  memset(returnRectifierNormalParameters, 0x00, sizeof(RectifierNormalParameters));
  return returnRectifierNormalParameters;
}

/******************************************************************************!
 * Function : RectifierNormalParametersDestroy
 ******************************************************************************/
void
RectifierNormalParametersDestroy
(RectifierNormalParameters* InRectifierNormalParameters)
{
  if ( InRectifierNormalParameters == NULL ) {
    return;
  }
  free(InRectifierNormalParameters);
}

/******************************************************************************!
 * Function : RectifierNormalParametersSet
 ******************************************************************************/
void
RectifierNormalParametersSet
(RectifierNormalParameters* InRectifierNormalParameters,
 uint16_t InVoltsOut,
 uint16_t InCurrent,
 uint16_t InHvsVolts,
 uint8_t InStatus)
{
  if ( InRectifierNormalParameters == NULL ) {
    return;
  }
  InRectifierNormalParameters->voltsOut = InVoltsOut;
  InRectifierNormalParameters->current  = InCurrent;
  InRectifierNormalParameters->hvsVolts = InHvsVolts;
  InRectifierNormalParameters->status   = InStatus;
}

/******************************************************************************!
 * Function : RectifierNormalParametersClear
 ******************************************************************************/
void
RectifierNormalParametersClear
(RectifierNormalParameters* InRectifierNormalParameters)
{
  if ( InRectifierNormalParameters == NULL ) {
    return;
  }
  memset(InRectifierNormalParameters, 0x00, sizeof(RectifierNormalParameters));
}

/******************************************************************************!
 * Function : RectifierNormalParametersSetFull
 ******************************************************************************/
void
RectifierNormalParametersSetFull
(RectifierNormalParameters* InRectifierNormalParameters,
 uint16_t InVoltsOut,
 uint16_t InCurrent,
 uint16_t InHvsVolts,
 uint8_t InTCO,
 uint8_t InDLS,
 uint8_t InTEO,
 uint8_t InDCP,
 uint8_t InWAL,
 uint8_t InCTC,
 uint8_t InRDL,
 uint8_t InACO)
{
  if ( InRectifierNormalParameters == NULL ) {
    return;
  }
  InRectifierNormalParameters->voltsOut = InVoltsOut;
  InRectifierNormalParameters->current  = InCurrent;
  InRectifierNormalParameters->hvsVolts = InHvsVolts;
  InRectifierNormalParameters->TCO      = InTCO;
  InRectifierNormalParameters->DLS      = InDLS;
  InRectifierNormalParameters->TEO      = InTEO;
  InRectifierNormalParameters->DCP      = InDCP;
  InRectifierNormalParameters->WAL      = InWAL;
  InRectifierNormalParameters->CTC      = InCTC;
  InRectifierNormalParameters->RDL      = InRDL;
  InRectifierNormalParameters->ACO      = InACO;
}

/******************************************************************************!
 * Function : RectifierNormalParametersPack
 ******************************************************************************/
void
RectifierNormalParametersPack
(RectifierNormalParameters* InRectifierNormalParameters, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRectifierNormalParameters == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)InRectifierNormalParameters->status;
  length++;
  *(buffer++) = (unsigned char)InRectifierNormalParameters->Empty5;
  length++;
  *(buffer++) = (unsigned char)(InRectifierNormalParameters->hvsVolts) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierNormalParameters->hvsVolts >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InRectifierNormalParameters->current) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierNormalParameters->current >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InRectifierNormalParameters->voltsOut) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierNormalParameters->voltsOut >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : RectifierNormalParametersUnpack
 ******************************************************************************/
void
RectifierNormalParametersUnpack
(RectifierNormalParameters* InRectifierNormalParameters, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InRectifierNormalParameters->status = (uint8_t)*(buffer++);
  InRectifierNormalParameters->Empty5 = (uint8_t)*(buffer++);
  InRectifierNormalParameters->hvsVolts = (uint16_t)*(buffer++);
  InRectifierNormalParameters->hvsVolts |= ((uint16_t)*(buffer++) << 8);
  InRectifierNormalParameters->current = (uint16_t)*(buffer++);
  InRectifierNormalParameters->current |= ((uint16_t)*(buffer++) << 8);
  InRectifierNormalParameters->voltsOut = (uint16_t)*(buffer++);
  InRectifierNormalParameters->voltsOut |= ((uint16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : DSMShuntStatusCreate
 ******************************************************************************/
DSMShuntStatus*
DSMShuntStatusCreate
()
{
  DSMShuntStatus*               returnDSMShuntStatus;
  returnDSMShuntStatus = (DSMShuntStatus*)malloc(sizeof(DSMShuntStatus));
  memset(returnDSMShuntStatus, 0x00, sizeof(DSMShuntStatus));
  return returnDSMShuntStatus;
}

/******************************************************************************!
 * Function : DSMShuntStatusDestroy
 ******************************************************************************/
void
DSMShuntStatusDestroy
(DSMShuntStatus* InDSMShuntStatus)
{
  if ( InDSMShuntStatus == NULL ) {
    return;
  }
  free(InDSMShuntStatus);
}

/******************************************************************************!
 * Function : DSMShuntStatusSet
 ******************************************************************************/
void
DSMShuntStatusSet
(DSMShuntStatus* InDSMShuntStatus,
 uint32_t InStatusHighBits,
 uint32_t InStatusLowBits)
{
  if ( InDSMShuntStatus == NULL ) {
    return;
  }
  InDSMShuntStatus->StatusHighBits = InStatusHighBits;
  InDSMShuntStatus->StatusLowBits  = InStatusLowBits;
}

/******************************************************************************!
 * Function : DSMShuntStatusClear
 ******************************************************************************/
void
DSMShuntStatusClear
(DSMShuntStatus* InDSMShuntStatus)
{
  if ( InDSMShuntStatus == NULL ) {
    return;
  }
  memset(InDSMShuntStatus, 0x00, sizeof(DSMShuntStatus));
}

/******************************************************************************!
 * Function : DSMShuntStatusSetFull
 ******************************************************************************/
void
DSMShuntStatusSetFull
(DSMShuntStatus* InDSMShuntStatus,
 uint32_t InHiBit24,
 uint32_t InHiBit23,
 uint32_t InHiBit22,
 uint32_t InHiBit21,
 uint32_t InHiBit20,
 uint32_t InHiBit19,
 uint32_t InHiBit18,
 uint32_t InHiBit17,
 uint32_t InHiBit16,
 uint32_t InHiBit15,
 uint32_t InHiBit14,
 uint32_t InHiBit13,
 uint32_t InHiBit12,
 uint32_t InHiBit11,
 uint32_t InHiBit10,
 uint32_t InHiBit09,
 uint32_t InHiBit08,
 uint32_t InHiBit07,
 uint32_t InHiBit06,
 uint32_t InHiBit05,
 uint32_t InHiBit04,
 uint32_t InHiBit03,
 uint32_t InHiBit02,
 uint32_t InHiBit01,
 uint32_t InLoBit24,
 uint32_t InLoBit23,
 uint32_t InLoBit22,
 uint32_t InLoBit21,
 uint32_t InLoBit20,
 uint32_t InLoBit19,
 uint32_t InLoBit18,
 uint32_t InLoBit17,
 uint32_t InLoBit16,
 uint32_t InLoBit15,
 uint32_t InLoBit14,
 uint32_t InLoBit13,
 uint32_t InLoBit12,
 uint32_t InLoBit11,
 uint32_t InLoBit10,
 uint32_t InLoBit09,
 uint32_t InLoBit08,
 uint32_t InLoBit07,
 uint32_t InLoBit06,
 uint32_t InLoBit05,
 uint32_t InLoBit04,
 uint32_t InLoBit03,
 uint32_t InLoBit02,
 uint32_t InLoBit01)
{
  if ( InDSMShuntStatus == NULL ) {
    return;
  }
  InDSMShuntStatus->HiBit24 = InHiBit24;
  InDSMShuntStatus->HiBit23 = InHiBit23;
  InDSMShuntStatus->HiBit22 = InHiBit22;
  InDSMShuntStatus->HiBit21 = InHiBit21;
  InDSMShuntStatus->HiBit20 = InHiBit20;
  InDSMShuntStatus->HiBit19 = InHiBit19;
  InDSMShuntStatus->HiBit18 = InHiBit18;
  InDSMShuntStatus->HiBit17 = InHiBit17;
  InDSMShuntStatus->HiBit16 = InHiBit16;
  InDSMShuntStatus->HiBit15 = InHiBit15;
  InDSMShuntStatus->HiBit14 = InHiBit14;
  InDSMShuntStatus->HiBit13 = InHiBit13;
  InDSMShuntStatus->HiBit12 = InHiBit12;
  InDSMShuntStatus->HiBit11 = InHiBit11;
  InDSMShuntStatus->HiBit10 = InHiBit10;
  InDSMShuntStatus->HiBit09 = InHiBit09;
  InDSMShuntStatus->HiBit08 = InHiBit08;
  InDSMShuntStatus->HiBit07 = InHiBit07;
  InDSMShuntStatus->HiBit06 = InHiBit06;
  InDSMShuntStatus->HiBit05 = InHiBit05;
  InDSMShuntStatus->HiBit04 = InHiBit04;
  InDSMShuntStatus->HiBit03 = InHiBit03;
  InDSMShuntStatus->HiBit02 = InHiBit02;
  InDSMShuntStatus->HiBit01 = InHiBit01;
  InDSMShuntStatus->LoBit24 = InLoBit24;
  InDSMShuntStatus->LoBit23 = InLoBit23;
  InDSMShuntStatus->LoBit22 = InLoBit22;
  InDSMShuntStatus->LoBit21 = InLoBit21;
  InDSMShuntStatus->LoBit20 = InLoBit20;
  InDSMShuntStatus->LoBit19 = InLoBit19;
  InDSMShuntStatus->LoBit18 = InLoBit18;
  InDSMShuntStatus->LoBit17 = InLoBit17;
  InDSMShuntStatus->LoBit16 = InLoBit16;
  InDSMShuntStatus->LoBit15 = InLoBit15;
  InDSMShuntStatus->LoBit14 = InLoBit14;
  InDSMShuntStatus->LoBit13 = InLoBit13;
  InDSMShuntStatus->LoBit12 = InLoBit12;
  InDSMShuntStatus->LoBit11 = InLoBit11;
  InDSMShuntStatus->LoBit10 = InLoBit10;
  InDSMShuntStatus->LoBit09 = InLoBit09;
  InDSMShuntStatus->LoBit08 = InLoBit08;
  InDSMShuntStatus->LoBit07 = InLoBit07;
  InDSMShuntStatus->LoBit06 = InLoBit06;
  InDSMShuntStatus->LoBit05 = InLoBit05;
  InDSMShuntStatus->LoBit04 = InLoBit04;
  InDSMShuntStatus->LoBit03 = InLoBit03;
  InDSMShuntStatus->LoBit02 = InLoBit02;
  InDSMShuntStatus->LoBit01 = InLoBit01;
}

/******************************************************************************!
 * Function : DSMShuntStatusPack
 ******************************************************************************/
void
DSMShuntStatusPack
(DSMShuntStatus* InDSMShuntStatus, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMShuntStatus == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusLowBits) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusLowBits >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusLowBits >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusLowBits >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusHighBits) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusHighBits >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusHighBits >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMShuntStatus->StatusHighBits >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMShuntStatusUnpack
 ******************************************************************************/
void
DSMShuntStatusUnpack
(DSMShuntStatus* InDSMShuntStatus, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMShuntStatus->StatusLowBits = (uint32_t)*(buffer++);
  InDSMShuntStatus->StatusLowBits |= ((uint32_t)*(buffer++) << 8);
  InDSMShuntStatus->StatusLowBits |= ((uint32_t)*(buffer++) << 16);
  InDSMShuntStatus->StatusLowBits |= ((uint32_t)*(buffer++) << 24);
  InDSMShuntStatus->StatusHighBits = (uint32_t)*(buffer++);
  InDSMShuntStatus->StatusHighBits |= ((uint32_t)*(buffer++) << 8);
  InDSMShuntStatus->StatusHighBits |= ((uint32_t)*(buffer++) << 16);
  InDSMShuntStatus->StatusHighBits |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : DSMReportCurrents2Create
 ******************************************************************************/
DSMReportCurrents2*
DSMReportCurrents2Create
()
{
  DSMReportCurrents2*           returnDSMReportCurrents2;
  returnDSMReportCurrents2 = (DSMReportCurrents2*)malloc(sizeof(DSMReportCurrents2));
  memset(returnDSMReportCurrents2, 0x00, sizeof(DSMReportCurrents2));
  return returnDSMReportCurrents2;
}

/******************************************************************************!
 * Function : DSMReportCurrents2Destroy
 ******************************************************************************/
void
DSMReportCurrents2Destroy
(DSMReportCurrents2* InDSMReportCurrents2)
{
  if ( InDSMReportCurrents2 == NULL ) {
    return;
  }
  free(InDSMReportCurrents2);
}

/******************************************************************************!
 * Function : DSMReportCurrents2Set
 ******************************************************************************/
void
DSMReportCurrents2Set
(DSMReportCurrents2* InDSMReportCurrents2,
 uint16_t InAmps4,
 uint16_t InAmps3,
 uint16_t InAmps2,
 uint16_t InAmps1)
{
  if ( InDSMReportCurrents2 == NULL ) {
    return;
  }
  InDSMReportCurrents2->amps4 = InAmps4;
  InDSMReportCurrents2->amps3 = InAmps3;
  InDSMReportCurrents2->amps2 = InAmps2;
  InDSMReportCurrents2->amps1 = InAmps1;
}

/******************************************************************************!
 * Function : DSMReportCurrents2Clear
 ******************************************************************************/
void
DSMReportCurrents2Clear
(DSMReportCurrents2* InDSMReportCurrents2)
{
  if ( InDSMReportCurrents2 == NULL ) {
    return;
  }
  memset(InDSMReportCurrents2, 0x00, sizeof(DSMReportCurrents2));
}

/******************************************************************************!
 * Function : DSMReportCurrents2SetFull
 ******************************************************************************/
void
DSMReportCurrents2SetFull
(DSMReportCurrents2* InDSMReportCurrents2,
 uint16_t InAmps4,
 uint16_t InAmps3,
 uint16_t InAmps2,
 uint16_t InAmps1)
{
  if ( InDSMReportCurrents2 == NULL ) {
    return;
  }
  InDSMReportCurrents2->amps4 = InAmps4;
  InDSMReportCurrents2->amps3 = InAmps3;
  InDSMReportCurrents2->amps2 = InAmps2;
  InDSMReportCurrents2->amps1 = InAmps1;
}

/******************************************************************************!
 * Function : DSMReportCurrents2Pack
 ******************************************************************************/
void
DSMReportCurrents2Pack
(DSMReportCurrents2* InDSMReportCurrents2, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMReportCurrents2 == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps1) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps1 >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps2) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps2 >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps3) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps3 >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps4) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents2->amps4 >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMReportCurrents2Unpack
 ******************************************************************************/
void
DSMReportCurrents2Unpack
(DSMReportCurrents2* InDSMReportCurrents2, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMReportCurrents2->amps1 = (uint16_t)*(buffer++);
  InDSMReportCurrents2->amps1 |= ((uint16_t)*(buffer++) << 8);
  InDSMReportCurrents2->amps2 = (uint16_t)*(buffer++);
  InDSMReportCurrents2->amps2 |= ((uint16_t)*(buffer++) << 8);
  InDSMReportCurrents2->amps3 = (uint16_t)*(buffer++);
  InDSMReportCurrents2->amps3 |= ((uint16_t)*(buffer++) << 8);
  InDSMReportCurrents2->amps4 = (uint16_t)*(buffer++);
  InDSMReportCurrents2->amps4 |= ((uint16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : DSMReportCurrents4Create
 ******************************************************************************/
DSMReportCurrents4*
DSMReportCurrents4Create
()
{
  DSMReportCurrents4*           returnDSMReportCurrents4;
  returnDSMReportCurrents4 = (DSMReportCurrents4*)malloc(sizeof(DSMReportCurrents4));
  memset(returnDSMReportCurrents4, 0x00, sizeof(DSMReportCurrents4));
  return returnDSMReportCurrents4;
}

/******************************************************************************!
 * Function : DSMReportCurrents4Destroy
 ******************************************************************************/
void
DSMReportCurrents4Destroy
(DSMReportCurrents4* InDSMReportCurrents4)
{
  if ( InDSMReportCurrents4 == NULL ) {
    return;
  }
  free(InDSMReportCurrents4);
}

/******************************************************************************!
 * Function : DSMReportCurrents4Set
 ******************************************************************************/
void
DSMReportCurrents4Set
(DSMReportCurrents4* InDSMReportCurrents4,
 uint32_t InAmps1,
 uint32_t InAmps2)
{
  if ( InDSMReportCurrents4 == NULL ) {
    return;
  }
  InDSMReportCurrents4->amps1 = InAmps1;
  InDSMReportCurrents4->amps2 = InAmps2;
}

/******************************************************************************!
 * Function : DSMReportCurrents4Clear
 ******************************************************************************/
void
DSMReportCurrents4Clear
(DSMReportCurrents4* InDSMReportCurrents4)
{
  if ( InDSMReportCurrents4 == NULL ) {
    return;
  }
  memset(InDSMReportCurrents4, 0x00, sizeof(DSMReportCurrents4));
}

/******************************************************************************!
 * Function : DSMReportCurrents4SetFull
 ******************************************************************************/
void
DSMReportCurrents4SetFull
(DSMReportCurrents4* InDSMReportCurrents4,
 uint32_t InAmps1,
 uint32_t InAmps2)
{
  if ( InDSMReportCurrents4 == NULL ) {
    return;
  }
  InDSMReportCurrents4->amps1 = InAmps1;
  InDSMReportCurrents4->amps2 = InAmps2;
}

/******************************************************************************!
 * Function : DSMReportCurrents4Pack
 ******************************************************************************/
void
DSMReportCurrents4Pack
(DSMReportCurrents4* InDSMReportCurrents4, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMReportCurrents4 == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps2) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps2 >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps2 >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps2 >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps1) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps1 >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps1 >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMReportCurrents4->amps1 >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMReportCurrents4Unpack
 ******************************************************************************/
void
DSMReportCurrents4Unpack
(DSMReportCurrents4* InDSMReportCurrents4, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMReportCurrents4->amps2 = (uint32_t)*(buffer++);
  InDSMReportCurrents4->amps2 |= ((uint32_t)*(buffer++) << 8);
  InDSMReportCurrents4->amps2 |= ((uint32_t)*(buffer++) << 16);
  InDSMReportCurrents4->amps2 |= ((uint32_t)*(buffer++) << 24);
  InDSMReportCurrents4->amps1 = (uint32_t)*(buffer++);
  InDSMReportCurrents4->amps1 |= ((uint32_t)*(buffer++) << 8);
  InDSMReportCurrents4->amps1 |= ((uint32_t)*(buffer++) << 16);
  InDSMReportCurrents4->amps1 |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : PartNumber1V2Create
 ******************************************************************************/
PartNumber1V2*
PartNumber1V2Create
()
{
  PartNumber1V2*                returnPartNumber1V2;
  returnPartNumber1V2 = (PartNumber1V2*)malloc(sizeof(PartNumber1V2));
  memset(returnPartNumber1V2, 0x00, sizeof(PartNumber1V2));
  return returnPartNumber1V2;
}

/******************************************************************************!
 * Function : PartNumber1V2Destroy
 ******************************************************************************/
void
PartNumber1V2Destroy
(PartNumber1V2* InPartNumber1V2)
{
  if ( InPartNumber1V2 == NULL ) {
    return;
  }
  free(InPartNumber1V2);
}

/******************************************************************************!
 * Function : PartNumber1V2Set
 ******************************************************************************/
void
PartNumber1V2Set
(PartNumber1V2* InPartNumber1V2,
 char* InPartNumber)
{
  int					i;
  if ( InPartNumber1V2 == NULL ) {
    return;
  }
  memset(InPartNumber1V2->PartNumber, 0x00, 9);
  for (i = 0; i < 8 && InPartNumber[i] != 0x00; i++) {
    InPartNumber1V2->PartNumber[i] = InPartNumber[i];
  }
}

/******************************************************************************!
 * Function : PartNumber1V2Clear
 ******************************************************************************/
void
PartNumber1V2Clear
(PartNumber1V2* InPartNumber1V2)
{
  if ( InPartNumber1V2 == NULL ) {
    return;
  }
  memset(InPartNumber1V2, 0x00, sizeof(PartNumber1V2));
}

/******************************************************************************!
 * Function : PartNumber1V2SetFull
 ******************************************************************************/
void
PartNumber1V2SetFull
(PartNumber1V2* InPartNumber1V2,
 char* InPartNumber)
{
  int					i;
  if ( InPartNumber1V2 == NULL ) {
    return;
  }
  memset(InPartNumber1V2->PartNumber, 0x00, 9);
  for (i = 0; i < 8 && InPartNumber[i] != 0x00; i++) {
    InPartNumber1V2->PartNumber[i] = InPartNumber[i];
  }
}

/******************************************************************************!
 * Function : PartNumber1V2Pack
 ******************************************************************************/
void
PartNumber1V2Pack
(PartNumber1V2* InPartNumber1V2, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InPartNumber1V2 == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  memcpy(buffer, InPartNumber1V2->PartNumber, 8);
  length += 8;
  *InLength = length;
}

/******************************************************************************!
 * Function : PartNumber1V2Unpack
 ******************************************************************************/
void
PartNumber1V2Unpack
(PartNumber1V2* InPartNumber1V2, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  memcpy(InPartNumber1V2->PartNumber, buffer, 8);
  InPartNumber1V2->PartNumber[8] = 0x00;
  buffer += 8;
}

/******************************************************************************!
 * Function : BootCodeVersionCreate
 ******************************************************************************/
BootCodeVersion*
BootCodeVersionCreate
()
{
  BootCodeVersion*              returnBootCodeVersion;
  returnBootCodeVersion = (BootCodeVersion*)malloc(sizeof(BootCodeVersion));
  memset(returnBootCodeVersion, 0x00, sizeof(BootCodeVersion));
  return returnBootCodeVersion;
}

/******************************************************************************!
 * Function : BootCodeVersionDestroy
 ******************************************************************************/
void
BootCodeVersionDestroy
(BootCodeVersion* InBootCodeVersion)
{
  if ( InBootCodeVersion == NULL ) {
    return;
  }
  free(InBootCodeVersion);
}

/******************************************************************************!
 * Function : BootCodeVersionSet
 ******************************************************************************/
void
BootCodeVersionSet
(BootCodeVersion* InBootCodeVersion,
 uint8_t InBootMajor,
 uint8_t InBootMinor,
 uint8_t InBootBugFix,
 uint8_t InBootBuild)
{
  if ( InBootCodeVersion == NULL ) {
    return;
  }
  InBootCodeVersion->bootMajor  = InBootMajor;
  InBootCodeVersion->bootMinor  = InBootMinor;
  InBootCodeVersion->bootBugFix = InBootBugFix;
  InBootCodeVersion->bootBuild  = InBootBuild;
}

/******************************************************************************!
 * Function : BootCodeVersionClear
 ******************************************************************************/
void
BootCodeVersionClear
(BootCodeVersion* InBootCodeVersion)
{
  if ( InBootCodeVersion == NULL ) {
    return;
  }
  memset(InBootCodeVersion, 0x00, sizeof(BootCodeVersion));
}

/******************************************************************************!
 * Function : BootCodeVersionSetFull
 ******************************************************************************/
void
BootCodeVersionSetFull
(BootCodeVersion* InBootCodeVersion,
 uint8_t InBootMajor,
 uint8_t InBootMinor,
 uint8_t InBootBugFix,
 uint8_t InBootBuild)
{
  if ( InBootCodeVersion == NULL ) {
    return;
  }
  InBootCodeVersion->bootMajor  = InBootMajor;
  InBootCodeVersion->bootMinor  = InBootMinor;
  InBootCodeVersion->bootBugFix = InBootBugFix;
  InBootCodeVersion->bootBuild  = InBootBuild;
}

/******************************************************************************!
 * Function : BootCodeVersionPack
 ******************************************************************************/
void
BootCodeVersionPack
(BootCodeVersion* InBootCodeVersion, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InBootCodeVersion == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)InBootCodeVersion->bootBuild;
  length++;
  *(buffer++) = (unsigned char)InBootCodeVersion->bootBugFix;
  length++;
  *(buffer++) = (unsigned char)InBootCodeVersion->bootMinor;
  length++;
  *(buffer++) = (unsigned char)InBootCodeVersion->bootMajor;
  length++;
  *InLength = length;
}

/******************************************************************************!
 * Function : BootCodeVersionUnpack
 ******************************************************************************/
void
BootCodeVersionUnpack
(BootCodeVersion* InBootCodeVersion, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InBootCodeVersion->bootBuild = (uint8_t)*(buffer++);
  InBootCodeVersion->bootBugFix = (uint8_t)*(buffer++);
  InBootCodeVersion->bootMinor = (uint8_t)*(buffer++);
  InBootCodeVersion->bootMajor = (uint8_t)*(buffer++);
}

/******************************************************************************!
 * Function : PartNumber2V2Create
 ******************************************************************************/
PartNumber2V2*
PartNumber2V2Create
()
{
  PartNumber2V2*                returnPartNumber2V2;
  returnPartNumber2V2 = (PartNumber2V2*)malloc(sizeof(PartNumber2V2));
  memset(returnPartNumber2V2, 0x00, sizeof(PartNumber2V2));
  return returnPartNumber2V2;
}

/******************************************************************************!
 * Function : PartNumber2V2Destroy
 ******************************************************************************/
void
PartNumber2V2Destroy
(PartNumber2V2* InPartNumber2V2)
{
  if ( InPartNumber2V2 == NULL ) {
    return;
  }
  free(InPartNumber2V2);
}

/******************************************************************************!
 * Function : PartNumber2V2Set
 ******************************************************************************/
void
PartNumber2V2Set
(PartNumber2V2* InPartNumber2V2,
 char* InPartNumber)
{
  int					i;
  if ( InPartNumber2V2 == NULL ) {
    return;
  }
  memset(InPartNumber2V2->PartNumber, 0x00, 9);
  for (i = 0; i < 8 && InPartNumber[i] != 0x00; i++) {
    InPartNumber2V2->PartNumber[i] = InPartNumber[i];
  }
}

/******************************************************************************!
 * Function : PartNumber2V2Clear
 ******************************************************************************/
void
PartNumber2V2Clear
(PartNumber2V2* InPartNumber2V2)
{
  if ( InPartNumber2V2 == NULL ) {
    return;
  }
  memset(InPartNumber2V2, 0x00, sizeof(PartNumber2V2));
}

/******************************************************************************!
 * Function : PartNumber2V2SetFull
 ******************************************************************************/
void
PartNumber2V2SetFull
(PartNumber2V2* InPartNumber2V2,
 char* InPartNumber)
{
  int					i;
  if ( InPartNumber2V2 == NULL ) {
    return;
  }
  memset(InPartNumber2V2->PartNumber, 0x00, 9);
  for (i = 0; i < 8 && InPartNumber[i] != 0x00; i++) {
    InPartNumber2V2->PartNumber[i] = InPartNumber[i];
  }
}

/******************************************************************************!
 * Function : PartNumber2V2Pack
 ******************************************************************************/
void
PartNumber2V2Pack
(PartNumber2V2* InPartNumber2V2, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InPartNumber2V2 == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  memcpy(buffer, InPartNumber2V2->PartNumber, 8);
  length += 8;
  *InLength = length;
}

/******************************************************************************!
 * Function : PartNumber2V2Unpack
 ******************************************************************************/
void
PartNumber2V2Unpack
(PartNumber2V2* InPartNumber2V2, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  memcpy(InPartNumber2V2->PartNumber, buffer, 8);
  InPartNumber2V2->PartNumber[8] = 0x00;
  buffer += 8;
}

/******************************************************************************!
 * Function : DSMStatus1Create
 ******************************************************************************/
DSMStatus1*
DSMStatus1Create
()
{
  DSMStatus1*                   returnDSMStatus1;
  returnDSMStatus1 = (DSMStatus1*)malloc(sizeof(DSMStatus1));
  memset(returnDSMStatus1, 0x00, sizeof(DSMStatus1));
  return returnDSMStatus1;
}

/******************************************************************************!
 * Function : DSMStatus1Destroy
 ******************************************************************************/
void
DSMStatus1Destroy
(DSMStatus1* InDSMStatus1)
{
  if ( InDSMStatus1 == NULL ) {
    return;
  }
  free(InDSMStatus1);
}

/******************************************************************************!
 * Function : DSMStatus1Set
 ******************************************************************************/
void
DSMStatus1Set
(DSMStatus1* InDSMStatus1,
 uint32_t InID2Bits)
{
  if ( InDSMStatus1 == NULL ) {
    return;
  }
  InDSMStatus1->ID2Bits = InID2Bits;
}

/******************************************************************************!
 * Function : DSMStatus1Clear
 ******************************************************************************/
void
DSMStatus1Clear
(DSMStatus1* InDSMStatus1)
{
  if ( InDSMStatus1 == NULL ) {
    return;
  }
  memset(InDSMStatus1, 0x00, sizeof(DSMStatus1));
}

/******************************************************************************!
 * Function : DSMStatus1SetFull
 ******************************************************************************/
void
DSMStatus1SetFull
(DSMStatus1* InDSMStatus1,
 uint32_t InID2Bit24,
 uint32_t InID2Bit23,
 uint32_t InID2Bit22,
 uint32_t InID2Bit21,
 uint32_t InID2Bit20,
 uint32_t InID2Bit19,
 uint32_t InID2Bit18,
 uint32_t InID2Bit17,
 uint32_t InID2Bit16,
 uint32_t InID2Bit15,
 uint32_t InID2Bit14,
 uint32_t InID2Bit13,
 uint32_t InID2Bit12,
 uint32_t InID2Bit11,
 uint32_t InID2Bit10,
 uint32_t InID2Bit09,
 uint32_t InID2Bit08,
 uint32_t InID2Bit07,
 uint32_t InID2Bit06,
 uint32_t InID2Bit05,
 uint32_t InID2Bit04,
 uint32_t InID2Bit03,
 uint32_t InID2Bit02,
 uint32_t InID2Bit01)
{
  if ( InDSMStatus1 == NULL ) {
    return;
  }
  InDSMStatus1->ID2Bit24 = InID2Bit24;
  InDSMStatus1->ID2Bit23 = InID2Bit23;
  InDSMStatus1->ID2Bit22 = InID2Bit22;
  InDSMStatus1->ID2Bit21 = InID2Bit21;
  InDSMStatus1->ID2Bit20 = InID2Bit20;
  InDSMStatus1->ID2Bit19 = InID2Bit19;
  InDSMStatus1->ID2Bit18 = InID2Bit18;
  InDSMStatus1->ID2Bit17 = InID2Bit17;
  InDSMStatus1->ID2Bit16 = InID2Bit16;
  InDSMStatus1->ID2Bit15 = InID2Bit15;
  InDSMStatus1->ID2Bit14 = InID2Bit14;
  InDSMStatus1->ID2Bit13 = InID2Bit13;
  InDSMStatus1->ID2Bit12 = InID2Bit12;
  InDSMStatus1->ID2Bit11 = InID2Bit11;
  InDSMStatus1->ID2Bit10 = InID2Bit10;
  InDSMStatus1->ID2Bit09 = InID2Bit09;
  InDSMStatus1->ID2Bit08 = InID2Bit08;
  InDSMStatus1->ID2Bit07 = InID2Bit07;
  InDSMStatus1->ID2Bit06 = InID2Bit06;
  InDSMStatus1->ID2Bit05 = InID2Bit05;
  InDSMStatus1->ID2Bit04 = InID2Bit04;
  InDSMStatus1->ID2Bit03 = InID2Bit03;
  InDSMStatus1->ID2Bit02 = InID2Bit02;
  InDSMStatus1->ID2Bit01 = InID2Bit01;
}

/******************************************************************************!
 * Function : DSMStatus1Pack
 ******************************************************************************/
void
DSMStatus1Pack
(DSMStatus1* InDSMStatus1, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMStatus1 == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMStatus1->ID2Bits) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus1->ID2Bits >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus1->ID2Bits >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus1->ID2Bits >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InDSMStatus1->Empty9) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus1->Empty9 >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus1->Empty9 >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus1->Empty9 >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMStatus1Unpack
 ******************************************************************************/
void
DSMStatus1Unpack
(DSMStatus1* InDSMStatus1, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMStatus1->ID2Bits = (uint32_t)*(buffer++);
  InDSMStatus1->ID2Bits |= ((uint32_t)*(buffer++) << 8);
  InDSMStatus1->ID2Bits |= ((uint32_t)*(buffer++) << 16);
  InDSMStatus1->ID2Bits |= ((uint32_t)*(buffer++) << 24);
  InDSMStatus1->Empty9 = (uint32_t)*(buffer++);
  InDSMStatus1->Empty9 |= ((uint32_t)*(buffer++) << 8);
  InDSMStatus1->Empty9 |= ((uint32_t)*(buffer++) << 16);
  InDSMStatus1->Empty9 |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : DSMStatus2Create
 ******************************************************************************/
DSMStatus2*
DSMStatus2Create
()
{
  DSMStatus2*                   returnDSMStatus2;
  returnDSMStatus2 = (DSMStatus2*)malloc(sizeof(DSMStatus2));
  memset(returnDSMStatus2, 0x00, sizeof(DSMStatus2));
  return returnDSMStatus2;
}

/******************************************************************************!
 * Function : DSMStatus2Destroy
 ******************************************************************************/
void
DSMStatus2Destroy
(DSMStatus2* InDSMStatus2)
{
  if ( InDSMStatus2 == NULL ) {
    return;
  }
  free(InDSMStatus2);
}

/******************************************************************************!
 * Function : DSMStatus2Set
 ******************************************************************************/
void
DSMStatus2Set
(DSMStatus2* InDSMStatus2,
 uint32_t InID1Bits,
 uint32_t InID0Bits)
{
  if ( InDSMStatus2 == NULL ) {
    return;
  }
  InDSMStatus2->ID1Bits = InID1Bits;
  InDSMStatus2->ID0Bits = InID0Bits;
}

/******************************************************************************!
 * Function : DSMStatus2Clear
 ******************************************************************************/
void
DSMStatus2Clear
(DSMStatus2* InDSMStatus2)
{
  if ( InDSMStatus2 == NULL ) {
    return;
  }
  memset(InDSMStatus2, 0x00, sizeof(DSMStatus2));
}

/******************************************************************************!
 * Function : DSMStatus2SetFull
 ******************************************************************************/
void
DSMStatus2SetFull
(DSMStatus2* InDSMStatus2,
 uint32_t InID1Bit24,
 uint32_t InID1Bit23,
 uint32_t InID1Bit22,
 uint32_t InID1Bit21,
 uint32_t InID1Bit20,
 uint32_t InID1Bit19,
 uint32_t InID1Bit18,
 uint32_t InID1Bit17,
 uint32_t InID1Bit16,
 uint32_t InID1Bit15,
 uint32_t InID1Bit14,
 uint32_t InID1Bit13,
 uint32_t InID1Bit12,
 uint32_t InID1Bit11,
 uint32_t InID1Bit10,
 uint32_t InID1Bit09,
 uint32_t InID1Bit08,
 uint32_t InID1Bit07,
 uint32_t InID1Bit06,
 uint32_t InID1Bit05,
 uint32_t InID1Bit04,
 uint32_t InID1Bit03,
 uint32_t InID1Bit02,
 uint32_t InID1Bit01,
 uint32_t InID0Bit24,
 uint32_t InID0Bit23,
 uint32_t InID0Bit22,
 uint32_t InID0Bit21,
 uint32_t InID0Bit20,
 uint32_t InID0Bit19,
 uint32_t InID0Bit18,
 uint32_t InID0Bit17,
 uint32_t InID0Bit16,
 uint32_t InID0Bit15,
 uint32_t InID0Bit14,
 uint32_t InID0Bit13,
 uint32_t InID0Bit12,
 uint32_t InID0Bit11,
 uint32_t InID0Bit10,
 uint32_t InID0Bit09,
 uint32_t InID0Bit08,
 uint32_t InID0Bit07,
 uint32_t InID0Bit06,
 uint32_t InID0Bit05,
 uint32_t InID0Bit04,
 uint32_t InID0Bit03,
 uint32_t InID0Bit02,
 uint32_t InID0Bit01)
{
  if ( InDSMStatus2 == NULL ) {
    return;
  }
  InDSMStatus2->ID1Bit24 = InID1Bit24;
  InDSMStatus2->ID1Bit23 = InID1Bit23;
  InDSMStatus2->ID1Bit22 = InID1Bit22;
  InDSMStatus2->ID1Bit21 = InID1Bit21;
  InDSMStatus2->ID1Bit20 = InID1Bit20;
  InDSMStatus2->ID1Bit19 = InID1Bit19;
  InDSMStatus2->ID1Bit18 = InID1Bit18;
  InDSMStatus2->ID1Bit17 = InID1Bit17;
  InDSMStatus2->ID1Bit16 = InID1Bit16;
  InDSMStatus2->ID1Bit15 = InID1Bit15;
  InDSMStatus2->ID1Bit14 = InID1Bit14;
  InDSMStatus2->ID1Bit13 = InID1Bit13;
  InDSMStatus2->ID1Bit12 = InID1Bit12;
  InDSMStatus2->ID1Bit11 = InID1Bit11;
  InDSMStatus2->ID1Bit10 = InID1Bit10;
  InDSMStatus2->ID1Bit09 = InID1Bit09;
  InDSMStatus2->ID1Bit08 = InID1Bit08;
  InDSMStatus2->ID1Bit07 = InID1Bit07;
  InDSMStatus2->ID1Bit06 = InID1Bit06;
  InDSMStatus2->ID1Bit05 = InID1Bit05;
  InDSMStatus2->ID1Bit04 = InID1Bit04;
  InDSMStatus2->ID1Bit03 = InID1Bit03;
  InDSMStatus2->ID1Bit02 = InID1Bit02;
  InDSMStatus2->ID1Bit01 = InID1Bit01;
  InDSMStatus2->ID0Bit24 = InID0Bit24;
  InDSMStatus2->ID0Bit23 = InID0Bit23;
  InDSMStatus2->ID0Bit22 = InID0Bit22;
  InDSMStatus2->ID0Bit21 = InID0Bit21;
  InDSMStatus2->ID0Bit20 = InID0Bit20;
  InDSMStatus2->ID0Bit19 = InID0Bit19;
  InDSMStatus2->ID0Bit18 = InID0Bit18;
  InDSMStatus2->ID0Bit17 = InID0Bit17;
  InDSMStatus2->ID0Bit16 = InID0Bit16;
  InDSMStatus2->ID0Bit15 = InID0Bit15;
  InDSMStatus2->ID0Bit14 = InID0Bit14;
  InDSMStatus2->ID0Bit13 = InID0Bit13;
  InDSMStatus2->ID0Bit12 = InID0Bit12;
  InDSMStatus2->ID0Bit11 = InID0Bit11;
  InDSMStatus2->ID0Bit10 = InID0Bit10;
  InDSMStatus2->ID0Bit09 = InID0Bit09;
  InDSMStatus2->ID0Bit08 = InID0Bit08;
  InDSMStatus2->ID0Bit07 = InID0Bit07;
  InDSMStatus2->ID0Bit06 = InID0Bit06;
  InDSMStatus2->ID0Bit05 = InID0Bit05;
  InDSMStatus2->ID0Bit04 = InID0Bit04;
  InDSMStatus2->ID0Bit03 = InID0Bit03;
  InDSMStatus2->ID0Bit02 = InID0Bit02;
  InDSMStatus2->ID0Bit01 = InID0Bit01;
}

/******************************************************************************!
 * Function : DSMStatus2Pack
 ******************************************************************************/
void
DSMStatus2Pack
(DSMStatus2* InDSMStatus2, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMStatus2 == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMStatus2->ID0Bits) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus2->ID0Bits >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus2->ID0Bits >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus2->ID0Bits >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InDSMStatus2->ID1Bits) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus2->ID1Bits >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus2->ID1Bits >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMStatus2->ID1Bits >> 24) & 0xFF;
  length += 4;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMStatus2Unpack
 ******************************************************************************/
void
DSMStatus2Unpack
(DSMStatus2* InDSMStatus2, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMStatus2->ID0Bits = (uint32_t)*(buffer++);
  InDSMStatus2->ID0Bits |= ((uint32_t)*(buffer++) << 8);
  InDSMStatus2->ID0Bits |= ((uint32_t)*(buffer++) << 16);
  InDSMStatus2->ID0Bits |= ((uint32_t)*(buffer++) << 24);
  InDSMStatus2->ID1Bits = (uint32_t)*(buffer++);
  InDSMStatus2->ID1Bits |= ((uint32_t)*(buffer++) << 8);
  InDSMStatus2->ID1Bits |= ((uint32_t)*(buffer++) << 16);
  InDSMStatus2->ID1Bits |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : RectifierAlarmStatusCreate
 ******************************************************************************/
RectifierAlarmStatus*
RectifierAlarmStatusCreate
()
{
  RectifierAlarmStatus*         returnRectifierAlarmStatus;
  returnRectifierAlarmStatus = (RectifierAlarmStatus*)malloc(sizeof(RectifierAlarmStatus));
  memset(returnRectifierAlarmStatus, 0x00, sizeof(RectifierAlarmStatus));
  return returnRectifierAlarmStatus;
}

/******************************************************************************!
 * Function : RectifierAlarmStatusDestroy
 ******************************************************************************/
void
RectifierAlarmStatusDestroy
(RectifierAlarmStatus* InRectifierAlarmStatus)
{
  if ( InRectifierAlarmStatus == NULL ) {
    return;
  }
  free(InRectifierAlarmStatus);
}

/******************************************************************************!
 * Function : RectifierAlarmStatusSet
 ******************************************************************************/
void
RectifierAlarmStatusSet
(RectifierAlarmStatus* InRectifierAlarmStatus,
 uint32_t InAlarms,
 uint16_t InStatus,
 uint16_t InCurrent)
{
  if ( InRectifierAlarmStatus == NULL ) {
    return;
  }
  InRectifierAlarmStatus->Alarms  = InAlarms;
  InRectifierAlarmStatus->Status  = InStatus;
  InRectifierAlarmStatus->current = InCurrent;
}

/******************************************************************************!
 * Function : RectifierAlarmStatusClear
 ******************************************************************************/
void
RectifierAlarmStatusClear
(RectifierAlarmStatus* InRectifierAlarmStatus)
{
  if ( InRectifierAlarmStatus == NULL ) {
    return;
  }
  memset(InRectifierAlarmStatus, 0x00, sizeof(RectifierAlarmStatus));
}

/******************************************************************************!
 * Function : RectifierAlarmStatusSetFull
 ******************************************************************************/
void
RectifierAlarmStatusSetFull
(RectifierAlarmStatus* InRectifierAlarmStatus,
 uint32_t InSNS,
 uint32_t InNDR,
 uint32_t InACL,
 uint32_t InDHV,
 uint32_t InSWO,
 uint32_t InEMS,
 uint32_t InHAT,
 uint32_t InCNV,
 uint32_t InCBO,
 uint32_t InINH,
 uint32_t InFMA,
 uint32_t InRHV,
 uint32_t InNPD,
 uint32_t InPTS,
 uint32_t InACH,
 uint32_t InFMI,
 uint32_t InPHL,
 uint32_t InPFF,
 uint32_t InDTS,
 uint32_t InTCL,
 uint32_t InPHV,
 uint32_t InDCU,
 uint32_t InLSI,
 uint32_t InDNP,
 uint32_t InAAA,
 uint16_t InLSC,
 uint16_t InRLS,
 uint16_t InLIN,
 uint16_t InCPR,
 uint16_t InVIN,
 uint16_t InCurrent)
{
  if ( InRectifierAlarmStatus == NULL ) {
    return;
  }
  InRectifierAlarmStatus->SNS     = InSNS;
  InRectifierAlarmStatus->NDR     = InNDR;
  InRectifierAlarmStatus->ACL     = InACL;
  InRectifierAlarmStatus->DHV     = InDHV;
  InRectifierAlarmStatus->SWO     = InSWO;
  InRectifierAlarmStatus->EMS     = InEMS;
  InRectifierAlarmStatus->HAT     = InHAT;
  InRectifierAlarmStatus->CNV     = InCNV;
  InRectifierAlarmStatus->CBO     = InCBO;
  InRectifierAlarmStatus->INH     = InINH;
  InRectifierAlarmStatus->FMA     = InFMA;
  InRectifierAlarmStatus->RHV     = InRHV;
  InRectifierAlarmStatus->NPD     = InNPD;
  InRectifierAlarmStatus->PTS     = InPTS;
  InRectifierAlarmStatus->ACH     = InACH;
  InRectifierAlarmStatus->FMI     = InFMI;
  InRectifierAlarmStatus->PHL     = InPHL;
  InRectifierAlarmStatus->PFF     = InPFF;
  InRectifierAlarmStatus->DTS     = InDTS;
  InRectifierAlarmStatus->TCL     = InTCL;
  InRectifierAlarmStatus->PHV     = InPHV;
  InRectifierAlarmStatus->DCU     = InDCU;
  InRectifierAlarmStatus->LSI     = InLSI;
  InRectifierAlarmStatus->DNP     = InDNP;
  InRectifierAlarmStatus->AAA     = InAAA;
  InRectifierAlarmStatus->LSC     = InLSC;
  InRectifierAlarmStatus->RLS     = InRLS;
  InRectifierAlarmStatus->LIN     = InLIN;
  InRectifierAlarmStatus->CPR     = InCPR;
  InRectifierAlarmStatus->VIN     = InVIN;
  InRectifierAlarmStatus->current = InCurrent;
}

/******************************************************************************!
 * Function : RectifierAlarmStatusPack
 ******************************************************************************/
void
RectifierAlarmStatusPack
(RectifierAlarmStatus* InRectifierAlarmStatus, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRectifierAlarmStatus == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->Alarms >> 24) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->Alarms >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->Alarms >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->Alarms) & 0xFF;
  length += 4;

  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->Status >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->Status) & 0xFF;
  length += 2;

  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->current >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAlarmStatus->current) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : RectifierAlarmStatusUnpack
 ******************************************************************************/
void
RectifierAlarmStatusUnpack
(RectifierAlarmStatus* InRectifierAlarmStatus, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InRectifierAlarmStatus->current = (uint16_t)*(buffer++);
  InRectifierAlarmStatus->current |= ((uint16_t)*(buffer++) << 8);
  InRectifierAlarmStatus->Status = (uint16_t)*(buffer++);
  InRectifierAlarmStatus->Status |= ((uint16_t)*(buffer++) << 8);
  InRectifierAlarmStatus->Alarms = (uint32_t)*(buffer++);
  InRectifierAlarmStatus->Alarms |= ((uint32_t)*(buffer++) << 8);
  InRectifierAlarmStatus->Alarms |= ((uint32_t)*(buffer++) << 16);
  InRectifierAlarmStatus->Alarms |= ((uint32_t)*(buffer++) << 24);
}

/******************************************************************************!
 * Function : DSMAlarmStatusCreate
 ******************************************************************************/
DSMAlarmStatus*
DSMAlarmStatusCreate
()
{
  DSMAlarmStatus*               returnDSMAlarmStatus;
  returnDSMAlarmStatus = (DSMAlarmStatus*)malloc(sizeof(DSMAlarmStatus));
  memset(returnDSMAlarmStatus, 0x00, sizeof(DSMAlarmStatus));
  return returnDSMAlarmStatus;
}

/******************************************************************************!
 * Function : DSMAlarmStatusDestroy
 ******************************************************************************/
void
DSMAlarmStatusDestroy
(DSMAlarmStatus* InDSMAlarmStatus)
{
  if ( InDSMAlarmStatus == NULL ) {
    return;
  }
  free(InDSMAlarmStatus);
}

/******************************************************************************!
 * Function : DSMAlarmStatusSet
 ******************************************************************************/
void
DSMAlarmStatusSet
(DSMAlarmStatus* InDSMAlarmStatus,
 uint16_t InAlarms,
 uint32_t InFuseBreakerAlarms,
 uint16_t InStatus)
{
  if ( InDSMAlarmStatus == NULL ) {
    return;
  }
  InDSMAlarmStatus->Alarms            = InAlarms;
  InDSMAlarmStatus->FuseBreakerAlarms = InFuseBreakerAlarms;
  InDSMAlarmStatus->Status            = InStatus;
}

/******************************************************************************!
 * Function : DSMAlarmStatusClear
 ******************************************************************************/
void
DSMAlarmStatusClear
(DSMAlarmStatus* InDSMAlarmStatus)
{
  if ( InDSMAlarmStatus == NULL ) {
    return;
  }
  memset(InDSMAlarmStatus, 0x00, sizeof(DSMAlarmStatus));
}

/******************************************************************************!
 * Function : DSMAlarmStatusSetFull
 ******************************************************************************/
void
DSMAlarmStatusSetFull
(DSMAlarmStatus* InDSMAlarmStatus,
 uint16_t InCTL,
 uint16_t InNDR,
 uint16_t InABC,
 uint16_t InSHR,
 uint16_t InABF,
 uint16_t InLVD,
 uint16_t InDCF,
 uint16_t InFBA,
 uint16_t InUFB,
 uint16_t InSOC,
 uint16_t InDNP,
 uint16_t InIOC,
 uint16_t InNRY,
 uint16_t InAAA,
 uint32_t InAlarmBit24,
 uint32_t InAlarmBit23,
 uint32_t InAlarmBit22,
 uint32_t InAlarmBit21,
 uint32_t InAlarmBit20,
 uint32_t InAlarmBit19,
 uint32_t InAlarmBit18,
 uint32_t InAlarmBit17,
 uint32_t InAlarmBit16,
 uint32_t InAlarmBit15,
 uint32_t InAlarmBit14,
 uint32_t InAlarmBit13,
 uint32_t InAlarmBit12,
 uint32_t InAlarmBit11,
 uint32_t InAlarmBit10,
 uint32_t InAlarmBit09,
 uint32_t InAlarmBit08,
 uint32_t InAlarmBit07,
 uint32_t InAlarmBit06,
 uint32_t InAlarmBit05,
 uint32_t InAlarmBit04,
 uint32_t InAlarmBit03,
 uint32_t InAlarmBit02,
 uint32_t InAlarmBit01,
 uint16_t InLLI,
 uint16_t InBLI,
 uint16_t InG50,
 uint16_t InGS1,
 uint16_t InGS2,
 uint16_t InS2L,
 uint16_t InS2B,
 uint16_t InBT1,
 uint16_t InBT2,
 uint16_t InBT3,
 uint16_t InBT4,
 uint16_t InBH1,
 uint16_t InBH2,
 uint16_t InBH3,
 uint16_t InBH4)
{
  if ( InDSMAlarmStatus == NULL ) {
    return;
  }
  InDSMAlarmStatus->CTL        = InCTL;
  InDSMAlarmStatus->NDR        = InNDR;
  InDSMAlarmStatus->ABC        = InABC;
  InDSMAlarmStatus->SHR        = InSHR;
  InDSMAlarmStatus->ABF        = InABF;
  InDSMAlarmStatus->LVD        = InLVD;
  InDSMAlarmStatus->DCF        = InDCF;
  InDSMAlarmStatus->FBA        = InFBA;
  InDSMAlarmStatus->UFB        = InUFB;
  InDSMAlarmStatus->SOC        = InSOC;
  InDSMAlarmStatus->DNP        = InDNP;
  InDSMAlarmStatus->IOC        = InIOC;
  InDSMAlarmStatus->NRY        = InNRY;
  InDSMAlarmStatus->AAA        = InAAA;
  InDSMAlarmStatus->AlarmBit24 = InAlarmBit24;
  InDSMAlarmStatus->AlarmBit23 = InAlarmBit23;
  InDSMAlarmStatus->AlarmBit22 = InAlarmBit22;
  InDSMAlarmStatus->AlarmBit21 = InAlarmBit21;
  InDSMAlarmStatus->AlarmBit20 = InAlarmBit20;
  InDSMAlarmStatus->AlarmBit19 = InAlarmBit19;
  InDSMAlarmStatus->AlarmBit18 = InAlarmBit18;
  InDSMAlarmStatus->AlarmBit17 = InAlarmBit17;
  InDSMAlarmStatus->AlarmBit16 = InAlarmBit16;
  InDSMAlarmStatus->AlarmBit15 = InAlarmBit15;
  InDSMAlarmStatus->AlarmBit14 = InAlarmBit14;
  InDSMAlarmStatus->AlarmBit13 = InAlarmBit13;
  InDSMAlarmStatus->AlarmBit12 = InAlarmBit12;
  InDSMAlarmStatus->AlarmBit11 = InAlarmBit11;
  InDSMAlarmStatus->AlarmBit10 = InAlarmBit10;
  InDSMAlarmStatus->AlarmBit09 = InAlarmBit09;
  InDSMAlarmStatus->AlarmBit08 = InAlarmBit08;
  InDSMAlarmStatus->AlarmBit07 = InAlarmBit07;
  InDSMAlarmStatus->AlarmBit06 = InAlarmBit06;
  InDSMAlarmStatus->AlarmBit05 = InAlarmBit05;
  InDSMAlarmStatus->AlarmBit04 = InAlarmBit04;
  InDSMAlarmStatus->AlarmBit03 = InAlarmBit03;
  InDSMAlarmStatus->AlarmBit02 = InAlarmBit02;
  InDSMAlarmStatus->AlarmBit01 = InAlarmBit01;
  InDSMAlarmStatus->LLI        = InLLI;
  InDSMAlarmStatus->BLI        = InBLI;
  InDSMAlarmStatus->G50        = InG50;
  InDSMAlarmStatus->GS1        = InGS1;
  InDSMAlarmStatus->GS2        = InGS2;
  InDSMAlarmStatus->S2L        = InS2L;
  InDSMAlarmStatus->S2B        = InS2B;
  InDSMAlarmStatus->BT1        = InBT1;
  InDSMAlarmStatus->BT2        = InBT2;
  InDSMAlarmStatus->BT3        = InBT3;
  InDSMAlarmStatus->BT4        = InBT4;
  InDSMAlarmStatus->BH1        = InBH1;
  InDSMAlarmStatus->BH2        = InBH2;
  InDSMAlarmStatus->BH3        = InBH3;
  InDSMAlarmStatus->BH4        = InBH4;
}

/******************************************************************************!
 * Function : DSMAlarmStatusPack
 ******************************************************************************/
void
DSMAlarmStatusPack
(DSMAlarmStatus* InDSMAlarmStatus, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMAlarmStatus == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMAlarmStatus->Status) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAlarmStatus->Status >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InDSMAlarmStatus->FuseBreakerAlarms) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAlarmStatus->FuseBreakerAlarms >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAlarmStatus->FuseBreakerAlarms >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAlarmStatus->FuseBreakerAlarms >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InDSMAlarmStatus->Alarms) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAlarmStatus->Alarms >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMAlarmStatusUnpack
 ******************************************************************************/
void
DSMAlarmStatusUnpack
(DSMAlarmStatus* InDSMAlarmStatus, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMAlarmStatus->Status = (uint16_t)*(buffer++);
  InDSMAlarmStatus->Status |= ((uint16_t)*(buffer++) << 8);
  InDSMAlarmStatus->FuseBreakerAlarms = (uint32_t)*(buffer++);
  InDSMAlarmStatus->FuseBreakerAlarms |= ((uint32_t)*(buffer++) << 8);
  InDSMAlarmStatus->FuseBreakerAlarms |= ((uint32_t)*(buffer++) << 16);
  InDSMAlarmStatus->FuseBreakerAlarms |= ((uint32_t)*(buffer++) << 24);
  InDSMAlarmStatus->Alarms = (uint16_t)*(buffer++);
  InDSMAlarmStatus->Alarms |= ((uint16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : RectifierAddressRequestCreate
 ******************************************************************************/
RectifierAddressRequest*
RectifierAddressRequestCreate
()
{
  RectifierAddressRequest*      returnRectifierAddressRequest;
  returnRectifierAddressRequest = (RectifierAddressRequest*)malloc(sizeof(RectifierAddressRequest));
  memset(returnRectifierAddressRequest, 0x00, sizeof(RectifierAddressRequest));
  return returnRectifierAddressRequest;
}

/******************************************************************************!
 * Function : RectifierAddressRequestDestroy
 ******************************************************************************/
void
RectifierAddressRequestDestroy
(RectifierAddressRequest* InRectifierAddressRequest)
{
  if ( InRectifierAddressRequest == NULL ) {
    return;
  }
  free(InRectifierAddressRequest);
}

/******************************************************************************!
 * Function : RectifierAddressRequestSet
 ******************************************************************************/
void
RectifierAddressRequestSet
(RectifierAddressRequest* InRectifierAddressRequest,
 uint16_t InNodeDescription,
 uint32_t InSerialNumber,
 int16_t InMaxCurrentCapacity)
{
  if ( InRectifierAddressRequest == NULL ) {
    return;
  }
  InRectifierAddressRequest->NodeDescription    = InNodeDescription;
  InRectifierAddressRequest->SerialNumber       = InSerialNumber;
  InRectifierAddressRequest->maxCurrentCapacity = InMaxCurrentCapacity;
}

/******************************************************************************!
 * Function : RectifierAddressRequestClear
 ******************************************************************************/
void
RectifierAddressRequestClear
(RectifierAddressRequest* InRectifierAddressRequest)
{
  if ( InRectifierAddressRequest == NULL ) {
    return;
  }
  memset(InRectifierAddressRequest, 0x00, sizeof(RectifierAddressRequest));
}

/******************************************************************************!
 * Function : RectifierAddressRequestSetFull
 ******************************************************************************/
void
RectifierAddressRequestSetFull
(RectifierAddressRequest* InRectifierAddressRequest,
 uint16_t InRatedCurrent,
 uint16_t InREZ,
 uint32_t InSerialNumber,
 int16_t InMaxCurrentCapacity)
{
  if ( InRectifierAddressRequest == NULL ) {
    return;
  }
  InRectifierAddressRequest->ratedCurrent       = InRatedCurrent;
  InRectifierAddressRequest->REZ                = InREZ;
  InRectifierAddressRequest->SerialNumber       = InSerialNumber;
  InRectifierAddressRequest->maxCurrentCapacity = InMaxCurrentCapacity;
}

/******************************************************************************!
 * Function : RectifierAddressRequestPack
 ******************************************************************************/
void
RectifierAddressRequestPack
(RectifierAddressRequest* InRectifierAddressRequest, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InRectifierAddressRequest == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InRectifierAddressRequest->maxCurrentCapacity) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAddressRequest->maxCurrentCapacity >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InRectifierAddressRequest->SerialNumber) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAddressRequest->SerialNumber >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAddressRequest->SerialNumber >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAddressRequest->SerialNumber >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InRectifierAddressRequest->NodeDescription) & 0xFF;
  *(buffer++) = (unsigned char)(InRectifierAddressRequest->NodeDescription >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : RectifierAddressRequestUnpack
 ******************************************************************************/
void
RectifierAddressRequestUnpack
(RectifierAddressRequest* InRectifierAddressRequest, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InRectifierAddressRequest->maxCurrentCapacity = (int16_t)*(buffer++);
  InRectifierAddressRequest->maxCurrentCapacity |= ((int16_t)*(buffer++) << 8);
  InRectifierAddressRequest->SerialNumber = (uint32_t)*(buffer++);
  InRectifierAddressRequest->SerialNumber |= ((uint32_t)*(buffer++) << 8);
  InRectifierAddressRequest->SerialNumber |= ((uint32_t)*(buffer++) << 16);
  InRectifierAddressRequest->SerialNumber |= ((uint32_t)*(buffer++) << 24);
  InRectifierAddressRequest->NodeDescription = (uint16_t)*(buffer++);
  InRectifierAddressRequest->NodeDescription |= ((uint16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : DSMAddressRequestCreate
 ******************************************************************************/
DSMAddressRequest*
DSMAddressRequestCreate
()
{
  DSMAddressRequest*            returnDSMAddressRequest;
  returnDSMAddressRequest = (DSMAddressRequest*)malloc(sizeof(DSMAddressRequest));
  memset(returnDSMAddressRequest, 0x00, sizeof(DSMAddressRequest));
  return returnDSMAddressRequest;
}

/******************************************************************************!
 * Function : DSMAddressRequestDestroy
 ******************************************************************************/
void
DSMAddressRequestDestroy
(DSMAddressRequest* InDSMAddressRequest)
{
  if ( InDSMAddressRequest == NULL ) {
    return;
  }
  free(InDSMAddressRequest);
}

/******************************************************************************!
 * Function : DSMAddressRequestSet
 ******************************************************************************/
void
DSMAddressRequestSet
(DSMAddressRequest* InDSMAddressRequest,
 uint16_t InNodeDescription,
 uint32_t InSerialNumber)
{
  if ( InDSMAddressRequest == NULL ) {
    return;
  }
  InDSMAddressRequest->NodeDescription = InNodeDescription;
  InDSMAddressRequest->SerialNumber    = InSerialNumber;
}

/******************************************************************************!
 * Function : DSMAddressRequestClear
 ******************************************************************************/
void
DSMAddressRequestClear
(DSMAddressRequest* InDSMAddressRequest)
{
  if ( InDSMAddressRequest == NULL ) {
    return;
  }
  memset(InDSMAddressRequest, 0x00, sizeof(DSMAddressRequest));
}

/******************************************************************************!
 * Function : DSMAddressRequestSetFull
 ******************************************************************************/
void
DSMAddressRequestSetFull
(DSMAddressRequest* InDSMAddressRequest,
 uint16_t InMaxPointsOnBus,
 uint16_t InLLI,
 uint16_t InBLI,
 uint16_t InTOP,
 uint16_t InGS1,
 uint16_t InGS2,
 uint16_t InTBD,
 uint16_t InG50,
 uint16_t InOSP,
 uint16_t InTDB,
 uint16_t InREZ,
 uint32_t InSerialNumber)
{
  if ( InDSMAddressRequest == NULL ) {
    return;
  }
  InDSMAddressRequest->MaxPointsOnBus = InMaxPointsOnBus;
  InDSMAddressRequest->LLI            = InLLI;
  InDSMAddressRequest->BLI            = InBLI;
  InDSMAddressRequest->TOP            = InTOP;
  InDSMAddressRequest->GS1            = InGS1;
  InDSMAddressRequest->GS2            = InGS2;
  InDSMAddressRequest->TBD            = InTBD;
  InDSMAddressRequest->G50            = InG50;
  InDSMAddressRequest->OSP            = InOSP;
  InDSMAddressRequest->TDB            = InTDB;
  InDSMAddressRequest->REZ            = InREZ;
  InDSMAddressRequest->SerialNumber   = InSerialNumber;
}

/******************************************************************************!
 * Function : DSMAddressRequestPack
 ******************************************************************************/
void
DSMAddressRequestPack
(DSMAddressRequest* InDSMAddressRequest, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InDSMAddressRequest == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InDSMAddressRequest->Empty19) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAddressRequest->Empty19 >> 8) & 0xFF;
  length += 2;
  *(buffer++) = (unsigned char)(InDSMAddressRequest->SerialNumber) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAddressRequest->SerialNumber >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAddressRequest->SerialNumber >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAddressRequest->SerialNumber >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InDSMAddressRequest->NodeDescription) & 0xFF;
  *(buffer++) = (unsigned char)(InDSMAddressRequest->NodeDescription >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : DSMAddressRequestUnpack
 ******************************************************************************/
void
DSMAddressRequestUnpack
(DSMAddressRequest* InDSMAddressRequest, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InDSMAddressRequest->Empty19 = (uint16_t)*(buffer++);
  InDSMAddressRequest->Empty19 |= ((uint16_t)*(buffer++) << 8);
  InDSMAddressRequest->SerialNumber = (uint32_t)*(buffer++);
  InDSMAddressRequest->SerialNumber |= ((uint32_t)*(buffer++) << 8);
  InDSMAddressRequest->SerialNumber |= ((uint32_t)*(buffer++) << 16);
  InDSMAddressRequest->SerialNumber |= ((uint32_t)*(buffer++) << 24);
  InDSMAddressRequest->NodeDescription = (uint16_t)*(buffer++);
  InDSMAddressRequest->NodeDescription |= ((uint16_t)*(buffer++) << 8);
}

/******************************************************************************!
 * Function : NodeAddressResponseCreate
 ******************************************************************************/
NodeAddressResponse*
NodeAddressResponseCreate
()
{
  NodeAddressResponse*          returnNodeAddressResponse;
  returnNodeAddressResponse = (NodeAddressResponse*)malloc(sizeof(NodeAddressResponse));
  memset(returnNodeAddressResponse, 0x00, sizeof(NodeAddressResponse));
  return returnNodeAddressResponse;
}

/******************************************************************************!
 * Function : NodeAddressResponseDestroy
 ******************************************************************************/
void
NodeAddressResponseDestroy
(NodeAddressResponse* InNodeAddressResponse)
{
  if ( InNodeAddressResponse == NULL ) {
    return;
  }
  free(InNodeAddressResponse);
}

/******************************************************************************!
 * Function : NodeAddressResponseSet
 ******************************************************************************/
void
NodeAddressResponseSet
(NodeAddressResponse* InNodeAddressResponse,
 uint16_t InOperationAddress,
 uint32_t InSerialNumber)
{
  if ( InNodeAddressResponse == NULL ) {
    return;
  }
  InNodeAddressResponse->OperationAddress = InOperationAddress;
  InNodeAddressResponse->SerialNumber     = InSerialNumber;
}

/******************************************************************************!
 * Function : NodeAddressResponseClear
 ******************************************************************************/
void
NodeAddressResponseClear
(NodeAddressResponse* InNodeAddressResponse)
{
  if ( InNodeAddressResponse == NULL ) {
    return;
  }
  memset(InNodeAddressResponse, 0x00, sizeof(NodeAddressResponse));
}

/******************************************************************************!
 * Function : NodeAddressResponseSetFull
 ******************************************************************************/
void
NodeAddressResponseSetFull
(NodeAddressResponse* InNodeAddressResponse,
 uint16_t InAddress,
 uint16_t InOperation,
 uint32_t InSerialNumber)
{
  if ( InNodeAddressResponse == NULL ) {
    return;
  }
  InNodeAddressResponse->Address      = InAddress;
  InNodeAddressResponse->Operation    = InOperation;
  InNodeAddressResponse->SerialNumber = InSerialNumber;
}

/******************************************************************************!
 * Function : NodeAddressResponsePack
 ******************************************************************************/
void
NodeAddressResponsePack
(NodeAddressResponse* InNodeAddressResponse, unsigned char* InBuffer, uint16_t* InLength)
{
  uint16_t                      length = 0;
  unsigned char*                buffer;

  if ( InNodeAddressResponse == NULL || InBuffer == NULL || InLength == NULL ) {
    return;
  }

  buffer = InBuffer;

  *(buffer++) = (unsigned char)(InNodeAddressResponse->SerialNumber) & 0xFF;
  *(buffer++) = (unsigned char)(InNodeAddressResponse->SerialNumber >> 8) & 0xFF;
  *(buffer++) = (unsigned char)(InNodeAddressResponse->SerialNumber >> 16) & 0xFF;
  *(buffer++) = (unsigned char)(InNodeAddressResponse->SerialNumber >> 24) & 0xFF;
  length += 4;
  *(buffer++) = (unsigned char)(InNodeAddressResponse->OperationAddress) & 0xFF;
  *(buffer++) = (unsigned char)(InNodeAddressResponse->OperationAddress >> 8) & 0xFF;
  length += 2;
  *InLength = length;
}

/******************************************************************************!
 * Function : NodeAddressResponseUnpack
 ******************************************************************************/
void
NodeAddressResponseUnpack
(NodeAddressResponse* InNodeAddressResponse, unsigned char* InBuffer, uint16_t InLength)
{
  unsigned char*                buffer;
  buffer = InBuffer;
  InNodeAddressResponse->SerialNumber = (uint32_t)*(buffer++);
  InNodeAddressResponse->SerialNumber |= ((uint32_t)*(buffer++) << 8);
  InNodeAddressResponse->SerialNumber |= ((uint32_t)*(buffer++) << 16);
  InNodeAddressResponse->SerialNumber |= ((uint32_t)*(buffer++) << 24);
  InNodeAddressResponse->OperationAddress = (uint16_t)*(buffer++);
  InNodeAddressResponse->OperationAddress |= ((uint16_t)*(buffer++) << 8);
}

