/*****************************************************************************
 * FILE NAME    : ESNACANInterface.h
 * DATE         : January 17 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _esnacaninterface_h_
#define _esnacaninterface_h_

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
#include "NodeType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define SWAP_BYTE_ORDER_WORD(x)                 ( (x >> 8) | (x << 8) )
#define SWAP_BYTE_ORDER_DWORD(x)                ( (x >> 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x000FF00) | (x << 24) )

#define ESNA_CAN_SYSTEM_PRIORITY                0x00
#define ESNA_CAN_TOOL_PRIORITY                  0x01    
#define ESNA_CAN_MON_PRIORITY                   0x02    
#define ESNA_CAN_BCM_PRIORITY                   0x03    
#define ESNA_CAN_IDENTIFY_PRIORITY              0x05    
#define ESNA_CAN_ROUTINE_PRIORITY               0x07    
#define ESNA_CAN_ADDRESS_PRIORITY               0x0b    

#define ESNA_NODE_REQ_SET_SYS_TIME              0x00
#define ESNA_NODE_REQ_SET_ESTOP_TIME            0x01
#define ESNA_RECT_REQ_REMOTE_HVS                0x03
#define ESNA_NODE_REQ_UPDATE_NV_INVENTORY       0x04
#define ESNA_RECT_REQ_WRITE_DEFAULT_PARAMS      0x05
#define ESNA_NODE_REQ_READ_MEMORY_LOCATION      0x10
#define ESNA_NODE_RES_READ_MEMORY_LOCATION      0x10
#define ESNA_NODE_REQ_RESTART                   0x14

#define ESNA_NODE_REQ_PART_NUMBER1              0x15
#define ESNA_NODE_RES_PART_NUMBER1              0x15

#define ESNA_NODE_REQ_REVISION_NUMBERS          0x16
#define ESNA_NODE_RES_REVISION_NUMBERS          0x16

#define ESNA_NODE_REQ_CUSTOMER_TEXT             0x18
#define ESNA_NODE_REQ_WRITE_CUSTOMER_TEXT1      0x18
#define ESNA_NODE_RES_WRITE_CUSTOMER_TEXT1      0x18
#define ESNA_NODE_REQ_WRITE_CUSTOMER_TEXT2      0x19
#define ESNA_NODE_RES_WRITE_CUSTOMER_TEXT2      0x19
#define ESNA_NODE_REQ_WRITE_CUSTOMER_TEXT3      0x1A
#define ESNA_NODE_RES_WRITE_CUSTOMER_TEXT3      0x1A
#define ESNA_NODE_REQ_WRITE_CUSTOMER_TEXT4      0x1B
#define ESNA_NODE_RES_WRITE_CUSTOMER_TEXT4      0x1B

#define ESNA_NODE_REQ_SET_TURN_OFF_TIME         0x1D
#define ESNA_NODE_RES_TURN_OFF_TIME             0x1D

#define ESNA_RECT_REQ_RATED_INPUT_VOLT          0x1E
#define ESNA_RECT_RES_RATED_INPUT_VOLT          0x1E

#define ESNA_RECT_RES_RATED_OUTPUT_VOLT         0x1F
#define ESNA_RECT_REQ_RATED_OUTPUT_VOLT         0x1F

#define ESNA_RECT_REQ_RATED_OUTPUT_CURRENT      0x20
#define ESNA_RECT_RES_RATED_OUTPUT_CURRENT      0x20

#define ESNA_RECT_REQ_DEFAULT_PARAMS            0x21
#define ESNA_RECT_RES_DEFAULT_PARAMS            0x21

#define ESNA_RECT_REQ_NORMAL_PARAMS             0x22
#define ESNA_RECT_WRI_NORMAL_PARAMS             0x22
#define ESNA_RECT_RES_NORMAL_PARAMS             0x22

#define ESNA_DSM_RES_SHUNT_STATUS               0x23

#define ESNA_DSM_REQ_PARAMETERS                 0x24
#define ESNA_DSMW_RI_PARAMETERS                 0x24
#define ESNA_DSM_RES_PARAMETERS                 0x24

#define ESNA_NODE_RES_PART_NUMBER2              0x25
#define ESNA_NODE_REQ_PART_NUMBER2              0x25

#define ESNA_NODE_RES_IDENTIFICATION            0x27
#define ESNA_NODE_REQ_IDENTIFICATION            0x27

#define ESNA_DSM_RES_MEASURE_CURRENTS           0x28
#define ESNA_DSM_RES_MEASURE_CURRENTS_01_04     0x28
#define ESNA_DSM_RES_MEASURE_CURRENTS_05_08     0x29
#define ESNA_DSM_RES_MEASURE_CURRENTS_09_12     0x2A
#define ESNA_DSM_RES_MEASURE_CURRENTS_13_16     0x2B
#define ESNA_DSM_RES_MEASURE_CURRENTS_17_20     0x2C
#define ESNA_DSM_RES_MEASURE_CURRENTS_21_24     0x2D
#define ESNA_DSM_RES_MEASURE_CURRENTS_25_28     0x2E
#define ESNA_DSM_RES_MEASURE_CURRENTS_29_32     0x2F

#define ESNA_NODE_REQ_MISC_INFO                 0x35
#define ESNA_NODE_RES_MISC_INFO                 0x35

#define ESNA_DSM_RES_FUSE_BREAKER_STATUS_2      0x3A

#define ESNA_DSM_RES_FUSE_BREAKER_STATUS_0_1    0x3B

#define ESNA_DSM_RES_BATTERY_VOLTAGE            0x3E

#define ESNA_DSM_RES_BATTERY_TEMP               0x3F

#define ESNA_RECT_RES_ALARM_STATUS              0x41

#define ESNA_DSM_RES_ALARM_STATUS               0x42

#define ESNA_DSM_RES_BUS_VOLTAGES               0x50

#define ESNA_DSM_RES_LVD_ALARMS                 0x52

/*****************************************************************************!
 * Exported Type
 *****************************************************************************/
#pragma pack(1)
typedef union _ESNACANID {
    uint8_t                             byte[4];
    uint16_t                            word[2];
    uint32_t                            dword;
    struct {
        uint32_t                        unitNumber:4;
        uint32_t                        shelfNumber:4;
        uint32_t                        nodeType:4;
        uint32_t                        sourceAddress:12;
        uint32_t                        type:4;
        uint32_t                        source:1;
        uint32_t                        empty:3;
    } AddressRequest;
    struct {
        uint32_t                        command:12;
        uint32_t                        nodeAddress:12;
        uint32_t                        type:4;
        uint32_t                        source:1;
        uint32_t                        empty:3;
    } NodeRequest;
} ESNACANID;

/*****************************************************************************!
 * Exported Type : ESNACANData 
 *****************************************************************************/
#pragma pack(1)
typedef union _ESNACANData
{
    uint8_t                             byte[8];
    uint16_t                            word[4];
    uint32_t                            dword[2];
    uint64_t                            qword;
    struct {
        uint16_t                        midWord1;
        uint32_t                        midDWord;
        uint16_t                        midWord2;
    };
} ESNACANData;

/*****************************************************************************!
 * Exported Type : ESNACANMessage
 *****************************************************************************/
typedef struct _ESNACANMessage
{
    bool                                rtr;
    ESNACANID                           id;
    ESNACANData                         data;
    uint8_t                             dataLength;
} ESNACANMessage;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
ESNACANSendNodeAddressRequest
(uint8_t InNodeType, uint8_t InUnitNumber,
 uint16_t InNodeDescription, uint32_t InSerialNumber);

void
ESNACANSendPartNumber1
(uint16_t InESNAAddress, uint8_t* InPartNumber1);

void
ESNACANSendPartNumber2
(uint16_t InESNAAddress, uint8_t* InPartNumber1);

void
ESNACANSendRevisionInfo
(uint16_t InESNAAddress, NodeRevisionInfo* InRevisionInfo);

void
ESNACANSendCustomerText
(uint16_t InESNAAddress, uint8_t* InCustomerText);

void
ESNACANSendRectifierRatedInputVoltage
(uint16_t InESNAAddress, uint16_t InVoltage, uint16_t InVoltageType);

void
ESNACANSendRectifierRatedOutputVoltage
(uint16_t InESNAAddress, uint16_t InVoltage, uint16_t InVoltageType);

void
ESNACANSendRectifierRatedOutputCurrent
(uint16_t InESNAAddress, uint16_t InCurrent, uint16_t InCurrentType);

void
ESNACANSendRectifierAlarmsStatus
(uint16_t InESNAAddress, RectifierType* InRectifier);

void
ESNACANSendRectifierNodeAddressRequest
(
 uint8_t                                InUnitNumber,
 uint16_t                               InRatedCurrent,
 uint16_t                               InMaxCurrentCapacity,
 uint32_t                               InSerialNumber
);

void
ESNACANSendDSMNodeAddressRequest
(
 uint8_t                                InUnitNumber,
 uint16_t                               InMaxPointsOnBus,
 uint32_t                               InSerialNumber
);

void
ESNACANSendDSMShuntStatusInstallUpdate_0_1
(uint16_t InNodeAddress, DSMType* InDSM);

void
ESNACANSendDSMShuntStatusInstallUpdate_2
(uint16_t InNodeAddress, DSMType* InDSM);

void
ESNACANSendDSMStatusUpdate
(uint16_t InNodeAddress, DSMType* InDSM);

void
ESNACANSendDSMShuntStatusUpdate
(uint16_t InNodeAddress, DSMType* InDSM);

void
ESNACANSendDSMShuntCurrentsUpdate
(uint16_t InNodeAddress, DSMType* InDSM, uint16_t InCommand);

void
ESNACANSendDSMUpdate1
(NodeType* InNode);
void
ESNACANSendDSMUpdate2
(NodeType* InNode);
#endif /* _esnacaninterface_h_*/
