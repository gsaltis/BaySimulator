/*****************************************************************************
 * FILE NAME    : ESNACANMessageQueue.h
 * DATE         : January 21 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _esnacanmessagequeue_h_
#define _esnacanmessagequeue_h_

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
#include "ESNACANInterface.h"
#include "Mutex.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define ESNA_CAN_MESSAGE_SIZE           40

/*****************************************************************************!
 * Exported Type : ESNACANMessageQueue
 *****************************************************************************/
typedef struct _ESNACANMessageQueue
{
    ESNACANMessage                      message[ESNA_CAN_MESSAGE_SIZE];
    int8_t                              messageIndex;
    Mutex                               queueMutex;
} ESNACANMessageQueue;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
ESNACANMessageQueue*
ESNACANMessageQueueCreate
(string InName);

bool
ESNACANMessageQueueEnqueue
(ESNACANMessageQueue* InQueue, ESNACANMessage* InMessage);

bool
ESNACANMessageQueueDequeue
(ESNACANMessageQueue* InQueue, ESNACANMessage* InMessage);

bool
ESNACANMessageQueueIsEmpty
(ESNACANMessageQueue* InQueue);

bool
ESNACANMessageQueueEnqueueParts
(ESNACANMessageQueue* InQueue, ESNACANID InId, uint8_t InLength, ESNACANData InDataLength);

#endif /* _esnacanmessagequeue_h_*/
