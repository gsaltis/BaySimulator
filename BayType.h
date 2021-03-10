/*****************************************************************************
 * FILE NAME    : BayType.h
 * DATE         : February 04 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _baytype_h_
#define _baytype_h_

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
#include "DSMType.h"
#include "RectifierType.h"
#include "NodeType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define BAY_MAX_RECTIFIER_COUNT 10
#define BAY_MAX_DSM_COUNT       4
#define BAY_MAX_COUNT           1

/*****************************************************************************!
 * Exported Type : BayType
 *****************************************************************************/
enum _BayType
{
    BayTypeNone                 = 0,
    BayTypeMain,
    BayTypeSupplemental,
    BayTypeDistribution
};
typedef enum _BayType BayType;

/*****************************************************************************!
 * Exported Type
 *****************************************************************************/
struct _Bay
{
    string                      name;
    BayType                     type;
    uint8_t                     index;
    RectifierType*              rectifiers[BAY_MAX_RECTIFIER_COUNT];
    DSMType*                    dsms[BAY_MAX_DSM_COUNT];
    uint8_t                     maxRectifierCount;
    uint8_t                     maxDSMCount;
};
typedef struct _Bay Bay;

/*****************************************************************************!
 * Exported Type : BaySet
 *****************************************************************************/
struct _BaySet
{
    Bay*                        bays[BAY_MAX_COUNT];
    uint8_t                     bayCount;
};
typedef struct _BaySet BaySet;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
Bay*
BaySetGetFirstBay
(BaySet* InBaySet);

Bay*
CreateBay
(string InName, BayType InType, uint8_t InIndex);

void
BayAddRectifier
(Bay* InBay, NodeType* InNode);

void
BaySetAllRectifierCurrents
(Bay* InBay, float InCurrent);

void
BaySetAllRectifiersOn
(Bay* InBay);

void
BaySetAllRectifiersOff
(Bay* InBay);

void
ClearBay
(Bay* InBay);

bool
BaySaveFile
(Bay* InBay, string InFilename);

void
BayAddDSM
(Bay* InBay, DSMType* InDSM);

RectifierType*
BayGetRectifier
(Bay* InBay, uint8_t InIndex);

void
BaySetName
(Bay* InBay, string InName);

DSMType*
BayGetDSM
(Bay* InBay, uint8_t InIndex);

void
BayRemoveRectifier
(Bay* InBay, RectifierType* InRectifier);

void
BayRemoveRectifierByIndex
(Bay* InBay, uint8_t InIndex);

void
BayRemoveDSM
(Bay* InBay, DSMType* InDSM);

void
BayRemoveDSMByIndex
(Bay* InBay, uint8_t InIndex);

//!
BaySet*
CreateBaySet
();

void
BaySetAddBay
(BaySet* InBaySet, Bay* InBay);

Bay*
BaySetFindBayByIndex
(BaySet* InBaySet, uint8_t InIndex);

string
BayTypeToString
(BayType InType);

RectifierType*
BayFindRectifier
(Bay* InBay, uint8_t InUnit);

DSMType*
BayFindDSM
(Bay* InBay, uint8_t InUnit);

bool
BayAllNodesTimedOut
(Bay* InBay);

bool
BayAnyNodeActive
(Bay* InBay);

bool
BayAnyNodeCommunicating
(Bay* InBay);

//!
int
BayGetRectifierCount
(Bay* InBay);

RectifierType*
BayGetFirstRectifier
(Bay* InBay);

RectifierType*
BayGetNextRectifier
(Bay* InBay, RectifierType* InRectifier);

//!
int
BayGetDSMCount
(Bay* InBay);

DSMType*
BayGetNextDSM
(Bay* InBay, DSMType* InDSM);

DSMType*
BayGetFirstDSM
(Bay* InBay);

//!
bool
BayRectifierExistsAtSlot
(Bay* InBay, uint16_t InNodeNumber);

#endif /* _baytype_h_*/
