/*****************************************************************************
 * FILE NAME    : ESNACANInterface.c
 * DATE         : January 17 2019
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
#include <ascii.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ESNACANInterface.h"
#include "CANInterface.h"
#include "CANWriteThread.h"
#include "ESNACANPacket.h"
#include "NodeDataStore.h"
#include "CLIThread.h"
#include "BayType.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
uint16_t
ESNACANInterfaceNextRectAddress = 2;

uint16_t
ESNACANInterfaceNextDSMAddress = 15;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void
ESNACANMessageClear
(ESNACANMessage* InMessage);

uint32_t
ESNASwapByteOrderDword
(uint32_t InU32);

/*****************************************************************************!
 * Function : ESNACANSendRectifierNodeAddressRequest
 * Purpose  : Send an address request to the BCM
 *****************************************************************************/
void
ESNACANSendRectifierNodeAddressRequest
(
 uint8_t                                InUnitNumber,
 uint16_t                               InRatedOutputCurrent,
 uint16_t                               InMaxCurrentCapacity,
 uint32_t                               InSerialNumber
)
{
#ifdef HAVE_BCM
  ESNACANMessage                      message;
  NodeAddressArbitrationField         arb;
  uint16_t                            IDLen;

  NodeAddressArbitrationFieldClear(&arb);
  NodeAddressArbitrationFieldSetFull(&arb, InUnitNumber, 0, NODE_TYPE_RECT, 0,
                                     ESNA_CAN_ADDRESS_PRIORITY, 1);
  NodeAddressArbitrationFieldPack(&arb, (unsigned char*)&(message.id.dword),
                                  &IDLen);

  message.data.midWord1 = be16toh(InRatedOutputCurrent);
  message.data.midDWord =  be32toh(InSerialNumber);
  message.data.midWord2 = be16toh(InMaxCurrentCapacity);
  message.dataLength = 8;
  CANWriteSend(&message);
#else
  NodeType*                           node;

  node = NodeDataStoreFindExisting(NODE_TYPE_RECT, InUnitNumber);
  if ( NULL == node ) {
    return;
  }
  node->ESNACANAddress = ESNACANInterfaceNextRectAddress++;
  node->startUpState = NodeStartUpWaitingAddress;
#endif
}

/*****************************************************************************!
 * Function : ESNACANSendDSMNodeAddressRequest
 * Purpose  : Send an address request to the BCM
 *****************************************************************************/
void
ESNACANSendDSMNodeAddressRequest
(
 uint8_t                                InUnitNumber,
 uint16_t                               InMaxPointsOnBus,
 uint32_t                               InSerialNumber
)
{
#ifdef HAVE_BCM
  ESNACANMessage                      message;
  NodeAddressArbitrationField         arb;

  uint16_t                            IDLen = 0;
  uint16_t                            dsmID;

  NodeAddressArbitrationFieldClear(&arb);
  NodeAddressArbitrationFieldSetFull(&arb, InUnitNumber, 0,
                                     NODE_TYPE_DSM, 0,
                                     ESNA_CAN_ADDRESS_PRIORITY, 1);

  NodeAddressArbitrationFieldPack(&arb, (unsigned char*)&(message.id.dword),
                                  &IDLen);

  dsmID = 0x0800 | InMaxPointsOnBus; 
  message.data.midWord1 = be16toh(dsmID);
  message.data.midDWord =  be32toh(InSerialNumber);
  message.data.midWord2 = 0;
  message.dataLength = 6;
  CANWriteSend(&message);
#else
  NodeType*                           node;

  node = NodeDataStoreFindExisting(NODE_TYPE_DSM, InUnitNumber);
  if ( NULL == node ) {
    return;
  }
  node->ESNACANAddress = ESNACANInterfaceNextDSMAddress++;
  node->startUpState = NodeStartUpSendInfo;
#endif

}

/*****************************************************************************!
 * Function : ESNACANSendPartNumber1
 *****************************************************************************/
void
ESNACANSendPartNumber1
(
 uint16_t                               InESNAAddress,
 uint8_t*                               InPartNumber1
)
{
  ESNACANMessage                        message;
  ArbitrationField                      arb;
  PartNumber1                           part1;
  uint16_t                              IDLen, dataLen;

  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_NODE_RES_PART_NUMBER1, InESNAAddress,
                          ESNA_CAN_BCM_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&(message.id.dword), &IDLen);

  PartNumber1SetFull(&part1, (char*)InPartNumber1);
  PartNumber1Pack(&part1, (unsigned char*)&(message.data.qword), &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANSendPartNumber2
 *****************************************************************************/
void
ESNACANSendPartNumber2
(
 uint16_t                               InESNAAddress,
 uint8_t*                               InPartNumber2
)
{
  ESNACANMessage                        message;
  ArbitrationField                      arb;
  PartNumber2V2                         part2;
  uint16_t                              IDLen, dataLen;

  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_NODE_RES_PART_NUMBER2, InESNAAddress,
                          ESNA_CAN_BCM_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&(message.id.dword), &IDLen);

  PartNumber2V2SetFull(&part2, (char*)InPartNumber2);
  PartNumber2V2Pack(&part2, (unsigned char*)&(message.data.qword), &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANSendCustomerText
 *****************************************************************************/
void
ESNACANSendCustomerText
(
 uint16_t                               InESNAAddress,
 uint8_t*                               InCustomerText
)
{
  ESNACANMessage                      message;
  int                                 k, j, i;
  uint8_t                             text[NODE_CUSTOMER_TEXT_SIZE_TOTAL];
  uint8_t                             command;

  command = ESNA_NODE_REQ_CUSTOMER_TEXT;
  memset(text, NODE_CUSTOMER_TEXT_SIZE_TOTAL, ' ');
  memcpy(text, InCustomerText, strlen((string)InCustomerText));

  k = 7;
  j = 0;

  // Break up the customer text into 4 separate messages
  for (i = 0; i < NODE_CUSTOMER_TEXT_SIZE_TOTAL; i++) {
    message.data.byte[j] = text[k];
    if ( (i + 1) % 8 == 0  ) {
      ESNACANMessageClear(&message);
      message.id.NodeRequest.nodeAddress  = InESNAAddress;
      message.id.NodeRequest.command      = command;
      message.id.NodeRequest.type         = ESNA_CAN_TOOL_PRIORITY;
      message.id.NodeRequest.source       = 1;
      message.dataLength = 8;
      CANWriteSend(&message);
      k = i + 8;
      j = 0;
      command++;
    } else {
      k--;
      j++;
    }
  }
}

/*****************************************************************************!
 * Function : ESNACANSendRectifierRatedInputVoltage
 *****************************************************************************/
void
ESNACANSendRectifierRatedInputVoltage
(
 uint16_t                               InESNAAddress,
 uint16_t                               InVoltage,
 uint16_t                               InVoltageType
)
{
  ArbitrationField                      arb;
  RectifierRatedInputVolt               ratedVolt;
  uint16_t                              IDLen, dataLen;
  ESNACANMessage                        message;

  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_RECT_RES_RATED_INPUT_VOLT, InESNAAddress,
                          ESNA_CAN_TOOL_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&(message.id.dword), &IDLen);

  RectifierRatedInputVoltSetFull(&ratedVolt, be16toh(InVoltage), (char*)&InVoltageType);
  RectifierRatedInputVoltPack(&ratedVolt, (unsigned char*)&(message.data.qword),
                              &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANSendRectifierRatedOutputVoltage
 *****************************************************************************/
void
ESNACANSendRectifierRatedOutputVoltage
(
 uint16_t                               InESNAAddress,
 uint16_t                               InVoltage,
 uint16_t                               InVoltageType
)
{
  ESNACANMessage                      message;
  ESNACANMessageClear(&message);
  message.id.NodeRequest.nodeAddress  = InESNAAddress;
  message.id.NodeRequest.command      = ESNA_RECT_RES_RATED_OUTPUT_VOLT;
  message.id.NodeRequest.type         = ESNA_CAN_TOOL_PRIORITY;
  message.id.NodeRequest.source       = 1;
  message.data.word[0] = be16toh(InVoltage);
  message.data.word[1] = InVoltageType;
  message.dataLength = 4;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANSendRectifierRatedOutputCurrent
 *****************************************************************************/
void
ESNACANSendRectifierRatedOutputCurrent
(
 uint16_t                               InESNAAddress,
 uint16_t                               InCurrent,
 uint16_t                               InCurrentType
)
{
  ESNACANMessage                      message;
  ESNACANMessageClear(&message);
  message.id.NodeRequest.nodeAddress  = InESNAAddress;
  message.id.NodeRequest.command      = ESNA_RECT_RES_RATED_OUTPUT_CURRENT;
  message.id.NodeRequest.type         = ESNA_CAN_TOOL_PRIORITY;
  message.id.NodeRequest.source       = 1;
  message.data.word[0] = (InCurrent);
  message.data.word[1] = InCurrentType;
  message.dataLength = 4;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANSendRevisionInfo
 *****************************************************************************/
void
ESNACANSendRevisionInfo
(
 uint16_t                               InESNAAddress,
 NodeRevisionInfo*                      InRevisionInfo
)
{
  ESNACANMessage                        message;
  ArbitrationField                      arb;
  RevisionNumbers                       rev;
  uint16_t                              IDLen, dataLen;

  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_NODE_RES_REVISION_NUMBERS, InESNAAddress,
                          ESNA_CAN_BCM_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&(message.id.dword), &IDLen);

  RevisionNumbersSetFull(&rev,
                         InRevisionInfo->bootBuild,
                         InRevisionInfo->bootBugFix,
                         InRevisionInfo->bootMinor,
                         InRevisionInfo->bootMajor,
                         InRevisionInfo->appBuild,
                         InRevisionInfo->appBugFix,
                         InRevisionInfo->appMinor,
                         InRevisionInfo->appMajor);
                         
  RevisionNumbersPack(&rev, (unsigned char*)&(message.data.qword), &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANSendRectifierAlarmsStatus
 *****************************************************************************/
void
ESNACANSendRectifierAlarmsStatus
(
 uint16_t                               InESNAAddress,
 RectifierType*                         InRectifier
)
{
  ArbitrationField                      arb;
  ESNACANMessage                        message;
  RectifierAlarmStatus                  status;
  uint16_t                              dataLen, IDLen;
  uint16_t                              current;

  current = (32768 * InRectifier->outputCurrent) / InRectifier->ratedOutputCurrent;
  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_RECT_RES_ALARM_STATUS,InESNAAddress,
                          ESNA_CAN_ROUTINE_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&message.id.dword, &IDLen);

  RectifierAlarmStatusSet(&status, InRectifier->alarms.allAlarms,
                          be16toh(InRectifier->status.allStatus), current);
  RectifierAlarmStatusPack(&status, (unsigned char*)&(message.data.qword),
                           &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANSendRectifierNormalParams
 *****************************************************************************/
void
ESNACANSendRectifierNormalParams
(
 uint16_t                               InESNAAddress,
 RectifierType*                         InRectifier
)
{
  ESNACANMessage                      message;
  ArbitrationField                    arb;
  RectifierNormalParameters           parms;
  uint16_t                            IDLen, dataLen;

  ESNACANMessageClear(&message);
  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_RECT_RES_NORMAL_PARAMS, InESNAAddress,
                          ESNA_CAN_ROUTINE_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&(message.id.dword), &IDLen);

  RectifierNormalParametersSet(&parms, InRectifier->parameters.volts * 1000,
                               (32768 * InRectifier->maxCurrentCapacity) / InRectifier->ratedOutputCurrent,
                               InRectifier->parameters.hsvVolts * 1000,
                               InRectifier->parameters.flags.allFlags);
  RectifierNormalParametersPack(&parms, (unsigned char*)&(message.data.qword), &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNACANMessageClear
 * Purpose  : Clear the contents of an ESANCANMessage structure
 *****************************************************************************/
void
ESNACANMessageClear
(
 ESNACANMessage*                InMessage
)
{
  InMessage->id.dword = 0;
  InMessage->data.qword = 0;
  InMessage->dataLength = 0;
}

/******************************************************************************!
 * Function : ESNACANSendDSMUpdate1
 ******************************************************************************/
void
ESNACANSendDSMUpdate1
(NodeType* InNode)
{
  int                           mode;

  if ( InNode->nodeType != NODE_TYPE_DSM ) {
    return;
  }
  if ( InNode->startUpState != NodeStartUpStarted ) {
    return;
  }

  for ( mode = DSM_FIRST_UPDATE_STATE1; mode <= DSM_MAX_UPDATE_STATE1; mode++ ) {
    switch (mode ) {
      case 1 : {
      case 2 : 
      case 3 : 
      case 4 : 
      case 5 : 
      case 6 : 
        ESNACANSendDSMShuntCurrentsUpdate(InNode->ESNACANAddress, &InNode->dsm, 0x27 + mode);
        break;
      }
    }
  }
  InNode->dsm.updateState = mode;
  InNode->dsm.dsmUpdateState = DSM_FIRST_UPDATE_STATE2;
}

/******************************************************************************!
 * Function : ESNACANSendDSMUpdate2
 ******************************************************************************/
void
ESNACANSendDSMUpdate2
(NodeType* InNode)
{
  int                           mode;

  if ( InNode->nodeType != NODE_TYPE_DSM ) {
    return;
  }
  if ( InNode->startUpState != NodeStartUpStarted ) {
    return;
  }

  for ( mode = DSM_FIRST_UPDATE_STATE2; mode <= DSM_MAX_UPDATE_STATE2; mode++ ) {
    switch (mode ) {
      case 7 : {
        ESNACANSendDSMShuntStatusUpdate(InNode->ESNACANAddress, &InNode->dsm);
        break;
      }
      case 8 : {
        ESNACANSendDSMShuntStatusInstallUpdate_2(InNode->ESNACANAddress, &InNode->dsm);
        break;
      }
      case 9 : {
        ESNACANSendDSMShuntStatusInstallUpdate_0_1(InNode->ESNACANAddress, &InNode->dsm);
        break;
      }
      case 10 : {
        ESNACANSendDSMStatusUpdate(InNode->ESNACANAddress, &InNode->dsm);
        break;
      }
    }
  }
  InNode->dsm.dsmUpdateState = DSM_FIRST_UPDATE_STATE1;
  InNode->dsm.updateState = mode;
}

/******************************************************************************!
 * Function : ESNACANSendDSMStatusUpdate
 ******************************************************************************/
void
ESNACANSendDSMStatusUpdate
(uint16_t InNodeAddress, DSMType* InDSM)
{
  ArbitrationField              arb;
  DSMAlarmStatus                status;
  ESNACANMessage                message;
  uint16_t                      IDLen, dataLen;
  uint32_t                      bits, bit;
  int                           i, j;
  int                           bitFieldInc;

  bitFieldInc = InDSM->parentNode->bay->type == BayTypeDistribution ? 1 : 2;
  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_DSM_RES_ALARM_STATUS, InNodeAddress, 
                          ESNA_CAN_ROUTINE_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&message.id.dword, &IDLen);

  bits = 0;
  for (j = i = 0; i < DSM_MAX_SHUNTS; i++, j += bitFieldInc) {
    Shunt*                      shunt = &(InDSM->shunts[i]);
    bit = 0;
    if ( shunt->occupied ) {
      bit = shunt->power == SHUNT_POWER_CLOSED ? 0 : 1;
    }
    bits |= (bit << j);
  }
  DSMAlarmStatusClear(&status);
  status.AAA = bits ? 1 : 0;
  status.FuseBreakerAlarms = htobe32(bits); 
  DSMAlarmStatusPack(&status, (unsigned char*)&message.data.word, &dataLen);
  message.dataLength = 8;
  CANWriteSend(&message);
}

/******************************************************************************!
 * Function : ESNACANSendDSMShuntCurrentsUpdate
 ******************************************************************************/
void
ESNACANSendDSMShuntCurrentsUpdate
(uint16_t InNodeAddress, DSMType* InDSM, uint16_t InCommand)
{
  ArbitrationField              arb;
  
  ESNACANMessage                message;
  uint16_t                      IDLen, dataLen;
  uint16_t                      index;
  DSMReportCurrents2            currents;
  uint16_t                      current1, current2;
  
  index = (InCommand - 0x28) * 2;
  
  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, InCommand, InNodeAddress, 
                          ESNA_CAN_ROUTINE_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&message.id.dword, &IDLen);

  current1 = InDSM->shunts[index+1].power == SHUNT_POWER_CLOSED ? htobe16(InDSM->shunts[index+1].current)  : 0;
  current2 = InDSM->shunts[index].power == SHUNT_POWER_CLOSED ? htobe16(InDSM->shunts[index].current)  : 0;
  DSMReportCurrents2Set(&currents, 
                        0,
                        (current1),
                        0,
                        (current2));
  DSMReportCurrents2Pack(&currents, (unsigned char*)&(message.data.qword), &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/******************************************************************************!
 * Function : ESNACANSendDSMShuntStatusUpdate
 ******************************************************************************/
void
ESNACANSendDSMShuntStatusUpdate
(uint16_t InNodeAddress, DSMType* InDSM)
{
  ArbitrationField              arb;
  ESNACANMessage                message;
  uint16_t                      IDLen, dataLen;
  DSMShuntStatus                status;
  int                           i, j;
  uint32_t                      loBits, hiBits;
  uint32_t                      hibit, lobit;
  int                           bitFieldInc;

  bitFieldInc = InDSM->parentNode->bay->type == BayTypeDistribution ? 1 : 2;
  DSMShuntStatusClear(&status);
  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_DSM_RES_SHUNT_STATUS, InNodeAddress, 
                          ESNA_CAN_ROUTINE_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&message.id.dword, &IDLen);

  
  j = 0; 
  loBits = hiBits = 0;
  for (i = 0; i < DSM_MAX_SHUNTS; i++, j += bitFieldInc) {
    Shunt*                      shunt = &(InDSM->shunts[i]);

    hibit = 0;
    lobit = 0;
    if ( shunt->removed ) {
      hibit = 1;
      lobit = 1;
    } else if ( shunt->occupied ) {
      if ( shunt->current > shunt->maxCurrent ) {
        lobit = 1;
      }
    }
    hiBits |= (hibit << j);
    loBits |= (lobit << j);
  }
  status.StatusHighBits = htobe32(hiBits);
  status.StatusLowBits  = htobe32(loBits);
  DSMShuntStatusPack(&status, (unsigned char*)&(message.data.qword), &dataLen);
  message.dataLength = dataLen;
  CANWriteSend(&message);
}

/******************************************************************************!
 * Function : ESNACANSendDSMShuntStatusInstallUpdate_2
 ******************************************************************************/
void
ESNACANSendDSMShuntStatusInstallUpdate_2
(uint16_t InNodeAddress, DSMType* InDSM)
{
  ArbitrationField              arb;
  ESNACANMessage                message;
  uint16_t                      IDLen;
  DSMStatus1                    IDBits;
  uint32_t                      bitsHigh[] = { 1, 0, 0, 1 };
  uint32_t                      bits;
  int                           i, j;
  bool                          distOnlyBay = InDSM->parentNode->bay->type == BayTypeDistribution;
  int                           bitFieldMultiplier = distOnlyBay ? 1 : 2;
  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_DSM_RES_FUSE_BREAKER_STATUS_2, InNodeAddress, 
                          ESNA_CAN_ROUTINE_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&message.id.dword, &IDLen);
  DSMStatus1Clear(&IDBits);
  bits = 0xff000000; 
  j = 0; 
  for ( i = 0; i < 12; ) {
    if ( InDSM->shunts[i].occupied ) {
      if ( bitsHigh[InDSM->shunts[i].type] ) {
        bits |= 1 << j;
      }
      j += InDSM->shunts[i].contactWidth * bitFieldMultiplier;
      i += InDSM->shunts[i].contactWidth;
    } else {
      bits |= 1 << j;
      j++;
      if ( !distOnlyBay ) {
        bits |= 1 << j;
        j++;
      }
      i++;
    }
  }

  IDBits.ID2Bits = be32toh(bits);
  message.data.dword[0] = IDBits.ID2Bits;
  message.dataLength = 4;
  CANWriteSend(&message);
}

/******************************************************************************!
 * Function : ESNACANSendDSMStatusInstallUpdate_0_1
 ******************************************************************************/
void
ESNACANSendDSMShuntStatusInstallUpdate_0_1
(uint16_t InNodeAddress, DSMType* InDSM)
{
  ArbitrationField              arb;
  ESNACANMessage                message;
  uint16_t                      IDLen;
  DSMStatus2                    IDBits;
  uint32_t                      bits;
  uint32_t                      bitsLow[] = { 1, 1, 1, 1 };
  uint32_t                      bitsMid[] = { 1, 0, 1, 0 };
  int                           i, j;
  bool                          distOnlyBay = InDSM->parentNode->bay->type == BayTypeDistribution;
  int                           bitFieldMultiplier = distOnlyBay ? 1 : 2;

  ArbitrationFieldClear(&arb);
  ArbitrationFieldSetFull(&arb, ESNA_DSM_RES_FUSE_BREAKER_STATUS_0_1, InNodeAddress, 
                          ESNA_CAN_ROUTINE_PRIORITY, 1);
  ArbitrationFieldPack(&arb, (unsigned char*)&message.id.dword, &IDLen);

  DSMStatus2Clear(&IDBits);

  bits = 0xff000000; 
  j = 0; 
  for ( i = 0; i < 12; ) {
    if ( InDSM->shunts[i].occupied ) {
      if ( bitsLow[InDSM->shunts[i].type] ) {
        bits |= 1 << j;
      }
      j += InDSM->shunts[i].contactWidth * bitFieldMultiplier;
      i += InDSM->shunts[i].contactWidth;
    } else {
      bits |= 1 << j;
      j++;
      if ( !distOnlyBay ) {
        bits |= 1 << j;
        j++;
      }
      i++;
    }
  }

  message.data.dword[1] = be32toh(bits);

  bits = 0xff000000; 
  j = 0; 
  for ( i = 0; i < 12; ) {
    if ( InDSM->shunts[i].occupied ) {
      if ( bitsMid[InDSM->shunts[i].type] ) {
        bits |= 1 << j;
      }
      j += InDSM->shunts[i].contactWidth * bitFieldMultiplier;
      i += InDSM->shunts[i].contactWidth;
    } else {
      bits |= 1 << j;
      j++;
      if ( !distOnlyBay ) {
        bits |= 1 << j;
        j++;
      }
      i++;
    }
  }
  message.data.dword[0] = be32toh(bits);
  message.dataLength = 8;
  CANWriteSend(&message);
}

/*****************************************************************************!
 * Function : ESNASwapByteOrderDword
 *****************************************************************************/
uint32_t
ESNASwapByteOrderDword(uint32_t InU32)
{
  uint8_t*                      p;
  uint8_t                       b;
  uint32_t                      bits;

  bits = InU32;
  p = (uint8_t*)&bits;
  b = p[0];
  p[0] = p[2];
  p[2] = b;
  return bits;
}
