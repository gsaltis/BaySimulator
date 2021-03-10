/******************************************************************************
 * FILE NAME    : ESNACANPacket.h
 * DATE         : May 09 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 ******************************************************************************/
#ifndef _esnacanpacket_h_
#define _esnacanpacket_h_

/******************************************************************************!
 * Global Headers
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#pragma pack(1)
/******************************************************************************!
 * Exported Type : ArbitrationField
 ******************************************************************************/
struct _ArbitrationField {
  union {
    uint32_t                    Fields;
    struct {
      uint32_t                  Command:12;
      uint32_t                  NodeAddress:12;
      uint32_t                  Priority:4;
      uint32_t                  Direction:1;
      uint32_t                  Empty1:3;
    };
  };
};
typedef struct _ArbitrationField ArbitrationField;

/******************************************************************************!
 * Exported Type : NodeAddressArbitrationField
 ******************************************************************************/
struct _NodeAddressArbitrationField {
  union {
    uint32_t                    Fields;
    struct {
      uint32_t                  unitNumber:4;
      uint32_t                  shelfNumber:4;
      uint32_t                  NodeType:4;
      uint32_t                  NodeAddress:12;
      uint32_t                  Priority:4;
      uint32_t                  Direction:1;
      uint32_t                  Empty2:3;
    };
  };
};
typedef struct _NodeAddressArbitrationField NodeAddressArbitrationField;

/******************************************************************************!
 * Exported Type : DSMNodeAddressArbitrationField
 ******************************************************************************/
struct _DSMNodeAddressArbitrationField {
  union {
    uint32_t                    Fields;
    struct {
      uint32_t                  Command:12;
      uint32_t                  RatedCurrent:12;
      uint32_t                  Empty3:3;
      uint32_t                  REZ:1;
      uint32_t                  Priority:4;
      uint32_t                  Direction:1;
      uint32_t                  Empty4:3;
    };
  };
};
typedef struct _DSMNodeAddressArbitrationField DSMNodeAddressArbitrationField;

/******************************************************************************!
 * Exported Type : SystemTime
 * Purpose       : Sync to system time (0x00)
 ******************************************************************************/
struct _SystemTime {
  uint32_t                      Time;
};
typedef struct _SystemTime SystemTime;

/******************************************************************************!
 * Exported Type : EStopTime
 * Purpose       : Estop to at specific time (0x01)
 ******************************************************************************/
struct _EStopTime {
  uint32_t                      Time;
};
typedef struct _EStopTime EStopTime;

/******************************************************************************!
 * Exported Type : PartNumber1
 * Purpose       : 1st half of Part Number (0x15)
 ******************************************************************************/
struct _PartNumber1 {
  char                          PartNumber[9];
};
typedef struct _PartNumber1 PartNumber1;

/******************************************************************************!
 * Exported Type : RevisionNumbers
 * Purpose       : Revision Numbers (0x16)
 ******************************************************************************/
struct _RevisionNumbers {
  uint8_t                       AppBuild;
  uint8_t                       AppBugFix;
  uint8_t                       AppMinor;
  uint8_t                       AppMajor;
  uint8_t                       BootBuild;
  uint8_t                       BootBugFix;
  uint8_t                       BootMinor;
  uint8_t                       BootMajor;
};
typedef struct _RevisionNumbers RevisionNumbers;

/******************************************************************************!
 * Exported Type : RectifierRatedInputVolt
 * Purpose       : Rectifier rated input voltage (0x1E)
 ******************************************************************************/
struct _RectifierRatedInputVolt {
  int16_t                       volts;
  char                          voltType[3];
};
typedef struct _RectifierRatedInputVolt RectifierRatedInputVolt;

/******************************************************************************!
 * Exported Type : RectifierRatedOutputVolt
 * Purpose       : Rectifier rated output voltage (0x1F)
 ******************************************************************************/
struct _RectifierRatedOutputVolt {
  int16_t                       volts;
  char                          voltType[3];
};
typedef struct _RectifierRatedOutputVolt RectifierRatedOutputVolt;

/******************************************************************************!
 * Exported Type : RectifierRatedOutputCurrent
 * Purpose       : Rectifier rated output current (0x20)
 ******************************************************************************/
struct _RectifierRatedOutputCurrent {
  int16_t                       current;
  char                          voltType[3];
  int16_t                       maxCurrentCapacity;
};
typedef struct _RectifierRatedOutputCurrent RectifierRatedOutputCurrent;

/******************************************************************************!
 * Exported Type : RectifierDefaultParameters
 * Purpose       : Rectifier default parameters (0x21)
 ******************************************************************************/
struct _RectifierDefaultParameters {
  uint16_t                      VoltageOut;
  uint16_t                      HVSOut;
};
typedef struct _RectifierDefaultParameters RectifierDefaultParameters;

/******************************************************************************!
 * Exported Type : RectifierNormalParameters
 * Purpose       : Rectifier normal parameters (0x22)
 ******************************************************************************/
struct _RectifierNormalParameters {
  uint16_t                      voltsOut;
  uint16_t                      current;
  uint16_t                      hvsVolts;
  uint8_t                       Empty5;
  union {
    uint8_t                     status;
    struct {
      uint8_t                   TCO:1;
      uint8_t                   DLS:1;
      uint8_t                   TEO:1;
      uint8_t                   DCP:1;
      uint8_t                   WAL:1;
      uint8_t                   CTC:1;
      uint8_t                   RDL:1;
      uint8_t                   ACO:1;
    };
  };
};
typedef struct _RectifierNormalParameters RectifierNormalParameters;

/******************************************************************************!
 * Exported Type : DSMShuntStatus
 * Purpose       : DSM Shunt Status (0x23)
 ******************************************************************************/
struct _DSMShuntStatus {
  union {
    uint32_t                    StatusHighBits;
    struct {
      uint32_t                  HiBit24:1;
      uint32_t                  HiBit23:1;
      uint32_t                  HiBit22:1;
      uint32_t                  HiBit21:1;
      uint32_t                  HiBit20:1;
      uint32_t                  HiBit19:1;
      uint32_t                  HiBit18:1;
      uint32_t                  HiBit17:1;
      uint32_t                  HiBit16:1;
      uint32_t                  HiBit15:1;
      uint32_t                  HiBit14:1;
      uint32_t                  HiBit13:1;
      uint32_t                  HiBit12:1;
      uint32_t                  HiBit11:1;
      uint32_t                  HiBit10:1;
      uint32_t                  HiBit09:1;
      uint32_t                  HiBit08:1;
      uint32_t                  HiBit07:1;
      uint32_t                  HiBit06:1;
      uint32_t                  HiBit05:1;
      uint32_t                  HiBit04:1;
      uint32_t                  HiBit03:1;
      uint32_t                  HiBit02:1;
      uint32_t                  HiBit01:1;
      uint32_t                  Empty6:8;
    };
  };
  union {
    uint32_t                    StatusLowBits;
    struct {
      uint32_t                  LoBit24:1;
      uint32_t                  LoBit23:1;
      uint32_t                  LoBit22:1;
      uint32_t                  LoBit21:1;
      uint32_t                  LoBit20:1;
      uint32_t                  LoBit19:1;
      uint32_t                  LoBit18:1;
      uint32_t                  LoBit17:1;
      uint32_t                  LoBit16:1;
      uint32_t                  LoBit15:1;
      uint32_t                  LoBit14:1;
      uint32_t                  LoBit13:1;
      uint32_t                  LoBit12:1;
      uint32_t                  LoBit11:1;
      uint32_t                  LoBit10:1;
      uint32_t                  LoBit09:1;
      uint32_t                  LoBit08:1;
      uint32_t                  LoBit07:1;
      uint32_t                  LoBit06:1;
      uint32_t                  LoBit05:1;
      uint32_t                  LoBit04:1;
      uint32_t                  LoBit03:1;
      uint32_t                  LoBit02:1;
      uint32_t                  LoBit01:1;
      uint32_t                  Empty7:8;
    };
  };
};
typedef struct _DSMShuntStatus DSMShuntStatus;

/******************************************************************************!
 * Exported Type : DSMReportCurrents2
 * Purpose       : DSM Shunt Status (0x28-0x2F)
 ******************************************************************************/
struct _DSMReportCurrents2 {
  uint16_t                      amps4;
  uint16_t                      amps3;
  uint16_t                      amps2;
  uint16_t                      amps1;
};
typedef struct _DSMReportCurrents2 DSMReportCurrents2;

/******************************************************************************!
 * Exported Type : DSMReportCurrents4
 * Purpose       : DSM Shunt Status (0x28-0x2F)
 ******************************************************************************/
struct _DSMReportCurrents4 {
  uint32_t                      amps1;
  uint32_t                      amps2;
};
typedef struct _DSMReportCurrents4 DSMReportCurrents4;

/******************************************************************************!
 * Exported Type : PartNumber1V2
 * Purpose       : 1st half of Part Number - Other Version (0x31)
 ******************************************************************************/
struct _PartNumber1V2 {
  char                          PartNumber[9];
};
typedef struct _PartNumber1V2 PartNumber1V2;

/******************************************************************************!
 * Exported Type : BootCodeVersion
 * Purpose       : Boot Code Version Number (0x32)
 ******************************************************************************/
struct _BootCodeVersion {
  uint8_t                       bootMajor;
  uint8_t                       bootMinor;
  uint8_t                       bootBugFix;
  uint8_t                       bootBuild;
};
typedef struct _BootCodeVersion BootCodeVersion;

/******************************************************************************!
 * Exported Type : PartNumber2V2
 * Purpose       : 2nd half of Part Number - Other Version (0x34)
 ******************************************************************************/
struct _PartNumber2V2 {
  char                          PartNumber[9];
};
typedef struct _PartNumber2V2 PartNumber2V2;

/******************************************************************************!
 * Exported Type : DSMStatus1
 * Purpose       : DSM ID Bits 2 (0x3A)
 ******************************************************************************/
struct _DSMStatus1 {
  uint32_t                      Empty9;
  union {
    uint32_t                    ID2Bits;
    struct {
      uint32_t                  ID2Bit24:1;
      uint32_t                  ID2Bit23:1;
      uint32_t                  ID2Bit22:1;
      uint32_t                  ID2Bit21:1;
      uint32_t                  ID2Bit20:1;
      uint32_t                  ID2Bit19:1;
      uint32_t                  ID2Bit18:1;
      uint32_t                  ID2Bit17:1;
      uint32_t                  ID2Bit16:1;
      uint32_t                  ID2Bit15:1;
      uint32_t                  ID2Bit14:1;
      uint32_t                  ID2Bit13:1;
      uint32_t                  ID2Bit12:1;
      uint32_t                  ID2Bit11:1;
      uint32_t                  ID2Bit10:1;
      uint32_t                  ID2Bit09:1;
      uint32_t                  ID2Bit08:1;
      uint32_t                  ID2Bit07:1;
      uint32_t                  ID2Bit06:1;
      uint32_t                  ID2Bit05:1;
      uint32_t                  ID2Bit04:1;
      uint32_t                  ID2Bit03:1;
      uint32_t                  ID2Bit02:1;
      uint32_t                  ID2Bit01:1;
      uint32_t                  Empty8:8;
    };
  };
};
typedef struct _DSMStatus1 DSMStatus1;

/******************************************************************************!
 * Exported Type : DSMStatus2
 * Purpose       : DSM ID 1 - 0 Bits (0x3B)
 ******************************************************************************/
struct _DSMStatus2 {
  union {
    uint32_t                    ID1Bits;
    struct {
      uint32_t                  ID1Bit24:1;
      uint32_t                  ID1Bit23:1;
      uint32_t                  ID1Bit22:1;
      uint32_t                  ID1Bit21:1;
      uint32_t                  ID1Bit20:1;
      uint32_t                  ID1Bit19:1;
      uint32_t                  ID1Bit18:1;
      uint32_t                  ID1Bit17:1;
      uint32_t                  ID1Bit16:1;
      uint32_t                  ID1Bit15:1;
      uint32_t                  ID1Bit14:1;
      uint32_t                  ID1Bit13:1;
      uint32_t                  ID1Bit12:1;
      uint32_t                  ID1Bit11:1;
      uint32_t                  ID1Bit10:1;
      uint32_t                  ID1Bit09:1;
      uint32_t                  ID1Bit08:1;
      uint32_t                  ID1Bit07:1;
      uint32_t                  ID1Bit06:1;
      uint32_t                  ID1Bit05:1;
      uint32_t                  ID1Bit04:1;
      uint32_t                  ID1Bit03:1;
      uint32_t                  ID1Bit02:1;
      uint32_t                  ID1Bit01:1;
      uint32_t                  Empty10:8;
    };
  };
  union {
    uint32_t                    ID0Bits;
    struct {
      uint32_t                  ID0Bit24:1;
      uint32_t                  ID0Bit23:1;
      uint32_t                  ID0Bit22:1;
      uint32_t                  ID0Bit21:1;
      uint32_t                  ID0Bit20:1;
      uint32_t                  ID0Bit19:1;
      uint32_t                  ID0Bit18:1;
      uint32_t                  ID0Bit17:1;
      uint32_t                  ID0Bit16:1;
      uint32_t                  ID0Bit15:1;
      uint32_t                  ID0Bit14:1;
      uint32_t                  ID0Bit13:1;
      uint32_t                  ID0Bit12:1;
      uint32_t                  ID0Bit11:1;
      uint32_t                  ID0Bit10:1;
      uint32_t                  ID0Bit09:1;
      uint32_t                  ID0Bit08:1;
      uint32_t                  ID0Bit07:1;
      uint32_t                  ID0Bit06:1;
      uint32_t                  ID0Bit05:1;
      uint32_t                  ID0Bit04:1;
      uint32_t                  ID0Bit03:1;
      uint32_t                  ID0Bit02:1;
      uint32_t                  ID0Bit01:1;
      uint32_t                  Empty11:8;
    };
  };
};
typedef struct _DSMStatus2 DSMStatus2;

/******************************************************************************!
 * Exported Type : RectifierAlarmStatus
 * Purpose       : Rectifier Alarm Status (0x41)
 ******************************************************************************/
struct _RectifierAlarmStatus {
  union {
    uint32_t                    Alarms;
    struct {
      uint32_t                  SNS:1;
      uint32_t                  NDR:1;
      uint32_t                  ACL:1;
      uint32_t                  DHV:1;
      uint32_t                  SWO:1;
      uint32_t                  EMS:1;
      uint32_t                  HAT:1;
      uint32_t                  CNV:1;
      uint32_t                  CBO:1;
      uint32_t                  INH:1;
      uint32_t                  FMA:1;
      uint32_t                  RHV:1;
      uint32_t                  NPD:1;
      uint32_t                  PTS:1;
      uint32_t                  ACH:1;
      uint32_t                  FMI:1;
      uint32_t                  PHL:1;
      uint32_t                  PFF:1;
      uint32_t                  DTS:1;
      uint32_t                  TCL:1;
      uint32_t                  PHV:1;
      uint32_t                  DCU:1;
      uint32_t                  LSI:1;
      uint32_t                  DNP:1;
      uint32_t                  Empty12:7;
      uint32_t                  AAA:1;
    };
  };
  union {
    uint16_t                    Status;
    struct {
      uint16_t                  Empty13:1;
      uint16_t                  LSC:1;
      uint16_t                  RLS:1;
      uint16_t                  LIN:1;
      uint16_t                  CPR:1;
      uint16_t                  VIN:1;
      uint16_t                  Empty14:10;
    };
  };
  uint16_t                      current;
};
typedef struct _RectifierAlarmStatus RectifierAlarmStatus;

/******************************************************************************!
 * Exported Type : DSMAlarmStatus
 * Purpose       : DSM Alarm Status (0x42)
 ******************************************************************************/
struct _DSMAlarmStatus {
  union {
    uint16_t                    Alarms;
    struct {
      uint16_t                  CTL:1;
      uint16_t                  NDR:1;
      uint16_t                  ABC:1;
      uint16_t                  SHR:1;
      uint16_t                  ABF:1;
      uint16_t                  LVD:1;
      uint16_t                  DCF:1;
      uint16_t                  FBA:1;
      uint16_t                  UFB:1;
      uint16_t                  SOC:1;
      uint16_t                  DNP:1;
      uint16_t                  IOC:1;
      uint16_t                  Empty15:2;
      uint16_t                  NRY:1;
      uint16_t                  AAA:1;
    };
  };
  union {
    uint32_t                    FuseBreakerAlarms;
    struct {
      uint32_t                  AlarmBit24:1;
      uint32_t                  AlarmBit23:1;
      uint32_t                  AlarmBit22:1;
      uint32_t                  AlarmBit21:1;
      uint32_t                  AlarmBit20:1;
      uint32_t                  AlarmBit19:1;
      uint32_t                  AlarmBit18:1;
      uint32_t                  AlarmBit17:1;
      uint32_t                  AlarmBit16:1;
      uint32_t                  AlarmBit15:1;
      uint32_t                  AlarmBit14:1;
      uint32_t                  AlarmBit13:1;
      uint32_t                  AlarmBit12:1;
      uint32_t                  AlarmBit11:1;
      uint32_t                  AlarmBit10:1;
      uint32_t                  AlarmBit09:1;
      uint32_t                  AlarmBit08:1;
      uint32_t                  AlarmBit07:1;
      uint32_t                  AlarmBit06:1;
      uint32_t                  AlarmBit05:1;
      uint32_t                  AlarmBit04:1;
      uint32_t                  AlarmBit03:1;
      uint32_t                  AlarmBit02:1;
      uint32_t                  AlarmBit01:1;
      uint32_t                  Empty16:8;
    };
  };
  union {
    uint16_t                    Status;
    struct {
      uint16_t                  Empty17:1;
      uint16_t                  LLI:1;
      uint16_t                  BLI:1;
      uint16_t                  G50:1;
      uint16_t                  GS1:1;
      uint16_t                  GS2:1;
      uint16_t                  S2L:1;
      uint16_t                  S2B:1;
      uint16_t                  BT1:1;
      uint16_t                  BT2:1;
      uint16_t                  BT3:1;
      uint16_t                  BT4:1;
      uint16_t                  BH1:1;
      uint16_t                  BH2:1;
      uint16_t                  BH3:1;
      uint16_t                  BH4:1;
    };
  };
};
typedef struct _DSMAlarmStatus DSMAlarmStatus;

/******************************************************************************!
 * Exported Type : RectifierAddressRequest
 * Purpose       : Rectifier Address Request
 ******************************************************************************/
struct _RectifierAddressRequest {
  union {
    uint16_t                    NodeDescription;
    struct {
      uint16_t                  ratedCurrent:12;
      uint16_t                  Empty18:3;
      uint16_t                  REZ:1;
    };
  };
  uint32_t                      SerialNumber;
  int16_t                       maxCurrentCapacity;
};
typedef struct _RectifierAddressRequest RectifierAddressRequest;

/******************************************************************************!
 * Exported Type : DSMAddressRequest
 * Purpose       : DSM Address Request
 ******************************************************************************/
struct _DSMAddressRequest {
  union {
    uint16_t                    NodeDescription;
    struct {
      uint16_t                  MaxPointsOnBus:6;
      uint16_t                  LLI:1;
      uint16_t                  BLI:1;
      uint16_t                  TOP:1;
      uint16_t                  GS1:1;
      uint16_t                  GS2:1;
      uint16_t                  TBD:1;
      uint16_t                  G50:1;
      uint16_t                  OSP:1;
      uint16_t                  TDB:1;
      uint16_t                  REZ:1;
    };
  };
  uint32_t                      SerialNumber;
  uint16_t                      Empty19;
};
typedef struct _DSMAddressRequest DSMAddressRequest;

/******************************************************************************!
 * Exported Type : NodeAddressResponse
 * Purpose       : Node Address Response
 ******************************************************************************/
struct _NodeAddressResponse {
  union {
    uint16_t                    OperationAddress;
    struct {
      uint16_t                  Address:12;
      uint16_t                  Operation:4;
    };
  };
  uint32_t                      SerialNumber;
};
typedef struct _NodeAddressResponse NodeAddressResponse;

/******************************************************************************!
 * Exported Functions
 ******************************************************************************/
//! ArbitrationField API Functions
ArbitrationField*
ArbitrationFieldCreate
();

void
ArbitrationFieldDestroy
(ArbitrationField* InArbitrationField);

void
ArbitrationFieldSet
(ArbitrationField* InPacket, uint32_t InFields);

void
ArbitrationFieldSetFull
(ArbitrationField* InPacket,  uint32_t InCommand, uint32_t InNodeAddress, uint32_t InPriority, uint32_t InDirection);

void
ArbitrationFieldPack
(ArbitrationField* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
ArbitrationFieldClear
(ArbitrationField* InPacket);

void
ArbitrationFieldUnpack
(ArbitrationField* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! NodeAddressArbitrationField API Functions
NodeAddressArbitrationField*
NodeAddressArbitrationFieldCreate
();

void
NodeAddressArbitrationFieldDestroy
(NodeAddressArbitrationField* InNodeAddressArbitrationField);

void
NodeAddressArbitrationFieldSet
(NodeAddressArbitrationField* InPacket, uint32_t InFields);

void
NodeAddressArbitrationFieldSetFull
(NodeAddressArbitrationField* InPacket,  uint32_t InUnitNumber, uint32_t InShelfNumber, uint32_t InNodeType, uint32_t InNodeAddress, uint32_t InPriority, uint32_t InDirection);

void
NodeAddressArbitrationFieldPack
(NodeAddressArbitrationField* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
NodeAddressArbitrationFieldClear
(NodeAddressArbitrationField* InPacket);

void
NodeAddressArbitrationFieldUnpack
(NodeAddressArbitrationField* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMNodeAddressArbitrationField API Functions
DSMNodeAddressArbitrationField*
DSMNodeAddressArbitrationFieldCreate
();

void
DSMNodeAddressArbitrationFieldDestroy
(DSMNodeAddressArbitrationField* InDSMNodeAddressArbitrationField);

void
DSMNodeAddressArbitrationFieldSet
(DSMNodeAddressArbitrationField* InPacket, uint32_t InFields);

void
DSMNodeAddressArbitrationFieldSetFull
(DSMNodeAddressArbitrationField* InPacket,  uint32_t InCommand, uint32_t InRatedCurrent, uint32_t InREZ, uint32_t InPriority, uint32_t InDirection);

void
DSMNodeAddressArbitrationFieldPack
(DSMNodeAddressArbitrationField* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMNodeAddressArbitrationFieldClear
(DSMNodeAddressArbitrationField* InPacket);

void
DSMNodeAddressArbitrationFieldUnpack
(DSMNodeAddressArbitrationField* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! SystemTime API Functions
SystemTime*
SystemTimeCreate
();

void
SystemTimeDestroy
(SystemTime* InSystemTime);

void
SystemTimeSet
(SystemTime* InPacket, uint32_t InTime);

void
SystemTimeSetFull
(SystemTime* InPacket,  uint32_t InTime);

void
SystemTimePack
(SystemTime* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
SystemTimeClear
(SystemTime* InPacket);

void
SystemTimeUnpack
(SystemTime* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! EStopTime API Functions
EStopTime*
EStopTimeCreate
();

void
EStopTimeDestroy
(EStopTime* InEStopTime);

void
EStopTimeSet
(EStopTime* InPacket, uint32_t InTime);

void
EStopTimeSetFull
(EStopTime* InPacket,  uint32_t InTime);

void
EStopTimePack
(EStopTime* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
EStopTimeClear
(EStopTime* InPacket);

void
EStopTimeUnpack
(EStopTime* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! PartNumber1 API Functions
PartNumber1*
PartNumber1Create
();

void
PartNumber1Destroy
(PartNumber1* InPartNumber1);

void
PartNumber1Set
(PartNumber1* InPacket, char* InPartNumber);

void
PartNumber1SetFull
(PartNumber1* InPacket,  char* InPartNumber);

void
PartNumber1Pack
(PartNumber1* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
PartNumber1Clear
(PartNumber1* InPacket);

void
PartNumber1Unpack
(PartNumber1* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RevisionNumbers API Functions
RevisionNumbers*
RevisionNumbersCreate
();

void
RevisionNumbersDestroy
(RevisionNumbers* InRevisionNumbers);

void
RevisionNumbersSet
(RevisionNumbers* InPacket, uint8_t InBootMajor, uint8_t InBootMinor, uint8_t InBootBugFix, uint8_t InBootBuild, uint8_t InAppMajor, uint8_t InAppMinor, uint8_t InAppBugFix, uint8_t InAppBuild);

void
RevisionNumbersSetFull
(RevisionNumbers* InPacket,  uint8_t InBootMajor, uint8_t InBootMinor, uint8_t InBootBugFix, uint8_t InBootBuild, uint8_t InAppMajor, uint8_t InAppMinor, uint8_t InAppBugFix, uint8_t InAppBuild);

void
RevisionNumbersPack
(RevisionNumbers* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RevisionNumbersClear
(RevisionNumbers* InPacket);

void
RevisionNumbersUnpack
(RevisionNumbers* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RectifierRatedInputVolt API Functions
RectifierRatedInputVolt*
RectifierRatedInputVoltCreate
();

void
RectifierRatedInputVoltDestroy
(RectifierRatedInputVolt* InRectifierRatedInputVolt);

void
RectifierRatedInputVoltSet
(RectifierRatedInputVolt* InPacket, int16_t InVolts, char* InVoltType);

void
RectifierRatedInputVoltSetFull
(RectifierRatedInputVolt* InPacket,  int16_t InVolts, char* InVoltType);

void
RectifierRatedInputVoltPack
(RectifierRatedInputVolt* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RectifierRatedInputVoltClear
(RectifierRatedInputVolt* InPacket);

void
RectifierRatedInputVoltUnpack
(RectifierRatedInputVolt* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RectifierRatedOutputVolt API Functions
RectifierRatedOutputVolt*
RectifierRatedOutputVoltCreate
();

void
RectifierRatedOutputVoltDestroy
(RectifierRatedOutputVolt* InRectifierRatedOutputVolt);

void
RectifierRatedOutputVoltSet
(RectifierRatedOutputVolt* InPacket, int16_t InVolts, char* InVoltType);

void
RectifierRatedOutputVoltSetFull
(RectifierRatedOutputVolt* InPacket,  int16_t InVolts, char* InVoltType);

void
RectifierRatedOutputVoltPack
(RectifierRatedOutputVolt* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RectifierRatedOutputVoltClear
(RectifierRatedOutputVolt* InPacket);

void
RectifierRatedOutputVoltUnpack
(RectifierRatedOutputVolt* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RectifierRatedOutputCurrent API Functions
RectifierRatedOutputCurrent*
RectifierRatedOutputCurrentCreate
();

void
RectifierRatedOutputCurrentDestroy
(RectifierRatedOutputCurrent* InRectifierRatedOutputCurrent);

void
RectifierRatedOutputCurrentSet
(RectifierRatedOutputCurrent* InPacket, int16_t InCurrent, char* InVoltType, int16_t InMaxCurrentCapacity);

void
RectifierRatedOutputCurrentSetFull
(RectifierRatedOutputCurrent* InPacket,  int16_t InCurrent, char* InVoltType, int16_t InMaxCurrentCapacity);

void
RectifierRatedOutputCurrentPack
(RectifierRatedOutputCurrent* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RectifierRatedOutputCurrentClear
(RectifierRatedOutputCurrent* InPacket);

void
RectifierRatedOutputCurrentUnpack
(RectifierRatedOutputCurrent* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RectifierDefaultParameters API Functions
RectifierDefaultParameters*
RectifierDefaultParametersCreate
();

void
RectifierDefaultParametersDestroy
(RectifierDefaultParameters* InRectifierDefaultParameters);

void
RectifierDefaultParametersSet
(RectifierDefaultParameters* InPacket, uint16_t InVoltageOut, uint16_t InHVSOut);

void
RectifierDefaultParametersSetFull
(RectifierDefaultParameters* InPacket,  uint16_t InVoltageOut, uint16_t InHVSOut);

void
RectifierDefaultParametersPack
(RectifierDefaultParameters* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RectifierDefaultParametersClear
(RectifierDefaultParameters* InPacket);

void
RectifierDefaultParametersUnpack
(RectifierDefaultParameters* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RectifierNormalParameters API Functions
RectifierNormalParameters*
RectifierNormalParametersCreate
();

void
RectifierNormalParametersDestroy
(RectifierNormalParameters* InRectifierNormalParameters);

void
RectifierNormalParametersSet
(RectifierNormalParameters* InPacket, uint16_t InVoltsOut, uint16_t InCurrent, uint16_t InHvsVolts, uint8_t InStatus);

void
RectifierNormalParametersSetFull
(RectifierNormalParameters* InPacket,  uint16_t InVoltsOut, uint16_t InCurrent, uint16_t InHvsVolts, uint8_t InTCO, uint8_t InDLS, uint8_t InTEO, uint8_t InDCP, uint8_t InWAL, uint8_t InCTC, uint8_t InRDL, uint8_t InACO);

void
RectifierNormalParametersPack
(RectifierNormalParameters* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RectifierNormalParametersClear
(RectifierNormalParameters* InPacket);

void
RectifierNormalParametersUnpack
(RectifierNormalParameters* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMShuntStatus API Functions
DSMShuntStatus*
DSMShuntStatusCreate
();

void
DSMShuntStatusDestroy
(DSMShuntStatus* InDSMShuntStatus);

void
DSMShuntStatusSet
(DSMShuntStatus* InPacket, uint32_t InStatusHighBits, uint32_t InStatusLowBits);

void
DSMShuntStatusSetFull
(DSMShuntStatus* InPacket,  uint32_t InHiBit24, uint32_t InHiBit23, uint32_t InHiBit22, uint32_t InHiBit21, uint32_t InHiBit20, uint32_t InHiBit19, uint32_t InHiBit18, uint32_t InHiBit17, uint32_t InHiBit16, uint32_t InHiBit15, uint32_t InHiBit14, uint32_t InHiBit13, uint32_t InHiBit12, uint32_t InHiBit11, uint32_t InHiBit10, uint32_t InHiBit09, uint32_t InHiBit08, uint32_t InHiBit07, uint32_t InHiBit06, uint32_t InHiBit05, uint32_t InHiBit04, uint32_t InHiBit03, uint32_t InHiBit02, uint32_t InHiBit01, uint32_t InLoBit24, uint32_t InLoBit23, uint32_t InLoBit22, uint32_t InLoBit21, uint32_t InLoBit20, uint32_t InLoBit19, uint32_t InLoBit18, uint32_t InLoBit17, uint32_t InLoBit16, uint32_t InLoBit15, uint32_t InLoBit14, uint32_t InLoBit13, uint32_t InLoBit12, uint32_t InLoBit11, uint32_t InLoBit10, uint32_t InLoBit09, uint32_t InLoBit08, uint32_t InLoBit07, uint32_t InLoBit06, uint32_t InLoBit05, uint32_t InLoBit04, uint32_t InLoBit03, uint32_t InLoBit02, uint32_t InLoBit01);

void
DSMShuntStatusPack
(DSMShuntStatus* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMShuntStatusClear
(DSMShuntStatus* InPacket);

void
DSMShuntStatusUnpack
(DSMShuntStatus* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMReportCurrents2 API Functions
DSMReportCurrents2*
DSMReportCurrents2Create
();

void
DSMReportCurrents2Destroy
(DSMReportCurrents2* InDSMReportCurrents2);

void
DSMReportCurrents2Set
(DSMReportCurrents2* InPacket, uint16_t InAmps4, uint16_t InAmps3, uint16_t InAmps2, uint16_t InAmps1);

void
DSMReportCurrents2SetFull
(DSMReportCurrents2* InPacket,  uint16_t InAmps4, uint16_t InAmps3, uint16_t InAmps2, uint16_t InAmps1);

void
DSMReportCurrents2Pack
(DSMReportCurrents2* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMReportCurrents2Clear
(DSMReportCurrents2* InPacket);

void
DSMReportCurrents2Unpack
(DSMReportCurrents2* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMReportCurrents4 API Functions
DSMReportCurrents4*
DSMReportCurrents4Create
();

void
DSMReportCurrents4Destroy
(DSMReportCurrents4* InDSMReportCurrents4);

void
DSMReportCurrents4Set
(DSMReportCurrents4* InPacket, uint32_t InAmps1, uint32_t InAmps2);

void
DSMReportCurrents4SetFull
(DSMReportCurrents4* InPacket,  uint32_t InAmps1, uint32_t InAmps2);

void
DSMReportCurrents4Pack
(DSMReportCurrents4* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMReportCurrents4Clear
(DSMReportCurrents4* InPacket);

void
DSMReportCurrents4Unpack
(DSMReportCurrents4* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! PartNumber1V2 API Functions
PartNumber1V2*
PartNumber1V2Create
();

void
PartNumber1V2Destroy
(PartNumber1V2* InPartNumber1V2);

void
PartNumber1V2Set
(PartNumber1V2* InPacket, char* InPartNumber);

void
PartNumber1V2SetFull
(PartNumber1V2* InPacket,  char* InPartNumber);

void
PartNumber1V2Pack
(PartNumber1V2* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
PartNumber1V2Clear
(PartNumber1V2* InPacket);

void
PartNumber1V2Unpack
(PartNumber1V2* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! BootCodeVersion API Functions
BootCodeVersion*
BootCodeVersionCreate
();

void
BootCodeVersionDestroy
(BootCodeVersion* InBootCodeVersion);

void
BootCodeVersionSet
(BootCodeVersion* InPacket, uint8_t InBootMajor, uint8_t InBootMinor, uint8_t InBootBugFix, uint8_t InBootBuild);

void
BootCodeVersionSetFull
(BootCodeVersion* InPacket,  uint8_t InBootMajor, uint8_t InBootMinor, uint8_t InBootBugFix, uint8_t InBootBuild);

void
BootCodeVersionPack
(BootCodeVersion* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
BootCodeVersionClear
(BootCodeVersion* InPacket);

void
BootCodeVersionUnpack
(BootCodeVersion* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! PartNumber2V2 API Functions
PartNumber2V2*
PartNumber2V2Create
();

void
PartNumber2V2Destroy
(PartNumber2V2* InPartNumber2V2);

void
PartNumber2V2Set
(PartNumber2V2* InPacket, char* InPartNumber);

void
PartNumber2V2SetFull
(PartNumber2V2* InPacket,  char* InPartNumber);

void
PartNumber2V2Pack
(PartNumber2V2* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
PartNumber2V2Clear
(PartNumber2V2* InPacket);

void
PartNumber2V2Unpack
(PartNumber2V2* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMStatus1 API Functions
DSMStatus1*
DSMStatus1Create
();

void
DSMStatus1Destroy
(DSMStatus1* InDSMStatus1);

void
DSMStatus1Set
(DSMStatus1* InPacket, uint32_t InID2Bits);

void
DSMStatus1SetFull
(DSMStatus1* InPacket,  uint32_t InID2Bit24, uint32_t InID2Bit23, uint32_t InID2Bit22, uint32_t InID2Bit21, uint32_t InID2Bit20, uint32_t InID2Bit19, uint32_t InID2Bit18, uint32_t InID2Bit17, uint32_t InID2Bit16, uint32_t InID2Bit15, uint32_t InID2Bit14, uint32_t InID2Bit13, uint32_t InID2Bit12, uint32_t InID2Bit11, uint32_t InID2Bit10, uint32_t InID2Bit09, uint32_t InID2Bit08, uint32_t InID2Bit07, uint32_t InID2Bit06, uint32_t InID2Bit05, uint32_t InID2Bit04, uint32_t InID2Bit03, uint32_t InID2Bit02, uint32_t InID2Bit01);

void
DSMStatus1Pack
(DSMStatus1* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMStatus1Clear
(DSMStatus1* InPacket);

void
DSMStatus1Unpack
(DSMStatus1* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMStatus2 API Functions
DSMStatus2*
DSMStatus2Create
();

void
DSMStatus2Destroy
(DSMStatus2* InDSMStatus2);

void
DSMStatus2Set
(DSMStatus2* InPacket, uint32_t InID1Bits, uint32_t InID0Bits);

void
DSMStatus2SetFull
(DSMStatus2* InPacket,  uint32_t InID1Bit24, uint32_t InID1Bit23, uint32_t InID1Bit22, uint32_t InID1Bit21, uint32_t InID1Bit20, uint32_t InID1Bit19, uint32_t InID1Bit18, uint32_t InID1Bit17, uint32_t InID1Bit16, uint32_t InID1Bit15, uint32_t InID1Bit14, uint32_t InID1Bit13, uint32_t InID1Bit12, uint32_t InID1Bit11, uint32_t InID1Bit10, uint32_t InID1Bit09, uint32_t InID1Bit08, uint32_t InID1Bit07, uint32_t InID1Bit06, uint32_t InID1Bit05, uint32_t InID1Bit04, uint32_t InID1Bit03, uint32_t InID1Bit02, uint32_t InID1Bit01, uint32_t InID0Bit24, uint32_t InID0Bit23, uint32_t InID0Bit22, uint32_t InID0Bit21, uint32_t InID0Bit20, uint32_t InID0Bit19, uint32_t InID0Bit18, uint32_t InID0Bit17, uint32_t InID0Bit16, uint32_t InID0Bit15, uint32_t InID0Bit14, uint32_t InID0Bit13, uint32_t InID0Bit12, uint32_t InID0Bit11, uint32_t InID0Bit10, uint32_t InID0Bit09, uint32_t InID0Bit08, uint32_t InID0Bit07, uint32_t InID0Bit06, uint32_t InID0Bit05, uint32_t InID0Bit04, uint32_t InID0Bit03, uint32_t InID0Bit02, uint32_t InID0Bit01);

void
DSMStatus2Pack
(DSMStatus2* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMStatus2Clear
(DSMStatus2* InPacket);

void
DSMStatus2Unpack
(DSMStatus2* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RectifierAlarmStatus API Functions
RectifierAlarmStatus*
RectifierAlarmStatusCreate
();

void
RectifierAlarmStatusDestroy
(RectifierAlarmStatus* InRectifierAlarmStatus);

void
RectifierAlarmStatusSet
(RectifierAlarmStatus* InPacket, uint32_t InAlarms, uint16_t InStatus, uint16_t InCurrent);

void
RectifierAlarmStatusSetFull
(RectifierAlarmStatus* InPacket,  uint32_t InSNS, uint32_t InNDR, uint32_t InACL, uint32_t InDHV, uint32_t InSWO, uint32_t InEMS, uint32_t InHAT, uint32_t InCNV, uint32_t InCBO, uint32_t InINH, uint32_t InFMA, uint32_t InRHV, uint32_t InNPD, uint32_t InPTS, uint32_t InACH, uint32_t InFMI, uint32_t InPHL, uint32_t InPFF, uint32_t InDTS, uint32_t InTCL, uint32_t InPHV, uint32_t InDCU, uint32_t InLSI, uint32_t InDNP, uint32_t InAAA, uint16_t InLSC, uint16_t InRLS, uint16_t InLIN, uint16_t InCPR, uint16_t InVIN, uint16_t InCurrent);

void
RectifierAlarmStatusPack
(RectifierAlarmStatus* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RectifierAlarmStatusClear
(RectifierAlarmStatus* InPacket);

void
RectifierAlarmStatusUnpack
(RectifierAlarmStatus* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMAlarmStatus API Functions
DSMAlarmStatus*
DSMAlarmStatusCreate
();

void
DSMAlarmStatusDestroy
(DSMAlarmStatus* InDSMAlarmStatus);

void
DSMAlarmStatusSet
(DSMAlarmStatus* InPacket, uint16_t InAlarms, uint32_t InFuseBreakerAlarms, uint16_t InStatus);

void
DSMAlarmStatusSetFull
(DSMAlarmStatus* InPacket,  uint16_t InCTL, uint16_t InNDR, uint16_t InABC, uint16_t InSHR, uint16_t InABF, uint16_t InLVD, uint16_t InDCF, uint16_t InFBA, uint16_t InUFB, uint16_t InSOC, uint16_t InDNP, uint16_t InIOC, uint16_t InNRY, uint16_t InAAA, uint32_t InAlarmBit24, uint32_t InAlarmBit23, uint32_t InAlarmBit22, uint32_t InAlarmBit21, uint32_t InAlarmBit20, uint32_t InAlarmBit19, uint32_t InAlarmBit18, uint32_t InAlarmBit17, uint32_t InAlarmBit16, uint32_t InAlarmBit15, uint32_t InAlarmBit14, uint32_t InAlarmBit13, uint32_t InAlarmBit12, uint32_t InAlarmBit11, uint32_t InAlarmBit10, uint32_t InAlarmBit09, uint32_t InAlarmBit08, uint32_t InAlarmBit07, uint32_t InAlarmBit06, uint32_t InAlarmBit05, uint32_t InAlarmBit04, uint32_t InAlarmBit03, uint32_t InAlarmBit02, uint32_t InAlarmBit01, uint16_t InLLI, uint16_t InBLI, uint16_t InG50, uint16_t InGS1, uint16_t InGS2, uint16_t InS2L, uint16_t InS2B, uint16_t InBT1, uint16_t InBT2, uint16_t InBT3, uint16_t InBT4, uint16_t InBH1, uint16_t InBH2, uint16_t InBH3, uint16_t InBH4);

void
DSMAlarmStatusPack
(DSMAlarmStatus* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMAlarmStatusClear
(DSMAlarmStatus* InPacket);

void
DSMAlarmStatusUnpack
(DSMAlarmStatus* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! RectifierAddressRequest API Functions
RectifierAddressRequest*
RectifierAddressRequestCreate
();

void
RectifierAddressRequestDestroy
(RectifierAddressRequest* InRectifierAddressRequest);

void
RectifierAddressRequestSet
(RectifierAddressRequest* InPacket, uint16_t InNodeDescription, uint32_t InSerialNumber, int16_t InMaxCurrentCapacity);

void
RectifierAddressRequestSetFull
(RectifierAddressRequest* InPacket,  uint16_t InRatedCurrent, uint16_t InREZ, uint32_t InSerialNumber, int16_t InMaxCurrentCapacity);

void
RectifierAddressRequestPack
(RectifierAddressRequest* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
RectifierAddressRequestClear
(RectifierAddressRequest* InPacket);

void
RectifierAddressRequestUnpack
(RectifierAddressRequest* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! DSMAddressRequest API Functions
DSMAddressRequest*
DSMAddressRequestCreate
();

void
DSMAddressRequestDestroy
(DSMAddressRequest* InDSMAddressRequest);

void
DSMAddressRequestSet
(DSMAddressRequest* InPacket, uint16_t InNodeDescription, uint32_t InSerialNumber);

void
DSMAddressRequestSetFull
(DSMAddressRequest* InPacket,  uint16_t InMaxPointsOnBus, uint16_t InLLI, uint16_t InBLI, uint16_t InTOP, uint16_t InGS1, uint16_t InGS2, uint16_t InTBD, uint16_t InG50, uint16_t InOSP, uint16_t InTDB, uint16_t InREZ, uint32_t InSerialNumber);

void
DSMAddressRequestPack
(DSMAddressRequest* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
DSMAddressRequestClear
(DSMAddressRequest* InPacket);

void
DSMAddressRequestUnpack
(DSMAddressRequest* InPacket, unsigned char* InBuffer, uint16_t InLength);

//! NodeAddressResponse API Functions
NodeAddressResponse*
NodeAddressResponseCreate
();

void
NodeAddressResponseDestroy
(NodeAddressResponse* InNodeAddressResponse);

void
NodeAddressResponseSet
(NodeAddressResponse* InPacket, uint16_t InOperationAddress, uint32_t InSerialNumber);

void
NodeAddressResponseSetFull
(NodeAddressResponse* InPacket,  uint16_t InAddress, uint16_t InOperation, uint32_t InSerialNumber);

void
NodeAddressResponsePack
(NodeAddressResponse* InPacket, unsigned char* InBuffer, uint16_t* InLength);

void
NodeAddressResponseClear
(NodeAddressResponse* InPacket);

void
NodeAddressResponseUnpack
(NodeAddressResponse* InPacket, unsigned char* InBuffer, uint16_t InLength);

#endif // _esnacanpacket_h_
