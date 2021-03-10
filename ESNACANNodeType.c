/*****************************************************************************
 FILE NAME      : ESNACANNodeType.c
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Company
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ESNACANNodeType.h"
#include "NodeDataStore.h"
#include "BayType.h"
#include "DSMType.h"
#include "main.h"
#include "RectifierType.h"

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
uint16_t                DefaultCurrentCapacity = 200;

NodeRevisionInfo
DefaultDSMRevision = {
    1, 0, 0, 0, 1, 1, 0, 0
};

NodeRevisionInfo
DefaultRectifierRevision = {
    1, 2, 0, 0, 1, 3, 0, 0
};

/*****************************************************************************!
 * Function : ESNANodeRectifierAdd
 *****************************************************************************/
ESNANodeStatus
ESNANodeRectifierAdd
(
 uint8_t                                InBayIndex,
 uint8_t                                InUnitNumber
)
{
    NodeType*                           node;
    Bay*                                bay;
    
    if ( InUnitNumber == 0 ) {
        return ESNA_NODE_STATUS_MISSING_UNIT_NUMBER;
    }

    if ( NodeDataStoreNodeExists(NODE_TYPE_RECT, InUnitNumber) ) {
        return ESNA_NODE_STATUS_DUPLICATE_NODE;
    }

    node = NodeDataStoreNodeInit(NODE_TYPE_RECT, InUnitNumber);
    node->nodeNumber = 0;
    node->ESNACANAddress = 0;
    node->nodeTimedOut = true;
    node->serialNumber = GetNextSerialNumber();
    node->rectifier.maxCurrentCapacity = DefaultCurrentCapacity * 1.1;
    node->rectifier.parentNode = node;
    node->revisionInfo = DefaultRectifierRevision;
    strcpy((string)node->partNumber, RECTIFIER_DEFAULT_PART_NUMBER); 
    bay = BaySetFindBayByIndex(mainBays, InBayIndex);
    if ( bay ) {
        BayAddRectifier(bay, node);
    }

    return ESNA_NODE_STATUS_OK;
}

/*****************************************************************************!
 * Function : ESNANodeDSMAdd
 *****************************************************************************/
ESNANodeStatus
ESNANodeDSMAdd
(
 uint8_t                                InUnitNumber,
 uint16_t                               InDescription
)
{
    NodeType*                           node;
    Bay*                                bay;

    if ( NodeDataStoreNodeExists(NODE_TYPE_DSM, InUnitNumber) ) {
        return ESNA_NODE_STATUS_DUPLICATE_NODE;
    }

    node = NodeDataStoreNodeInit(NODE_TYPE_DSM, InUnitNumber);
    node->dsm.description.whole = InDescription;
    node->dsm.updateState = 0;
    node->dsm.dsmUpdateState = DSM_UPDATE_STATE_1;
    node->serialNumber = GetNextSerialNumber();
    node->dsm.parentNode = node;
    node->ESNACANAddress = 0;
    node->nodeTimedOut = false;
    node->revisionInfo = DefaultDSMRevision;
    bay = BaySetFindBayByIndex(mainBays, 1);
    if ( bay ) {
        BayAddDSM(bay, &(node->dsm));
    }

    return ESNA_NODE_STATUS_OK;
}

/*****************************************************************************!
 * Function : ESNANodeSetAppVersion
 *****************************************************************************/
ESNANodeStatus
ESNANodeSetAppVersion
(
 uint8_t                                InUnitNumber,
 uint8_t                                InMajorVersion,
 uint8_t                                InMinorVersion,
 uint8_t                                InBugFixVersion,
 uint8_t                                InBuildVersion
)
{
    NodeType*                           node;
    
    if ( InUnitNumber == 0 ) {
        return ESNA_NODE_STATUS_MISSING_UNIT_NUMBER;
    }
    
    if ( !NodeDataStoreNodeExists(NODE_TYPE_NONE, InUnitNumber) ) {
        return ESNA_NODE_STATUS_MISSING_NODE;
    }
    
    node = NodeDataStoreFindExisting(NODE_TYPE_NONE, InUnitNumber);
    node->revisionInfo.appMajor = InMajorVersion;
    node->revisionInfo.appMinor = InMinorVersion;
    node->revisionInfo.appBugFix = InBugFixVersion;
    node->revisionInfo.appBuild = InBuildVersion;
    return ESNA_NODE_STATUS_OK;
}

 
/*****************************************************************************!
 * Function : ESNANodeSetBootVersion
 *****************************************************************************/
ESNANodeStatus
ESNANodeSetBootVersion
(
 uint8_t                                InUnitNumber,
 uint8_t                                InMajorVersion,
 uint8_t                                InMinorVersion,
 uint8_t                                InBugFixVersion,
 uint8_t                                InBuildVersion
)
{
    NodeType*                           node;
    
    if ( InUnitNumber == 0 ) {
        return ESNA_NODE_STATUS_MISSING_UNIT_NUMBER;
    }

     if ( !NodeDataStoreNodeExists(NODE_TYPE_NONE, InUnitNumber) ) {
         return ESNA_NODE_STATUS_MISSING_NODE;
     }

     node = NodeDataStoreFindExisting(NODE_TYPE_NONE, InUnitNumber);
     node->revisionInfo.bootMajor = InMajorVersion;
     node->revisionInfo.bootMinor = InMinorVersion;
     node->revisionInfo.bootBugFix = InBugFixVersion;
     node->revisionInfo.bootBuild = InBuildVersion;
     return ESNA_NODE_STATUS_OK;
}

 
