/*****************************************************************************
 * FILE NAME    : BayType.c
 * DATE         : February 04 2019
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
#include <StringUtils.h>
#include <MemoryManager.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "BayType.h"
#include "NodeType.h"
#include "RectifierType.h"
#include "CLIThread.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Function : CreateBay
 *****************************************************************************/
Bay*
CreateBay
(
 string                         InName,
 BayType                        InType,
 uint8_t                        InIndex
)
{
    Bay*                        bay;

    bay = (Bay*)GetMemory(sizeof(Bay));
    memset(bay, 0x00, sizeof(Bay));
    bay->name = StringCopy(InName);
    bay->type = InType;
    bay->index = InIndex;
    switch (InType) {
        case BayTypeMain : {
            bay->maxRectifierCount = BAY_MAX_RECTIFIER_COUNT;
            bay->maxDSMCount = 2;
            break;
        }
        case BayTypeSupplemental : {
            bay->maxRectifierCount = BAY_MAX_RECTIFIER_COUNT;
            bay->maxDSMCount = 2;
            break;
        }
        case BayTypeDistribution : {
            bay->maxRectifierCount = 0;
            bay->maxDSMCount = 4;
            break;
        }
        default : {
            break;
        }
    }
    return bay;
}

/*****************************************************************************!
 * Function : BayAddRectifier
 *****************************************************************************/
void
BayAddRectifier
(
 Bay*                           InBay,
 NodeType*                      InNode
)
{
  int                                 i;
  if ( NULL == InBay || NULL == InNode ) {
    return;
  }
  if ( InBay->maxRectifierCount == 0 ) {
    return;
  }

  i = InNode->location.unitNumber - 1;
  InBay->rectifiers[i] = &(InNode->rectifier);
  NodeSetBay(InNode, InBay);
}

/*****************************************************************************!
 * Function : BayAddDSM
 *****************************************************************************/
void
BayAddDSM
(
 Bay*                                   InBay,
 DSMType*                               InDSM
)
{
  int                                   i;
  if ( NULL == InBay || NULL == InDSM ) {
    return;
  }
  i = InDSM->parentNode->location.unitNumber - 1;
  if ( i > InBay->maxDSMCount ) {
    return;
  }
  InBay->dsms[i] = InDSM;
  NodeSetBay(InDSM->parentNode, InBay);
}

/*****************************************************************************!
 * Function : BayGetRectifier
 *****************************************************************************/
RectifierType*
BayGetRectifier
(
 Bay*                           InBay,
 uint8_t                        InIndex
)
{
    if ( NULL == InBay ) {
        return NULL;
    }
    if ( InBay->maxRectifierCount == 0 ) {
        return NULL;
    }
    return InBay->rectifiers[InIndex];
}

/*****************************************************************************!
 * Function : BayGetDSM
 *****************************************************************************/
DSMType*
BayGetDSM
(
 Bay*                           InBay,
 uint8_t                        InIndex
)
{
    if ( NULL == InBay ) {
        return NULL;
    }

    if ( InBay->maxDSMCount == 0 ) {
        return NULL;
    }

    return InBay->dsms[InIndex-1];
}

/*****************************************************************************!
 * Function : BayRemoveRectifier
 *****************************************************************************/
void
BayRemoveRectifier
(
 Bay*                           InBay,
 RectifierType*                 InRectifier
)
{
  int16_t                       i;
  NodeType*                     node;
  
  if ( NULL == InBay || NULL == InRectifier ) {
    return ;
  }

  node = InRectifier->parentNode;
  i = node->location.unitNumber - 1;
  NodeClear(node);
  InBay->rectifiers[i] = NULL;
}
 
/*****************************************************************************!
 * Function : BayRemoveRectifierByIndex
 *****************************************************************************/
void
BayRemoveRectifierByIndex
(
 Bay*                           InBay,
 uint8_t                        InIndex
)
{
    if ( InBay == NULL ) {
        return;
    }
    (void)InIndex;
}

/*****************************************************************************!
 * Function : BayRemoveDSM
 *****************************************************************************/
void
BayRemoveDSM
(
 Bay*                           InBay,
 DSMType*                       InDSM
)
{
  if ( NULL == InBay || NULL == InDSM ) {
    return;
  }
  if ( InDSM == InBay->dsms[0] ) {
    NodeClear(InBay->dsms[0]->parentNode);
    InBay->dsms[0] = NULL;
    
  } else if ( InDSM == InBay->dsms[1] ) {
    NodeClear(InBay->dsms[1]->parentNode);
    InBay->dsms[1] = NULL;
  }
}

/*****************************************************************************!
 * Function : BayRemoveDSMByIndex
 *****************************************************************************/
void
BayRemoveDSMByIndex
(
 Bay*                           InBay,
 uint8_t                        InIndex
)
{
    if ( NULL == InBay ) {
        return;
    }
    (void)InIndex;
}

/*****************************************************************************!
 * Function : BaySetName
 *****************************************************************************/
void
BaySetName
(Bay* InBay, string InName)
{
  if ( NULL == InBay || NULL == InName ) {
    return;
  }

  if ( InBay->name ) {
    FreeMemory(InBay->name);
  }
  InBay->name = StringCopy(InName);
}

    
/*****************************************************************************!
 * Function : CreateBaySet
 *****************************************************************************/
BaySet*
CreateBaySet
()
{
    BaySet*                     baySet;
    int                         i;
    baySet = (BaySet*)GetMemory(sizeof(BaySet));
    baySet->bayCount = 0;
    for (i = 0; i < baySet->bayCount; i++) {
        baySet->bays[i] = NULL;
    }
    return baySet;
}

/*****************************************************************************!
 * Function : BaySetAddBay
 *****************************************************************************/
void
BaySetAddBay
(
 BaySet*                        InBaySet,
 Bay*                           InBay
)
{
    if ( NULL == InBaySet || NULL == InBay ) {
        return;
    }
    if ( InBaySet->bayCount >= BAY_MAX_COUNT ) {
        return;
    }
    InBaySet->bays[InBaySet->bayCount] = InBay;
    InBaySet->bayCount++;
}

/*****************************************************************************!
 * Function : BaySetFindBayByIndex
 * Purpose  : Return the bay with a given index
 *****************************************************************************/
Bay*
BaySetFindBayByIndex
(
 BaySet*                        InBaySet,
 uint8_t                        InIndex
)
{
  int                           i;
  if ( NULL == InBaySet ) {
    return NULL;
  }
  for (i = 0; i < InBaySet->bayCount; i++) {
    if ( InBaySet->bays[i]->index == InIndex ) {
      return InBaySet->bays[i];
    }
  }
  return NULL;
}


/*****************************************************************************!
 * Function : BaySetGetFirstBay
 * Purpose  : Return the first bay in a list
 *****************************************************************************/
Bay*
BaySetGetFirstBay
(BaySet* InBaySet)
{
  if ( NULL == InBaySet ) {
    return NULL;
  }
  if ( InBaySet->bayCount == 0 ) {
    return NULL;
  }
  return InBaySet->bays[0];
}

/******************************************************************************!
 * Function : BayTypeToString
 ******************************************************************************/
string
BayTypeToString
(
 BayType                        InType
)
{
  switch(InType) {
    case BayTypeNone : {
      return "None";
    }
    case BayTypeMain : {
      return "Main";
    }
    case BayTypeSupplemental : {
      return "Supplemental";
    }
    case BayTypeDistribution : {
      return "Distribution";
    }
  }
  return "None";
}

/******************************************************************************!
 * Function : BayFindRectifier
 ******************************************************************************/
RectifierType*
BayFindRectifier
(
 Bay*                           InBay,
 uint8_t                        InUnit
)
{
  RectifierType*                rect;
 
  if ( NULL == InBay ) {
    return NULL;
  }
  for ( rect = BayGetFirstRectifier(InBay); rect;
        rect = BayGetNextRectifier(InBay, rect) ) {
    NodeType* node = rect->parentNode;
    if ( node->location.unitNumber == InUnit ) {
      return rect;
    }
  }
  return NULL;
}

/******************************************************************************!
 * Function : BayFindDSM
 ******************************************************************************/
DSMType*
BayFindDSM
(
 Bay*                           InBay,
 uint8_t                        InUnit
)
{
  DSMType*                      dsm;
  int                           i;

  if ( NULL == InBay ) {
    return NULL;
  }
  for (i = 0; i < InBay->maxDSMCount; i++) {
    dsm = InBay->dsms[i];
    if ( dsm ) {
      NodeType* node = dsm->parentNode;
      if ( node->location.unitNumber == InUnit ) {
        return dsm;
      }
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : ClearBay
 *****************************************************************************/
void
ClearBay
(Bay* InBay)
{
  int                                   i;
  if ( NULL == InBay ) {
    return;
  }

  for (i = 0; i < InBay->maxDSMCount; i++) {
    if ( InBay->dsms[i] ) {
      NodeClear(InBay->dsms[i]->parentNode);
    }
    InBay->dsms[i] = NULL;
  }

  for (i = 0; i < InBay->maxRectifierCount; i++) {
    if ( InBay->rectifiers[i] ) {
      NodeClear(InBay->rectifiers[i]->parentNode);
    }
    InBay->rectifiers[i] = NULL;
  }
}


/*****************************************************************************!
 * Function : SaveBayFile
 *****************************************************************************/
bool
BaySaveFile
(Bay* InBay, string InFilename)
{
  (void)InBay;
  (void)InFilename;
  return true;
}

/******************************************************************************!
 * Function : BayAnyNodeActive
 ******************************************************************************/
bool
BayAnyNodeActive
(Bay* InBay)
{
  RectifierType*                        rectifier;
  DSMType*                              dsm;
  
  if ( InBay == NULL ) {
    return false;
  }
  rectifier = BayGetFirstRectifier(InBay);
  if ( rectifier ) {
    return true;
  }
  dsm = BayGetFirstDSM(InBay);
  if ( dsm ) {
    return true;
  }
  return false;
}

/******************************************************************************!
 * Function : BayAnyNodeCommunicating
 ******************************************************************************/
bool
BayAnyNodeCommunicating
(Bay* InBay)
{
  RectifierType*                rect;
  DSMType*                      dsm;
  
  if ( !BayAnyNodeActive(InBay) ) {
    return false;
  }

  for ( rect = BayGetFirstRectifier(InBay); rect; rect = BayGetNextRectifier(InBay, rect) ) {
    if ( NodeTypeIsCommunicating(rect->parentNode) ) {
      return true;
    }
  }

  for ( dsm = BayGetFirstDSM(InBay); dsm; dsm = BayGetNextDSM(InBay, dsm) ) {
    if ( NodeTypeIsCommunicating(dsm->parentNode) ) {
      return true;
    }
  }

  return false;
}

/******************************************************************************!
 * Function : BayAllNodesTimedOut
 ******************************************************************************/
bool
BayAllNodesTimedOut
(Bay* InBay)
{
  RectifierType*                        rect;
  DSMType*                              dsm;
  
  if ( !BayAnyNodeActive(InBay) ) {
    return false;
  }

  for (rect = BayGetFirstRectifier(InBay); rect; rect = BayGetNextRectifier(InBay, rect)) {
    if ( !rect->parentNode->nodeTimedOut ) {
      return false;
    }
  }


  for ( dsm = BayGetFirstDSM(InBay); dsm; dsm = BayGetNextDSM(InBay, dsm) ) {
    if ( !dsm->parentNode->nodeTimedOut ) {
      return false;
    }
  }

  return true;
}

/*****************************************************************************!
 * Function : BaySetAllRectifierCurrents
 *****************************************************************************/
void
BaySetAllRectifierCurrents
(Bay* InBay, float InCurrent)
{
  RectifierType*                        rect;
  
  if ( NULL == InBay ) {
    return;
  }

  for ( rect = BayGetFirstRectifier(InBay); rect; rect = BayGetNextRectifier(InBay, rect) ) {
    rect->outputCurrent = InCurrent;
  }
}

/*****************************************************************************!
 * 
 *****************************************************************************/
void
BaySetAllRectifiersOn
(Bay* InBay)
{
  RectifierType*                        rect;

  if ( NULL == InBay ) {
    return;
  }

  for ( rect = BayGetFirstRectifier(InBay); rect; rect = BayGetNextRectifier(InBay, rect) ) {
    RectifierTurnOn(rect);
    rect->parentNode->ESNACANAddress = 0;
  }
}  

/*****************************************************************************!
 * 
 *****************************************************************************/
void
BaySetAllRectifiersOff
(Bay* InBay)
{
  RectifierType*                        rect;
  
  if ( NULL == InBay ) {
    return;
  }

  for ( rect = BayGetFirstRectifier(InBay); rect; rect = BayGetNextRectifier(InBay, rect) ) {
    RectifierTurnOff(rect);
    rect->parentNode->startUpState = NodeStartUpNeedAddress;
    rect->parentNode->ESNACANAddress = 0;
  }
}  


/*****************************************************************************!
 * Function : BayGetFirstRectifier
 *****************************************************************************/
RectifierType*
BayGetFirstRectifier
(Bay* InBay)
{
  int i;
  
  if ( NULL == InBay ) {
    return NULL;
  }

  for (i = 0; i < BAY_MAX_RECTIFIER_COUNT; i++) {
    if ( InBay->rectifiers[i] ) {
      return InBay->rectifiers[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : BayGetNextRectifier
 *****************************************************************************/
RectifierType*
BayGetNextRectifier
(Bay* InBay, RectifierType* InRectifier)
{
  int i;
  
  if ( NULL == InBay || NULL == InRectifier ) {
    return NULL;
  }

  for (i = 0; i < BAY_MAX_RECTIFIER_COUNT; i++) {
    if ( InBay->rectifiers[i] == InRectifier ) {
      break;
    }

  }
  if ( i == BAY_MAX_RECTIFIER_COUNT ) {
    return NULL;
  }

  i++;

  for ( ; i < BAY_MAX_RECTIFIER_COUNT; i++ ) {
    if ( InBay->rectifiers[i] ) {
      return InBay->rectifiers[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Fundtion : BayGetRectifierCount
 *****************************************************************************/
int
BayGetRectifierCount
(Bay* InBay)
{
  RectifierType*                        rect;
  int                                   count = 0;

  for ( rect = BayGetFirstRectifier(InBay); rect; rect = BayGetNextRectifier(InBay, rect) ) {
    count++;
  }

  return count;
}

/*****************************************************************************!
 * Function : BayRectifierExistsAtSlot
 *****************************************************************************/
bool
BayRectifierExistsAtSlot
(Bay* InBay, uint16_t InNodeNumber)
{
  int                                   n = InNodeNumber - 1;
  if ( n < 0 || n >= BAY_MAX_RECTIFIER_COUNT ) {
    return false;
  }
  if ( InBay->rectifiers[n]  ) {
    return true;
  }
  return false;
}
 
/*****************************************************************************!
 * Function : BayGetDSMCount
 *****************************************************************************/
int
BayGetDSMCount
(Bay* InBay)
{
  DSMType*                              dsm;
  int                                   count = 0;

  for ( dsm = BayGetFirstDSM(InBay); dsm; dsm = BayGetNextDSM(InBay, dsm) ) {
    count++;
  }

  return count;
}

/*****************************************************************************!
 * Function : BayGetNextDSM
 *****************************************************************************/
DSMType*
BayGetNextDSM
(Bay* InBay, DSMType* InDSM)
{
  int i;
  
  if ( NULL == InBay || NULL == InDSM ) {
    return NULL;
  }

  for (i = 0; i < InBay->maxDSMCount; i++) {
    if ( InBay->dsms[i] == InDSM ) {
      break;
    }

  }
  if ( i == InBay->maxDSMCount ) {
    return NULL;
  }

  i++;

  for ( ; i < InBay->maxDSMCount; i++ ) {
    if ( InBay->dsms[i] ) {
      return InBay->dsms[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : BayGetFirstDSM
 *****************************************************************************/
DSMType*
BayGetFirstDSM
(Bay* InBay)
{
  int i;
  
  if ( NULL == InBay ) {
    return NULL;
  }

  for (i = 0; i < InBay->maxDSMCount; i++) {
    if ( InBay->dsms[i] ) {
      return InBay->dsms[i];
    }
  }
  return NULL;
}


