/*****************************************************************************
 FILE NAME      : DsmType.h
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Company
******************************************************************************/
#ifndef _dsmtype_h_
#define _dsmtype_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdint.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "DSMElementType.h"
#include "ShuntType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define DSM_MAX_BATTERY_COUNT   4
#define DSM_MAX_SHUNT_COUNT     24
#define DSM_MAX_SHUNTS          12
#define DSM_FIRST_UPDATE_STATE1 1
#define DSM_MAX_UPDATE_STATE1   6
#define DSM_FIRST_UPDATE_STATE2 7
#define DSM_MAX_UPDATE_STATE2   10
#define DSM_UPDATE_STATE_NONE   0
#define DSM_UPDATE_STATE_1      1
#define DSM_UPDATE_STATE_2      2

/*****************************************************************************!
 * Exported Type : DSMStatusType
 *****************************************************************************/
typedef union {
  uint8_t                       byte[2];
  uint16_t                      word;
  struct {
    uint16_t                    bh4:1;
    uint16_t                    bh3:1;
    uint16_t                    bh2:1;
    uint16_t                    bh1:1;
        
    uint16_t                    bt4:1;
    uint16_t                    bt3:1;
    uint16_t                    bt2:1;
    uint16_t                    bt1:1;

    uint16_t                    s2b:1;
    uint16_t                    s2l:1;
    uint16_t                    gs2:1;
    uint16_t                    gs1:1;

    uint16_t                    g50:1;
    uint16_t                    bli:1;
    uint16_t                    lli:1;
  } bit;
} DSMStatusType;

/*****************************************************************************!
 * Exported Type : DSMAlarmType
 *****************************************************************************/
typedef union {
  uint8_t  byte[2];
  uint16_t word;
  struct {
    uint8_t                     aaa:1; // Any alarm active
    uint8_t                     nry:1; // Not reporting yet
    uint8_t                     b15:1; // Battery halfstring #5 installed
    uint8_t                     b14:1; // Battery halfstring #4 installed
    uint8_t                     ioc:1; // Installed options changed
    uint8_t                     dnp:1; // Duplicate node present 
    uint8_t                     soc:1; // Shunt over curren
    uint8_t                     ufb:1; // Unknown fuse/breaker
    uint8_t                     fba:1; // Fuse/Breaker Alarm
    uint8_t                     dcf:1; // Disconnect controller fail
    uint8_t                     lvd:1; // Low Voltage Disconnect
    uint8_t                     bpf:1; // 
    uint8_t                     shr:1; // Shunt Removed
    uint8_t                     abc:1; // A/B jumper changed
    uint8_t                     ndr:1; // No data received
    uint8_t                     ctl:1; // Controller failed
  } bit;
} DSMAlarmType;

/*****************************************************************************!
 * Exported Type : DSMCommandType
 *****************************************************************************/
typedef union {
  uint8_t  byte;
  struct {
    uint8_t                     lps_aco:1;
    uint8_t                     rdl:1;
    uint8_t                     ctc:1;
    uint8_t                     sic:1;
    uint8_t                     wal:1;
    uint8_t                     b02:1;
    uint8_t                     frc:1;
    uint8_t                     fdc:1;  
  } bit;
} DSMCommandType;

/*****************************************************************************!
 * Exported Type : DSMDescription
 *****************************************************************************/
typedef union {
  struct {
    uint16_t                    maxPointsOnBus:6;
    uint16_t                    lli:1;
    uint16_t                    bli:1;
    uint16_t                    top:1;
    uint16_t                    gs1:1;
    uint16_t                    gs2:1;
    uint16_t                    tbd:1;
    uint16_t                    g50:1;
    uint16_t                    osp:1;
    uint16_t                    tdb:1;
    uint16_t                    rez:1;
  } bit;
  uint16_t                      whole;
} DSMDescription;

/*****************************************************************************!
 * Exported Type : DSMShuntCurrents
 *****************************************************************************/
typedef union {
  uint16_t                      integerAmps[32];
  int32_t                       signedAmps[16];
} DSMShuntCurrents;
    
/*****************************************************************************!
 * Exported Type : DSMType
 *****************************************************************************/
typedef struct {
  DSMAlarmType                  alarm;
  DSMStatusType                 status;
  DSMCommandType                command;
  int                           dsmUpdateState;
  DSMDescription                description;
  DSMShuntCurrents              shuntCurrents;
  uint8_t                       numberOfPointsFoundNow;
  uint8_t                       fuseAlarmByte[3];
  uint8_t                       shuntAlarmHiByte[3];
  uint8_t                       shuntAlarmLoByte[3];
  uint16_t                      busVoltages[2];
  uint8_t                       IDBytes[3][3];
  struct _NodeType*             parentNode;
  Shunt                         shunts[DSM_MAX_SHUNTS];
  uint8_t                       updateState;
} DSMType;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
uint8_t
DSMGetShuntBits
(DSMType* InDSM, int InBitIndex);

uint8_t
DSMGetIDBits
(DSMType* InDSM, int InByteIndex, int InBitIndex);

void
DSMInsertShunt
(DSMType* InDSM, uint8_t InContactIndex, uint8_t InContactWidth, uint32_t InContactnMaxCurrent, ShuntType InShuntType);

bool
DSMShuntFits
(DSMType* InDSM, uint8_t InContactIndex, uint8_t InContactWidth);

void
DSMShuntRemove
(DSMType* InDSM, uint8_t InContactIndex);

bool
DSMShuntOccupied
(DSMType* InDSM, uint8_t InSlotIndex);

void
DSMDisplay
(DSMType* InDSM);

#endif /* _dsmtype_h_*/
