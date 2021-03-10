/*****************************************************************************
 FILE NAME      : node_type.h
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Company
******************************************************************************/
#ifndef _node_type_h_
#define _node_type_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "DSMType.h"
#include "RectifierType.h"

/*****************************************************************************!
 * Imported Types
 *****************************************************************************/
struct _Bay;

/*****************************************************************************!
 * Exported Types
 *****************************************************************************/
#define NODE_PART_NUMBER_SIZE           16
#define NODE_CUSTOMER_TEXT_SIZE         8
#define NODE_CUSTOMER_TEXT_COUNT        4
#define NODE_CUSTOMER_TEXT_SIZE_TOTAL   \
    (NODE_CUSTOMER_TEXT_COUNT * NODE_CUSTOMER_TEXT_SIZE)

#define NODE_TYPE_NONE                  0xFF
#define NODE_TYPE_SYSTEM_UNIT           0x00
#define NODE_TYPE_RECT                  0x02
#define NODE_TYPE_DSM                   0x03

#define NodeStartUpNone                 0
#define NodeStartUpNeedAddress          1
#define NodeStartUpSendInfo             2
#define NodeStartUpStarted              3
#define NodeStartUpWaitingAddress       4
#define NodeStartUpReAdressing          5
#define NodeStartUpSentPartNumber1      6
#define NodeStartUpSentPartNumber2      7
#define NodeStartUpSentRevisionValues   8

/*****************************************************************************!
 * Exported Type : NodeRevisionInfo
 *****************************************************************************/
typedef struct
{
    uint8_t                             bootMajor;
    uint8_t                             bootMinor;
    uint8_t                             bootBugFix;
    uint8_t                             bootBuild;
    uint8_t                             appMajor;
    uint8_t                             appMinor;
    uint8_t                             appBugFix;
    uint8_t                             appBuild;
} NodeRevisionInfo;

/*****************************************************************************!
 * Exported Type : CANInfoFieldType
 *****************************************************************************/
typedef union {
        uint16_t        InfoWord;
        struct {
                uint16_t        type:4, extrainfo:12;
        } infofield;
        struct {
                uint16_t        type:4, unused:1, hi12_lo24:1, abnotset:1, abvalue:1, numposvalue:8;
        } dsmfield;
        struct {
                uint16_t    type:4, gbb:1, ratedcurrent:11;
        } rectfield;
} NodeCANInfoFieldType;

/*****************************************************************************!
 * Exported Type : NodeLocation
 *****************************************************************************/
typedef struct {
    uint8_t                             shelf;
    uint8_t                             unitNumber;
    uint8_t                             slaveNumber;
} NodeLocation;
    
/*****************************************************************************!
 * Exported Type : NodeType
 *****************************************************************************/
typedef struct _NodeType
{
    uint8_t                             startUpState;
    uint64_t                            lastActivityTime;
    uint64_t                            lastUpdateTime;
    bool                                available;
    uint32_t                            systemTime;
    uint32_t                            emergencyStopTime;
    uint32_t                            lowBatteryDisconnectTime;
    struct _Bay*                        bay;
    NodeLocation                        location;
    NodeCANInfoFieldType                canInfo;
    uint32_t                            serialNumber;
    uint16_t                            ESNACANAddress;
    uint16_t                            GBBCANAddress;
    uint16_t                            nodeNumber;
    uint8_t                             nodeType;
    uint8_t                             partNumber[NODE_PART_NUMBER_SIZE+1];

    NodeRevisionInfo                    revisionInfo;
    uint8_t                             customerText[NODE_CUSTOMER_TEXT_SIZE_TOTAL+1];
    uint16_t                            turnOffTime;
    union {
        RectifierType                   rectifier;
        DSMType                         dsm;
    };
    uint64_t                            lastInputMessageTime;
    uint64_t                            lastOutputMessageTime;
    bool                                dumpio;
    bool                                nodeTimedOut;
} NodeType;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
bool
NodeIsRectifier
(NodeType* InNode);

void
NodeClear
(NodeType* InNode);

bool
NodeIsGBBRectifier
(NodeType* InNode);

void
NodeSetLastActiveTime
(NodeType* InNode, uint64_t InLastActiveTime);

void
NodeSetLastInputMessageTime
(NodeType* InNode);

void
NodeAdvanceStartState
(NodeType* InNode);

void
NodeSetStartState
(NodeType* InNode, uint8_t InState);

void
NodeSetAddress
(NodeType* InNode, uint16_t InESNAAddress);

void
NodeSetBay
(NodeType* InNode, struct _Bay* InBay);

void
NodeDisplay
(NodeType* InNode);

string
NodeTypeToString
(uint8_t InNodeType);

bool
NodeTypeIsCommunicating
(NodeType* InNode);
#endif /* _node_type_h_*/
