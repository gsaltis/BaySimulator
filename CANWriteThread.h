/*****************************************************************************
 * FILE NAME    : CANWriteThread.h
 * DATE         : January 22 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _canwritethread_h_
#define _canwritethread_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ESNACANMessageQueue.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/
extern
ESNACANMessageQueue*
CANWriteQueue;

extern
bool
CANWriteDisplayPacket;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
CANWriteInit
();

void
CANWriteSend
(ESNACANMessage* InMessage);

void*
CANWriteThread
(void* InParameters);

void
CANSendBuffers
(unsigned char* InIDBuffer, unsigned char* InDataBuffer, uint8_t InDataBufferLen);

#endif /* _canwritethread_h_*/
