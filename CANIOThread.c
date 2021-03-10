/*****************************************************************************
 * FILE NAME    : CANIOThread.c
 * DATE         : January 17 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#define _POSIX_C_SOURCE 200809L

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ascii.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "CANInterface.h"
#include "ESNACANInterface.h"
#include "CANIOThread.h"
#include "NodeType.h"
#include "NodeDataStore.h"
#include "HTTPServerThread.h"
#include "CANIOThread.h"
#include "SystemTime.h"
#include "main.h"
#include "CANInterface.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Imported Data
 *****************************************************************************/
void
ProcessInputMessage
(ESNACANMessage* InMessage);

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
ThreadHandle
CANThreadHandle;

ESNACANMessageQueue*
CANReadQueue = NULL;

bool
CANWriteDisplayPacket = false;

ESNACANMessageQueue*
CANWriteQueue = NULL;

CANInterface*
canInterface;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Function : CreateCANIOThread
 *****************************************************************************/
void
CANIOInit()
{
}

/*****************************************************************************!
 * Function : CANIOThread
 *****************************************************************************/
void*
CANIOThread
(
 void*                                  InParameters
)
{
  ESNACANMessage                        message;
  int                                   returnValue;

  canInterface = (CANInterface*)InParameters;
  while (true) {
    returnValue = CANInterfaceRead(canInterface, &message.id.dword, &message.data.qword, &message.dataLength);
    if ( returnValue == CAN_READ_OK ) {
      ProcessInputMessage(&message);
    }
    ThreadSleep(10);
  }
}

/*****************************************************************************!
 * Function : CANWriteSend
 *****************************************************************************/
void
CANWriteSend
(
 ESNACANMessage*                InMessage
)
{
  if ( NULL == InMessage ) {
    return;
  }
  CANInterfaceWrite(canInterface, InMessage->id.dword, InMessage->data.qword, InMessage->dataLength);
}

/*****************************************************************************!
 * Function : CANIOThreadStop
 *****************************************************************************/
void
CANIOThreadStop
()
{
  CANInterfaceClose(canInterface);
  pthread_cancel(canReadThread);
}

/*****************************************************************************!
 * Function : CANIOThreadRestart
 *****************************************************************************/
void
CANIOThreadRestart
()
{
  CANInterface*                 interface;

  CANIOThreadStop();
  CANInterfaceReset();
  interface = CANInterfaceInit(MAIN_CAN_INTERFACE_NAME);
  pthread_create(&canReadThread, NULL, CANIOThread, interface);
}
 
