/*****************************************************************************
 * FILE NAME    : ProcessInputMessageThread.c
 * DATE         : January 21 2019
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

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "CANIOThread.h"
#include "NodeType.h"
#include "NodeDataStore.h"
#include "HTTPServerThread.h"
#include "BayType.h"
#include "Thread.h"
#include "SystemTime.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
ThreadHandle
ProcessInMessageThreadHandle;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void
ProcessInputMessage
(ESNACANMessage* InMessage);

void
ProcessCANAddressInputMessage
(ESNACANMessage* InMessage);

void
ProcessBCMInputMessage
(ESNACANMessage* InMessage);

void
ProcessRoutineInputMessage
(ESNACANMessage* InMessage);

void
ProcessToolInputMessage
(ESNACANMessage* InMessage);

void
ProcessCANRectNormalParameters
(ESNACANMessage* InMessage);

void
ProcessCANNodeRevisionNumbers
(ESNACANMessage* InMessage);

void
ProcessCANNodeCustomerText
(ESNACANMessage* InMessage);

void
ProcessCANNodeWriteCustomerText
(ESNACANMessage* InMessage);

void
ProcessCANNodeRatedInputVolt
(ESNACANMessage* InMessage);

void
ProcessCANNodeRatedOutputVolt
(ESNACANMessage* InMessage);

void
ProcessCANNodeRatedOutputCurrent
(ESNACANMessage* InMessage);

void
ProcessCANNodeDefaultParams
(ESNACANMessage* InMessage);

void
ProcessCANNodePartNumber1
(ESNACANMessage* InMessage);

void
ProcessCANNodePartNumber2
(ESNACANMessage* InMessage);

void
ProcessCANRectReadNormalParams
(ESNACANMessage* InMessage);

void
ProcessSystemInputMessage
(ESNACANMessage* InMessage);

void
ProcessRectWriteDefaultParams
(ESNACANMessage* InMessage);

/*****************************************************************************!
 * Function : ProcessInputMessageInit
 *****************************************************************************/
void
ProcessInputMessageInit
()
{
}

/*****************************************************************************!
 * Function : ProcessInputMessageThread
 *****************************************************************************/
void*
ProcessInputMessageThread
(
 void*                                  InParameters
)
{
  ESNACANMessage                      message;
    
  while (true) {
    if ( ESNACANMessageQueueIsEmpty(CANReadQueue) ) {
      continue;
    }
    ESNACANMessageQueueDequeue(CANReadQueue, &message);
    ProcessInputMessage(&message);
  }
}

/*****************************************************************************!
 * Function : ProcessInputMessage
 *****************************************************************************/
void
ProcessInputMessage
(
 ESNACANMessage*                        InMessage
)
{
  if ( NULL == InMessage ) {
    return;
  }

  switch (InMessage->id.NodeRequest.type) {
    case ESNA_CAN_SYSTEM_PRIORITY : {
      ProcessSystemInputMessage(InMessage);
      break;
    }
            
    case ESNA_CAN_TOOL_PRIORITY : {
      ProcessToolInputMessage(InMessage);
      break;
    }
            
    case ESNA_CAN_MON_PRIORITY : {
      break;
    }
            
    case ESNA_CAN_BCM_PRIORITY : {
      ProcessBCMInputMessage(InMessage);
      break;
    }
            
    case ESNA_CAN_IDENTIFY_PRIORITY : {
      break;
    }
            
    case ESNA_CAN_ROUTINE_PRIORITY : {
      ProcessRoutineInputMessage(InMessage);
      break;
    }
            
    case ESNA_CAN_ADDRESS_PRIORITY : {
      ProcessCANAddressInputMessage(InMessage);
      break;
    }
  }
}

/*****************************************************************************!
 * Function : ProcessBCMInputMessage
 *****************************************************************************/
void
ProcessBCMInputMessage
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                           node;
  uint16_t                            nodeAddress;
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  switch (InMessage->id.NodeRequest.command) {
    case ESNA_NODE_REQ_PART_NUMBER1 : {
      ProcessCANNodePartNumber1(InMessage);
      break;
    }
        
    case ESNA_NODE_REQ_PART_NUMBER2 : {
      ProcessCANNodePartNumber2(InMessage);
      break;
    }
        
    case ESNA_NODE_REQ_REVISION_NUMBERS : {
      ProcessCANNodeRevisionNumbers(InMessage);
      break;
    }
  }
}

/*****************************************************************************!
 * Function : ProcessCANAddressInputMessage
 *****************************************************************************/
void
ProcessCANAddressInputMessage
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                           node;
  uint8_t                             unitNumber, type;
        
  if ( NULL == InMessage ) {
    return;
  }

  unitNumber = InMessage->id.AddressRequest.unitNumber;
  type = InMessage->id.AddressRequest.nodeType;

  node = NodeDataStoreFindExisting(type, unitNumber);
  if ( NULL == node ) {
    printf("\nCould not find node %s %d\n", NodeTypeToString(type), unitNumber);
    return;
  }
  NodeSetLastInputMessageTime(node);
  node->ESNACANAddress = htobe16(InMessage->data.word[0]) & 0x0FFF;
  NodeSetStartState(node, NodeStartUpSendInfo);
  NodeSetLastInputMessageTime(node);
}

/*****************************************************************************!
 * Function : ProcessRoutineInputMessage
 *****************************************************************************/
void
ProcessRoutineInputMessage
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                           node;
  uint16_t                            nodeAddress;
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  switch (InMessage->id.NodeRequest.command) {
    case ESNA_RECT_WRI_NORMAL_PARAMS : {
      ProcessCANRectNormalParameters(InMessage);
      break;
    }
  }
}

/*****************************************************************************!
 * Function : ProcessToolInputMessage
 *****************************************************************************/
void
ProcessToolInputMessage
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                           node;
  uint16_t                            nodeAddress;
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
    
  NodeSetLastInputMessageTime(node);
  switch (InMessage->id.NodeRequest.command) {
    case ESNA_NODE_REQ_CUSTOMER_TEXT : {
      ProcessCANNodeCustomerText(InMessage);
      break;
    }
        
    case ESNA_RECT_REQ_RATED_INPUT_VOLT : {
      ProcessCANNodeRatedInputVolt(InMessage);
      break;
    }
        
    case ESNA_RECT_REQ_RATED_OUTPUT_VOLT : {
      ProcessCANNodeRatedOutputVolt(InMessage);
      break;
    }
        
    case ESNA_RECT_REQ_RATED_OUTPUT_CURRENT : {
      ProcessCANNodeRatedOutputCurrent(InMessage);
      break;
    }
        
    case ESNA_RECT_REQ_DEFAULT_PARAMS : {
      ProcessCANNodeDefaultParams(InMessage);
      break;
    }
        
    case ESNA_RECT_REQ_NORMAL_PARAMS : {
      ProcessCANRectReadNormalParams(InMessage);
      break;
    }        
  }
}

/*****************************************************************************!
 * Function : ProcessCANNodeRevisionNumbers
 *****************************************************************************/
void
ProcessCANNodeRevisionNumbers
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;
  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  NodeSetLastActiveTime(node, SystemGetTimeInMilliseconds());
  node->startUpState = NodeStartUpSentRevisionValues;
  ESNACANSendRevisionInfo(node->ESNACANAddress, &(node->revisionInfo));
}

/*****************************************************************************!
 * Function : ProcessCANNodeCustomerText
 *****************************************************************************/
void
ProcessCANNodeCustomerText
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;

  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  ESNACANSendCustomerText(nodeAddress, node->customerText);
  NodeSetLastInputMessageTime(node);
  NodeSetLastActiveTime(node, SystemGetTimeInMilliseconds());
}

/*****************************************************************************!
 * Function : ProcessCANNodeWriteCustomerText
 *****************************************************************************/
void
ProcessCANNodeWriteCustomerText
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;

  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  NodeSetLastInputMessageTime(node);
  if ( NULL == node ) {
    return;
  }
}

/*****************************************************************************!
 * Function : ProcessCANNodeRatedInputVolt
 *****************************************************************************/
void
ProcessCANNodeRatedInputVolt
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;

  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  ESNACANSendRectifierRatedInputVoltage(nodeAddress,
                                        node->rectifier.ratedInputVoltage,
                                        node->rectifier.ratedInputVoltageType);
}

/*****************************************************************************!
 * Function : ProcessCANNodeRatedOutputVolt
 *****************************************************************************/
void
ProcessCANNodeRatedOutputVolt
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;

  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  ESNACANSendRectifierRatedInputVoltage(nodeAddress,
                                        node->rectifier.ratedInputVoltage,
                                        node->rectifier.ratedInputVoltageType);
    
}

/*****************************************************************************!
 * Function : ProcessCANNodeRatedOutputCurrent
 *****************************************************************************/
void
ProcessCANNodeRatedOutputCurrent
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;

  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  ESNACANSendRectifierRatedOutputCurrent(nodeAddress,
                                         node->rectifier.ratedOutputCurrent,
                                         node->rectifier.ratedOutputCurrentType);
}

/*****************************************************************************!
 * Function : ProcessCANNodeDefaultParams
 *****************************************************************************/
void
ProcessCANNodeDefaultParams
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;

  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
}

/*****************************************************************************!
 * Function : ProcessCANRectReadNormalParams
 *****************************************************************************/
void
ProcessCANRectReadNormalParams
(
 ESNACANMessage*                InMessage
)
{
  NodeType*                   node;
  uint16_t                    nodeAddress;

  if ( NULL == InMessage ) {
    return;
  }
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
}

/*****************************************************************************!
 * Function : ProcessCANRectNormalParameters
 *****************************************************************************/
void
ProcessCANRectNormalParameters
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                           node;

  node = NodeDataStoreFindNodeByAddress(InMessage->id.NodeRequest.nodeAddress);
  if ( NULL == node ) {
    return;
  }

  node->rectifier.parameters.volts            = InMessage->data.word[3];
  node->rectifier.parameters.current          = InMessage->data.word[2];
  node->rectifier.parameters.hsvVolts         = InMessage->data.word[1];
  node->rectifier.parameters.flags.allFlags   = InMessage->data.word[0];
  NodeSetLastInputMessageTime(node);
}

/*****************************************************************************!
 * Function : ProcessSystemInputMessage
 *****************************************************************************/
void
ProcessSystemInputMessage
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                           node;
  uint16_t                            nodeAddress;
  nodeAddress = InMessage->id.NodeRequest.nodeAddress;
  node = NodeDataStoreFindNodeByAddress(nodeAddress);
  if ( NULL == node ) {
    return;
  }
    
  NodeSetLastInputMessageTime(node);
  switch (InMessage->id.NodeRequest.command) {
    case ESNA_NODE_REQ_SET_SYS_TIME : {
      break;
    }
      
    case ESNA_NODE_REQ_SET_ESTOP_TIME : {
      break;
    }
      
    case ESNA_RECT_REQ_REMOTE_HVS : {
      break;
    }
      
    case ESNA_NODE_REQ_UPDATE_NV_INVENTORY : {
      break;
    }
      
    case ESNA_RECT_REQ_WRITE_DEFAULT_PARAMS : {
      ProcessRectWriteDefaultParams(InMessage);
      break;
    }
  }
  NodeSetLastInputMessageTime(node);
}

/*****************************************************************************!
 * Function : ProcessRectWriteDefaultParams
 *****************************************************************************/
void
ProcessRectWriteDefaultParams
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                             node;
  uint16_t                              voltageOut;
  uint16_t                              voltageHVS;

  voltageOut = InMessage->data.word[3];
  voltageHVS = InMessage->data.word[1];
  
  for (node = NodeDataStoreGetFirstTypedNode(NODE_TYPE_RECT);
       node;
       node = NodeDataStoreGetNextTypedNode(node)) {
    node->rectifier.parameters.hsvVolts = voltageHVS;
    node->rectifier.parameters.volts    = voltageOut;
    NodeSetLastInputMessageTime(node);
  }
}

/*****************************************************************************!
 * Function : ProcessCANNodePartNumber1
 *****************************************************************************/
void
ProcessCANNodePartNumber1
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                             node;

  if ( NULL == InMessage ) {
    return;
  }
  node = NodeDataStoreFindNodeByAddress(InMessage->id.NodeRequest.nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  NodeSetLastActiveTime(node, SystemGetTimeInMilliseconds());
  node->startUpState = NodeStartUpSentPartNumber1;
  ESNACANSendPartNumber1(InMessage->id.NodeRequest.nodeAddress, node->partNumber);
}

/*****************************************************************************!
 * Function : ProcessCANNodePartNumber2
 *****************************************************************************/
void
ProcessCANNodePartNumber2
(
 ESNACANMessage*                        InMessage
)
{
  NodeType*                             node;
  
  if ( NULL == InMessage ) {
    return;
  }
  node = NodeDataStoreFindNodeByAddress(InMessage->id.NodeRequest.nodeAddress);
  if ( NULL == node ) {
    return;
  }
  NodeSetLastInputMessageTime(node);
  NodeSetLastActiveTime(node, SystemGetTimeInMilliseconds());
  node->startUpState = NodeStartUpSentPartNumber2;
  ESNACANSendPartNumber2(InMessage->id.NodeRequest.nodeAddress, &(node->partNumber[8]));
}

