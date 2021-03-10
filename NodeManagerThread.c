/*****************************************************************************
 * FILE NAME    : NodeManagerThread.c
 * DATE         : January 22 2019
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
#include <time.h>
#include <math.h>
#include <unistd.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "NodeDataStore.h"
#include "NodeType.h"
#include "NodeManagerThread.h"
#include "ESNACANInterface.h"
#include "Thread.h"
#include "SystemTime.h"
#include "System.h"
#include "BayType.h"
#include "CLIThread.h"
#include "CANIOThread.h"
#include "main.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define ACTIVITY_TIME_PERIOD            1000
#define LAPSED_INPUT_TIMEOUT            (ACTIVITY_TIME_PERIOD * 5)
#define LAPSED_UPDATE_TIMEOUT           ACTIVITY_TIME_PERIOD
#define LAPSED_UPDATE_DSM_TIMEOUT_1     170
#define LAPSED_UPDATE_DSM_TIMEOUT_2     830
#define LAPSED_ADDRESS_REQUEST_TIME     ACTIVITY_TIME_PERIOD
#define LAPSED_INTERFACE_TIMEOUT        (ACTIVITY_TIME_PERIOD * 60)
#define LAPSED_NODE_INFO_REQUEST_TIME   (ACTIVITY_TIME_PERIOD * 2)
#define WRITE_PAUSE_TIME                (1 * 1000)

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
ThreadHandle
NodeManagerThreadHandle;

/*****************************************************************************!
 * Imported Functions
 *****************************************************************************/
extern int usleep (__useconds_t __useconds);

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void
LogReaddressAttempt
(NodeType* InNode);

static void
LogAddressAssigned
(NodeType* InNode);

static void
NodeManagerSendAddressRequest
(NodeType* InNode);

/*****************************************************************************!
 * Function : NodeManagerInit
 *****************************************************************************/
void
NodeManagerInit()
{
}

/*****************************************************************************!
 * Function : NodeManagerThread
 *****************************************************************************/
void*
NodeManagerThread
(
 void*                          InParameters
)
{
  NodeType*                     node = NULL;
  int64_t                       t, n, lastBayCheckTime; 
  Bay*                          bay = NULL;

  bay = BaySetFindBayByIndex(mainBays, 1);
  while (true) {
    usleep(WRITE_PAUSE_TIME);
    // Loop over all configured nodes
    node = (NULL == node) ? NodeDataStoreGetFirstNode() : NodeDataStoreGetNextNode(node);
    if ( NULL == node ) {
      continue;
    }

    t = SystemGetTimeInMilliseconds();

    //! This is a check to see if we think the SCLAN interface has gone away.
    //  Until we have a more definitive way of checking this will have to do
    if ( !BayAnyNodeCommunicating(bay) ) {
      if ( BayAllNodesTimedOut(bay) ) {
        if ( lastBayCheckTime == 0 ) {
          lastBayCheckTime = t;
        } else {
          n = t - lastBayCheckTime;
          if ( n  > LAPSED_INTERFACE_TIMEOUT )  {
            CANIOThreadRestart();
            lastBayCheckTime = t;
          }
        }
      }
    }
     
    if ( node->startUpState == NodeStartUpNeedAddress ) {
      n = t - node->lastActivityTime;
      if ( n > LAPSED_ADDRESS_REQUEST_TIME ) {
        NodeManagerSendAddressRequest(node);
        node->lastActivityTime = t;
        node->lastUpdateTime = 0;
        node->startUpState = NodeStartUpWaitingAddress;
      }
      continue;
    } 

    //! We are waiting for an address from the BCM
    if ( node->startUpState == NodeStartUpWaitingAddress ) {
      n = t - node->lastActivityTime;
      if ( n > LAPSED_ADDRESS_REQUEST_TIME ) {
        node->lastActivityTime = t;
        node->lastUpdateTime = 0;
        NodeManagerSendAddressRequest(node);
      } 
      continue;
    }
    
    //! We have and address now we start sending periodic updates
    //  Either a DSM or rectifier
    n = t - node->lastUpdateTime;
    // Check to see if more than the alotted time has elapsed or the clock has
    //   moved backwards
    if ( node->nodeType == NODE_TYPE_RECT ) {
      if ( n > LAPSED_UPDATE_TIMEOUT || n < 0 ) { 
        ESNACANSendRectifierAlarmsStatus(node->ESNACANAddress, &(node->rectifier));
        node->lastActivityTime = t;
        node->lastUpdateTime = t;
        node->nodeTimedOut = false;
      }
    } else { // node->nodeType == NODE_TYPE_DSM
      if ( node->dsm.dsmUpdateState == DSM_FIRST_UPDATE_STATE1 &&
           (n > LAPSED_UPDATE_DSM_TIMEOUT_1  || n < 0 ) ) {
        node->lastActivityTime = t;
        node->lastUpdateTime = t;
        ESNACANSendDSMUpdate1(node);
        node->nodeTimedOut = false;
      } else if ( node->dsm.dsmUpdateState == DSM_FIRST_UPDATE_STATE2 &&
                  (n > LAPSED_UPDATE_DSM_TIMEOUT_2 || n < 0 ) ) {
        node->lastActivityTime = t;
        node->lastUpdateTime = t;
        ESNACANSendDSMUpdate2(node);
        node->nodeTimedOut = false;
      }
    }
      
    //! We have received an address now we cycle through the 
    //  initial startup data requests from the BCM.
    if ( node->startUpState == NodeStartUpSendInfo || 
         node->startUpState == NodeStartUpSentPartNumber1 ||
         node->startUpState == NodeStartUpSentPartNumber2 ) {
      n = t - node->lastActivityTime;
      if ( n > LAPSED_NODE_INFO_REQUEST_TIME ) {
        node->lastActivityTime = t;
        node->lastUpdateTime = 0;
        NodeManagerSendAddressRequest(node);
      } 
      continue;
    }

    //! We have reeived all the startup data requests from the BCM.
    //  From here we starting processing routine requests
    if ( node->startUpState == NodeStartUpSentRevisionValues ) {
      LogAddressAssigned(node);
      node->startUpState = NodeStartUpStarted;
      continue;
    } 

    //! Process routine requests
    if ( node->startUpState == NodeStartUpStarted ) {
      n = t - node->lastInputMessageTime;
      if ( n > LAPSED_INPUT_TIMEOUT ) {
        //! More than 5 seconds has elapsed since we last heard from the BCM
        //  We now setup to request a new address.
        node->startUpState = NodeStartUpNeedAddress;
        node->lastInputMessageTime = 0;
        node->lastUpdateTime = 0;
        node->nodeTimedOut = true;
        LogReaddressAttempt(node);
        continue;
      }

    }
  }
}

/******************************************************************************!
 * Function : NodeManagerSendAddressRequest
 ******************************************************************************/
void
NodeManagerSendAddressRequest
(NodeType* InNode)
{
  if ( InNode->nodeType == NODE_TYPE_RECT ) {
    ESNACANSendRectifierNodeAddressRequest(InNode->location.unitNumber,
                                           InNode->rectifier.ratedOutputCurrent,
                                           InNode->rectifier.maxCurrentCapacity,
                                           InNode->serialNumber);
  } else {
    ESNACANSendDSMNodeAddressRequest(InNode->location.unitNumber,
                                     InNode->dsm.description.bit.maxPointsOnBus,
                                     InNode->serialNumber);
  }
}

/******************************************************************************!
 * Function : LogReaddressAttempt
 ******************************************************************************/
void
LogReaddressAttempt
(NodeType* InNode)
{
  if ( InNode == NULL ) {
    return;
  }
  SystemTime                            t;
  SystemGetTime(&t); 
  CLILogMessage("Node Timed Out : %s %d re-addressed at %02d/%02d/%04d - %02d:%02d:%02d.%03d\n", 
                 NodeTypeToString(InNode->nodeType),
                 InNode->location.unitNumber,
                 t.month, t.day, t.year, t.hour, t.minute, t.second, t.millisecond);
}

/******************************************************************************!
 * Function : LogAddressAssigned
 ******************************************************************************/
void
LogAddressAssigned
(NodeType* InNode)
{
  if ( InNode == NULL ) {
    return;
  }
  SystemTime                            t;
  SystemGetTime(&t); 
  CLILogMessage("%s %d assigned address %d at %02d/%02d/%04d - %02d:%02d:%02d.%03d\n",
                NodeTypeToString(InNode->nodeType),
                 InNode->location.unitNumber, InNode->ESNACANAddress,
                 t.month, t.day, t.year, t.hour, t.minute, t.second, t.millisecond);
}
