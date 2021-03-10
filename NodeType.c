/*****************************************************************************
 FILE NAME      : NodeType.c
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Company
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "NodeType.h"
#include "ESNACANInterface.h"
#include "BayType.h"
#include "SystemTime.h"
#include "CLIThread.h"

/*****************************************************************************!
 * Function : NodeIsRectifier
 *****************************************************************************/
bool
NodeIsRectifier
(
 NodeType*                              InNode
)
{
    if ( NULL == InNode ) {
        return false;
    }

    return InNode->nodeType == NODE_TYPE_RECT;
}

/*****************************************************************************!
 * Function : NodeIsGBBRectifier
 *****************************************************************************/
bool
NodeIsGBBRectifier
(
 NodeType*                              InNode
)
{
    if ( NULL == InNode ) {
        return false;
    }

    if ( InNode->nodeType != NODE_TYPE_RECT ) {
        return false;
    }

    if ( InNode->canInfo.rectfield.gbb ) {
        return true;
    }

    return false;
}

/*****************************************************************************!
 * Function : NodeIsSystemUnit
 *****************************************************************************/
bool
NodeIsSystemUnit
(
 NodeType*                              InNode
)
{
    if ( NULL == InNode ) {
        return false;
    }
    return InNode->nodeType == NODE_TYPE_SYSTEM_UNIT;
}

/*****************************************************************************!
 * Function : NodeSetLastActiveTime
 *****************************************************************************/
void
NodeSetLastActiveTime
(
 NodeType*                              InNode,
 uint64_t                               InLastActiveTime
)
{
    if ( NULL == InNode ) {
        return;
    }
    InNode->lastActivityTime = InLastActiveTime;
}

/******************************************************************************!
 * Function : NodeSetStartState
 *****************************************************************************/
void
NodeSetStartState
(
 NodeType*                              InNode,
 uint8_t                                InState
)
{
    if ( NULL == InNode ) {
        return;
    }
    InNode->startUpState = InState;
}

/*****************************************************************************!
 * Function : NodeAdvanceStartState
 *****************************************************************************/
void
NodeAdvanceStartState
(
 NodeType*                              InNode
)
{
    if ( NULL == InNode ) {
        return;
    }

    switch(InNode->startUpState) {
        case NodeStartUpNone :
        case NodeStartUpNeedAddress : {
            InNode->startUpState = NodeStartUpSendInfo;
            if ( InNode->nodeType == NODE_TYPE_DSM ) {
                InNode->dsm.updateState = DSM_UPDATE_STATE_NONE;
            }
            break;
        }
            
        case NodeStartUpSendInfo : {
            InNode->startUpState = NodeStartUpStarted;
            if ( InNode->nodeType == NODE_TYPE_DSM ) {
                InNode->dsm.updateState = DSM_FIRST_UPDATE_STATE1;
            }
            break;
        }
            
        case NodeStartUpStarted : {
            break;
        }
    }    
}

/*****************************************************************************!
 * Function : NodeSetAddress
 *****************************************************************************/
void
NodeSetAddress
(
 NodeType*                      InNode,
 uint16_t                       InESNAAddress
)
{
    if ( NULL == InNode ) {
        return;
    }

    InNode->ESNACANAddress = InESNAAddress;
    InNode->nodeNumber = InESNAAddress;
}

/*****************************************************************************!
 * Function : NodeSetBay
 *****************************************************************************/
void
NodeSetBay
(
 NodeType*                      InNode,
 Bay*                           InBay
)
{
    if ( NULL == InNode || NULL == InBay ) {
        return;
    }

    InNode->bay = InBay;
}

/*****************************************************************************!
 * Function : NodeDisplay
 *****************************************************************************/
void
NodeDisplay
(NodeType* InNode)
{

    SystemTime                          t;
    printf("\r\n%s                    Node%s\r\n", HeadingStyle, ColorWhite);
    printf("                    Type : ");
    if ( InNode->nodeType == NODE_TYPE_RECT ) {
        printf("Rectifier\r\n");
    } else if ( InNode->nodeType == NODE_TYPE_DSM ) {
        printf("DSM\r\n");
    }
    printf("                Revision : ");
    printf("App : %d.%d.%d.%d -- Boot : %d.%d.%d.%d\r\n",
           InNode->revisionInfo.appMajor,
           InNode->revisionInfo.appMinor,
           InNode->revisionInfo.appBugFix,
           InNode->revisionInfo.appBuild,
           InNode->revisionInfo.bootMajor,
           InNode->revisionInfo.bootMinor,
           InNode->revisionInfo.bootBugFix,
           InNode->revisionInfo.bootBuild);
    printf("             Unit Number : %d\r\n", InNode->location.unitNumber);
    printf("           Serial Number : %u\r\n", (uint32_t)InNode->serialNumber);
    printf("             Part Number : %s\r\n", InNode->partNumber);
    printf("           Customer Text : %s\r\n", InNode->customerText);
    printf("          Start Up State : %d\r\n", InNode->startUpState);
    printf("        ESNA CAN Address : %d\r\n", InNode->ESNACANAddress);
    SystemConvertTime(&t, InNode->lastActivityTime);
    printf("      Last Activity Time : %02d:%02d:%02d.%03d\r\n", t.hour, t.minute, t.second, t.millisecond);
    SystemConvertTime(&t, InNode->lastInputMessageTime);
    printf("         Last Input Time : %02d:%02d:%02d.%03d\r\n", t.hour, t.minute, t.second, t.millisecond);
    SystemConvertTime(&t, InNode->lastOutputMessageTime);
    printf("        Last Output Time : %02d:%02d:%02d.%03d\r\n", t.hour, t.minute, t.second, t.millisecond);
    printf("               Timed Out : %s\r\n", InNode->nodeTimedOut ? "True" : "False");
}

/******************************************************************************!
 * Function : NodeSetLastInputMessageTime
 ******************************************************************************/
void
NodeSetLastInputMessageTime
(NodeType* InNode)
{
  if ( NULL == InNode ) {
    return;
  }
  InNode->lastInputMessageTime = SystemGetTimeInMilliseconds();
}

/*****************************************************************************!
 * Function : NodeTypeToString
 *****************************************************************************/
string
NodeTypeToString
(uint8_t InNodeType)
{
  if ( NODE_TYPE_DSM == InNodeType ) {
    return "DSM";
  }
  if ( NODE_TYPE_RECT == InNodeType ) {
    return "Rectifier";
  }

  if ( NODE_TYPE_SYSTEM_UNIT == InNodeType ) {
    return "System Unit";
  }

  return "Unknown";
}

/******************************************************************************!
 * Function : NodeTypeIsCommunicating
 ******************************************************************************/
bool
NodeTypeIsCommunicating
(NodeType* InNode)
{
  if ( InNode == NULL ) {
    return false;
  }
  if ( InNode->available ) { 
    return false;
  }
  if ( InNode->nodeTimedOut ) {
    return false;
  }
  if ( InNode->startUpState == NodeStartUpNone || 
       InNode->startUpState == NodeStartUpNeedAddress ||
       InNode->startUpState == NodeStartUpWaitingAddress ) {
     return false;
  }
  return true;
}

/*****************************************************************************!
 * Function : NodeClear
 *****************************************************************************/
void
NodeClear
(NodeType* InNode)
{
  if ( NULL == InNode ) {
    return;
  }
  InNode->startUpState                  = NodeStartUpNone;
  InNode->lastActivityTime              = 0;
  InNode->lastUpdateTime                = 0;
  InNode->available                     = true;
  InNode->systemTime                    = 0;
  InNode->emergencyStopTime             = 0;
  InNode->lowBatteryDisconnectTime      = 0;
  InNode->bay                           = NULL;
  InNode->serialNumber                  = 0;
  InNode->ESNACANAddress                = 0;
  InNode->GBBCANAddress                 = 0;
  InNode->nodeNumber                    = 0;
  InNode->nodeType                      = 0;
  InNode->turnOffTime                   = 0;
  InNode->lastInputMessageTime          = 0;
  InNode->lastOutputMessageTime         = 0;
  InNode->dumpio                        = false;
  InNode->nodeTimedOut                  = false;
}
