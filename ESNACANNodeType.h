/*****************************************************************************
 FILE NAME      : ESNACANNodeType.h
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Company
******************************************************************************/
#ifndef _esnacannodetype_h_
#define _esnacannodetype_h_

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "DSMType.h"
#include "RectifierType.h"

/*****************************************************************************!
 * Exported Type : ESNANodeStatus
 *****************************************************************************/
typedef enum {
    ESNA_NODE_STATUS_OK                         = 0,
    ESNA_NODE_STATUS_MISSING_UNIT_NUMBER,
    ESNA_NODE_STATUS_INVALID_CURRENT_CAPACITY,
    ESNA_NODE_STATUS_DUPLICATE_NODE,
    ESNA_NODE_STATUS_MISSING_NODE
} ESNANodeStatus;

/*****************************************************************************!
 * Exported Type : ESNACANNodeType
 *****************************************************************************/
typedef union
{
    RectifierType                       rectifier;
    DSMType                             dsm;
} ESNACANNodeType;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
ESNANodeStatus
ESNANodeRectifierAdd
(uint8_t InBayIndex, uint8_t InUnitNumber);

ESNANodeStatus
ESNANodeDSMAdd
(uint8_t InBayIndex, uint16_t InDescription);

#endif /* _esnacannodetype_h_*/
