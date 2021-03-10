/*****************************************************************************
 FILE NAME      : NodeDataStore.c
 DATE           : July 25 2018
 PROJECT        : 
 COPYRIGHT      : Copyright (C) 2018 by Vertic Company
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <ascii.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "NodeDataStore.h"
#include "BayType.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define NODE_DATASTORE_DSM_INDEX_OFFSET  (NODE_DATASTORE_MAX_NODE_COUNT / 2)

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
NodeType
Nodes[NODE_DATASTORE_MAX_NODE_COUNT+1];

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
NodeType*
NodeDataStoreFindExisting
(uint8_t InNodeType, uint8_t InUnitNumber);

static uint16_t
NodeDataStoreFindNextAvailable
(uint8_t InNodeType);

/*****************************************************************************!
 * Function : NodeDataStoreInit
 *****************************************************************************/
void
NodeDataStoreInit()
{
  int                                 i;

  for (i = 0; i < NODE_DATASTORE_MAX_NODE_COUNT; i++) {
    memset(&(Nodes[i]), 0x00, sizeof(NodeType));
    Nodes[i].nodeType = NODE_TYPE_NONE;
    Nodes[i].available = true;
  }
}

/*****************************************************************************!
 * Function : NodeDataStoreNodeInit
 *****************************************************************************/
NodeType*
NodeDataStoreNodeInit
(
 uint8_t                                InNodeType,
 uint8_t                                InUnitNumber
)
{
  uint16_t                            nodeNumber;
  NodeType*                           node;
    
  NodeDataStoreUpdateStart();

  // Check to see if a node of this type, unit number exists
  node = NodeDataStoreFindExisting(InNodeType, InUnitNumber);
  if ( node != NULL ) {
    // This node already exists
    NodeDataStoreUpdateEnd();
    return node;
  }

  // Find the next available slot in the table
  nodeNumber = NodeDataStoreFindNextAvailable(InNodeType);
  if ( nodeNumber == NODE_DATASTORE_MAX_NODE_COUNT + 1 ) {
    NodeDataStoreUpdateEnd();
    return NULL;
  }

  Nodes[nodeNumber].partNumber[0] = '1';
  memset(&(Nodes[nodeNumber]), NUL, sizeof(NodeType));
  Nodes[nodeNumber].nodeNumber = nodeNumber;
  Nodes[nodeNumber].nodeType = InNodeType;
  Nodes[nodeNumber].ESNACANAddress = 0;
  Nodes[nodeNumber].location.unitNumber = InUnitNumber;
  Nodes[nodeNumber].startUpState = NodeStartUpNeedAddress;
  Nodes[nodeNumber].lastActivityTime = 0;
  Nodes[nodeNumber].lastUpdateTime = 0;
  Nodes[nodeNumber].lastInputMessageTime = 0;
  Nodes[nodeNumber].lastOutputMessageTime = 0;
  if ( InNodeType == NODE_TYPE_RECT ) {
    RectifierInitialize(&Nodes[nodeNumber].rectifier);
  }
  NodeDataStoreUpdateEnd();
  return &Nodes[nodeNumber];
}

/*****************************************************************************!
 * Function : NodeDataStoreNodeClear
 *****************************************************************************/
void
NodeDataStoreNodeClear
(
 uint16_t                               InNodeNum
)
{
  
  NodeDataStoreUpdateStart();
  if ( InNodeNum > NODE_DATASTORE_MAX_NODE_COUNT ) {
    return;
  }
  memset(&(Nodes[InNodeNum]), NUL, sizeof(NodeType));
  Nodes[InNodeNum].available = true;
  NodeDataStoreUpdateEnd();
}

/*****************************************************************************!
 * Funtion : NodeDataStoreGetNode
 *****************************************************************************/
NodeType*
NodeDataStoreGetNode
(
 uint16_t                               InNodeNum
)
{
  if ( InNodeNum > NODE_DATASTORE_MAX_NODE_COUNT ) {
    return NULL;
  }
  if ( Nodes[InNodeNum].nodeNumber != InNodeNum ) {
    return NULL;
  }
  return &Nodes[InNodeNum];
}

/*****************************************************************************!
 * Function : NodeDataStoreUpdateStart
 *****************************************************************************/
bool
NodeDataStoreUpdateStart()
{
  return true;
}

/*****************************************************************************!
 * Function : NodeDataStoreUpdateEnd
 *****************************************************************************/
void
NodeDataStoreUpdateEnd()
{

}

/*****************************************************************************!
 * Function : NodeDataStoreFindExisting
 * Purpose  : Check to see if a node with a given node type and
 *            unit number is already initialized.
 * Note     : This function assumes the caller has locked the data store
 *            before calling this function.
 *****************************************************************************/
NodeType*
NodeDataStoreFindExisting
(
 uint8_t                                InNodeType,
 uint8_t                                InUnitNumber
)
{
  int                                 i;
    
  for (i = 1; i < NODE_DATASTORE_MAX_NODE_COUNT; i++ ) {
    if ( !Nodes[i].available ) {
      if ( InNodeType == NODE_TYPE_NONE ) {
        if ( Nodes[i].location.unitNumber == InUnitNumber ) {
          return &(Nodes[i]);
        }
      } else { 
        if ( Nodes[i].nodeType == InNodeType &&
             Nodes[i].location.unitNumber == InUnitNumber ) {
          return &(Nodes[i]);
        }
      }
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : NodeDataStoreFindNextAvailable
 * Purpose  : Find the index (address) of the next available node
 * Note     : This function assumes the caller has locked the data store
 *            before calling this function.
 *****************************************************************************/
uint16_t
NodeDataStoreFindNextAvailable
(
 uint8_t                                InNodeType
)
{
  int                                 i, j;

  if ( InNodeType == NODE_TYPE_RECT ) {
    i = 2;
    j = NODE_DATASTORE_DSM_INDEX_OFFSET;
  } else if ( InNodeType == NODE_TYPE_DSM ) {
    i = NODE_DATASTORE_DSM_INDEX_OFFSET;
    j = NODE_DATASTORE_MAX_NODE_COUNT;
  } else {
    return NODE_DATASTORE_MAX_NODE_COUNT + 1;
  }
        
  for (; i < j; i++) {
    if ( Nodes[i].available ) {
      return i;
    }
  }
  return NODE_DATASTORE_MAX_NODE_COUNT+1;
}

/*****************************************************************************!
 * Function : NodeDataStoreGetCount
 *****************************************************************************/
uint16_t
NodeDataStoreGetCount()
{
  int                                 i;
  uint16_t                            count;

  count = 0;
  for (i = 1; i < NODE_DATASTORE_MAX_NODE_COUNT; i++) {
    if ( Nodes[i].nodeType != NODE_TYPE_NONE ) {
      count++;
    }
  }
  return count;
}

/*****************************************************************************!
 * Function : NodeDataStoreGetFirstNode
 *****************************************************************************/
NodeType*
NodeDataStoreGetFirstNode()
{
  int                                 i;

  for (i = 0; i < NODE_DATASTORE_MAX_NODE_COUNT; i++) {
    if ( !Nodes[i].available ) {
      return &Nodes[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : NodeDataStoreGetFirstTypedNode
 *****************************************************************************/
NodeType*
NodeDataStoreGetFirstTypedNode
(
 uint8_t                                InNodeType
)
{
  int                                 i;

  for (i = 0; i < NODE_DATASTORE_MAX_NODE_COUNT; i++) {
    if ( !Nodes[i].available && Nodes[i].nodeType == InNodeType ) {
      return &Nodes[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : NodeDataStoreGetNextNode
 * Purpose  : Return the first node of a given type (e.g., rectifier or dsm)
 *            or NULL if not found.
 *****************************************************************************/
NodeType*
NodeDataStoreGetNextNode
(
 NodeType*                              InNode
)
{
  int                                 i;
    
  if ( NULL == InNode ) {
    return NULL;
  }

  for (i = 0; i < NODE_DATASTORE_MAX_NODE_COUNT; i++ ) {
    if ( InNode == &(Nodes[i]) ) {
      break;
    }

  }
                
  for (i++; i < NODE_DATASTORE_MAX_NODE_COUNT; i++) {
    if ( !Nodes[i].available ) {
      return &Nodes[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : NodeDataStoreGetNextTypedNode
 * Purpose  : Return the next with same type as InNode or NULL if not found
 *****************************************************************************/
NodeType*
NodeDataStoreGetNextTypedNode
(
 NodeType*                              InNode
)
{
  int                                 i;
    
  if ( NULL == InNode ) {
    return NULL;
  }

  // Skip to the node where we want to start searching
  for (i = 0; i < NODE_DATASTORE_MAX_NODE_COUNT; i++) {
    if ( &Nodes[i] == InNode ) {
      break;
    }  
  }
  
  if ( i == NODE_DATASTORE_MAX_NODE_COUNT ) {
    // We found no nodes of this type -- shouldn't happen but be prepared for
    // when it does
    return NULL;
  }
  
  // Advance to next node and keep on looking for a node of a given type
  for (i++; i < NODE_DATASTORE_MAX_NODE_COUNT; i++) {
    if (Nodes[i].nodeType == InNode->nodeType ) {
      // Found one 
      return &Nodes[i];
    }
  }
  
  // There is no more nodes of the requested type
  return NULL;
}

/*****************************************************************************!
 * Function : NodeDataStoreCount
 *****************************************************************************/
int
NodeDataStoreCount()
{
  int                                 nodeCount;
  int					i;

  for ( i = 0 ; i < NODE_DATASTORE_MAX_NODE_COUNT ; i++ ) {
    if ( !Nodes[i].available && Nodes[i].nodeType != NODE_TYPE_NONE ) {
      nodeCount++;
    }
  }
  return nodeCount;
}

/*****************************************************************************!
 * Function : NodeDataStoreSetSystemUnit
 *****************************************************************************/
void
NodeDataStoreSetSystemUnit()
{
  NodeType*                           systemUnitNode;

  NodeDataStoreUpdateStart();
  systemUnitNode = &(Nodes[0]);
  memset(systemUnitNode, 0x00, sizeof(NodeType));
  systemUnitNode->nodeType = NODE_TYPE_SYSTEM_UNIT;
  NodeDataStoreUpdateEnd();
}

/*****************************************************************************!
 * Function : NodeDataStoreGetSystemUnit
 *****************************************************************************/
NodeType*
NodeDataStoreGetSystemUnit()
{
  int						i;
  for ( i = 0 ; i < NODE_DATASTORE_MAX_NODE_COUNT ; i++ ) {
    if ( Nodes[i].nodeType != NODE_TYPE_SYSTEM_UNIT ) {
      return &Nodes[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : NodeDataStoreNodeExists
 *****************************************************************************/
bool
NodeDataStoreNodeExists
(
 uint8_t                                InNodeType,
 uint8_t                                InUnitNumber
)
{
  return NodeDataStoreFindExisting(InNodeType, InUnitNumber) != NULL;
}

/*****************************************************************************!
 * Function : NodeDataStoreReserveNode
 *****************************************************************************/
bool
NodeDataStoreReserveNode
(
 uint8_t                                InNodeType,
 uint8_t                                InUnitNumber
)
{
  NodeType*                           node;
  node = NodeDataStoreNodeInit(InNodeType, InUnitNumber);
  node->available = false;
  node->ESNACANAddress = 0;
  return true;
}
    
/*****************************************************************************!
 * Function : NodeDataStoreFindExistingInBay
 *****************************************************************************/
NodeType*
NodeDataStoreFindExistingInBay
(
 uint8_t                                InNodeType,
 uint8_t                                InBayIndex,
 uint8_t                                InUnitNumber
)
{
  NodeType*                             node;

  for ( node = NodeDataStoreGetFirstTypedNode(InNodeType);
        node;
        node = NodeDataStoreGetNextTypedNode(node) ) {
    if ( node->bay->index == InBayIndex &&
         node->location.unitNumber == InUnitNumber ) {
      return node;
    }
  }
  return NULL;
}

/******************************************************************************!
 * Function : NodeDataStoreFindNodeByAddress
 ******************************************************************************/
NodeType*
NodeDataStoreFindNodeByAddress
(uint16_t InESNAAddress)
{
  NodeType*                             node;

  for ( node = NodeDataStoreGetFirstNode(InESNAAddress);
        node;
        node = NodeDataStoreGetNextNode(node) ) {
    if ( node->ESNACANAddress == InESNAAddress ) {
      return node;
    }
  }
  return NULL;
}
