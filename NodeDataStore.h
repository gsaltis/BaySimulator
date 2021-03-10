/*****************************************************************************
 FILE NAME      : NodeDataStore.h
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Company
******************************************************************************/
#ifndef _nodedatastore_h_
#define _nodedatastore_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdbool.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "NodeType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define NODE_DATASTORE_MAX_NODE_COUNT             255

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
NodeDataStoreInit();

NodeType*
NodeDataStoreGetNode
(uint16_t InNodeNum);

NodeType*
NodeDataStoreNodeInit
(uint8_t InNodeType, uint8_t InUnitNumber);

void
NodeDataStoreNodeClear
(uint16_t InNodeNum);

bool
NodeDataStoreUpdateStart();

void
NodeDataStoreUpdateEnd();

uint16_t
NodeDataStoreGetCount();

NodeType*
NodeDataStoreGetFirstNode();

NodeType*
NodeDataStoreGetFirstTypedNode(uint8_t InNodeType);

NodeType*
NodeDataStoreGetNextNode
(NodeType* InNode);

NodeType*
NodeDataStoreGetNextTypedNode
(NodeType* InNode);

int
NodeDataStoreCount();

void
NodeDataStoreSetSystemUnit();

NodeType*
NodeDataStoreGetSystemUnit();

NodeType*
NodeDataStoreFindNextAvailableNode
(uint8_t InNodeType);

bool
NodeDataStoreNodeExists
(uint8_t InNodeType, uint8_t InUnitNumber);

NodeType*
NodeDataStoreFindExisting
(uint8_t InNodeType, uint8_t InUnitNumber);

NodeType*
NodeDataStoreFindExistingInBay
(uint8_t InNodeType, uint8_t InBayIndex, uint8_t InUnitNumber);

NodeType*
NodeDataStoreFindNodeByAddress
(uint16_t InESNAAddress);

#endif /* _nodedatastore_h_*/
