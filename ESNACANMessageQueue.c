/*****************************************************************************
 * FILE NAME    : ESNACANMessageQueue.c
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
#include <MemoryManager.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ESNACANMessageQueue.h"
#include "SystemTime.h"

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
 * Function : ESNACANMessageQueueCreate
 *****************************************************************************/
ESNACANMessageQueue*
ESNACANMessageQueueCreate
(
 string                                 InName
)
{
  ESNACANMessageQueue*                queue;
    
  queue = (ESNACANMessageQueue*)GetMemory(sizeof(ESNACANMessageQueue));
  queue->messageIndex = -1;
  queue->queueMutex = MutexCreate(InName);

  return queue;
}

/*****************************************************************************!
 * Function : ESNACANMessageQueueEnqueue
 *****************************************************************************/
bool
ESNACANMessageQueueEnqueue
(
 ESNACANMessageQueue*                   InQueue,
 ESNACANMessage*                        InMessage
)
{
  if ( NULL == InQueue || NULL == InMessage ) {
    return false;
  }

  if ( InQueue->messageIndex >= ESNA_CAN_MESSAGE_SIZE ) {
    return false;
  }

  MutexGet(InQueue->queueMutex);
  InQueue->messageIndex++;
  InQueue->message[InQueue->messageIndex].id = InMessage->id;
  InQueue->message[InQueue->messageIndex].data = InMessage->data;
  InQueue->message[InQueue->messageIndex].dataLength = InMessage->dataLength;
  MutexRelease(InQueue->queueMutex);
  return true;
}

/*****************************************************************************!
 * Function : ESNACANMessageQueueEnqueueParts
 *****************************************************************************/
bool
ESNACANMessageQueueEnqueueParts
(
 ESNACANMessageQueue*                   InQueue,
 ESNACANID                              InId,
 uint8_t                                InDataLength,
 ESNACANData                            InData
)
{
  if ( NULL == InQueue ) {
    return false;
  }

  if ( InQueue->messageIndex >= ESNA_CAN_MESSAGE_SIZE ) {
    return false;
  }


  char st[64];
  (void)printf("%s %d : %s\n", __FILE__, __LINE__, SystemTimeGetString(st));
  MutexGet(InQueue->queueMutex);
  InQueue->messageIndex++;
  InQueue->message[InQueue->messageIndex].id = InId;
  InQueue->message[InQueue->messageIndex].data = InData;
  InQueue->message[InQueue->messageIndex].dataLength = InDataLength;
  MutexRelease(InQueue->queueMutex);
  (void)printf("%s %d : %s\n", __FILE__, __LINE__, SystemTimeGetString(st));
  return true;
}

/*****************************************************************************!
 * Function : ESNACANMessageQueueDequeue
 * Purpose  : Remove and return the oldest element from queue (item [0])
 * Note     : The queue is FIFO
 *****************************************************************************/
bool
ESNACANMessageQueueDequeue
(
 ESNACANMessageQueue*                   InQueue,
 ESNACANMessage*                        InMessage
)
{
  int                                   i;
  if ( NULL == InQueue || NULL == InMessage ) {
    return false;
  }
  if ( InQueue->messageIndex  < 0 ) {
    return false;
  }

  char st[64];
  (void)printf("%s %d : %s\n", __FILE__, __LINE__, SystemTimeGetString(st));
  MutexGet(InQueue->queueMutex);
  InMessage->id = InQueue->message[0].id;
  InMessage->data = InQueue->message[0].data;
  InMessage->dataLength = InQueue->message[0].dataLength;
    
  for (i = 1; i <= InQueue->messageIndex; i++) {
    InQueue->message[i-1].id = InQueue->message[i].id;  
    InQueue->message[i-1].data = InQueue->message[i].data;  
    InQueue->message[i-1].dataLength = InQueue->message[i].dataLength;  
  }
  InQueue->messageIndex--;
  MutexRelease(InQueue->queueMutex);

  (void)printf("%s %d : %s\n", __FILE__, __LINE__, SystemTimeGetString(st));
  return true;
}

/*****************************************************************************!
 * Function : ESNACANMessageQueueIsEmpty
 *****************************************************************************/
bool
ESNACANMessageQueueIsEmpty
(
 ESNACANMessageQueue*                   InQueue
)
{
  if ( NULL == InQueue ) {
    return false;
  }
  return InQueue->messageIndex < 0;
}


