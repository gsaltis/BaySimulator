/*****************************************************************************
 * FILE NAME    : CANInterface.c
 * DATE         : January 15 2019
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "CANInterface.h"
#include "ascii.h"
#include "HTTPServerThread.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define READ_BUF_SIZE                   128
#define READ_TIMEOUT                    500

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
CANInterface*
CANInterfacePort;

string
CANInterfacePortSettings = "115200,n,8,1";

bool
CANInterfaceMonitor = false;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
CANInterface*
CANInterfaceOpenPort
(string InPortName);

void
DisplayOSErrorMessage
(string InString);

void
CANInterfaceHandleMessage
(char InCommand, string InBuffer, uint16_t InBufferSize);

/*****************************************************************************!
 * Function : CANInterfaceOpen
 * Purpose  : Public face of opening the CAN device, (nuts and bolts are in 
 *            CANInterfaceOpenPort)
 *****************************************************************************/
CANInterface*
CANInterfaceOpen
()
{
  return NULL;
}

/*****************************************************************************!
 * Function : CANInterfaceFlush
 * Purpose  : Clear the interface with any leftover packets from previous
 *            sessions
 *****************************************************************************/
void
CANInterfaceFlush
()
{
}

/*****************************************************************************!
 * Function : CANInterfaceOpenNamed
 * Purpose  : Open a CAN interfcae device by port name (e.g., COM1)
 *****************************************************************************/
CANInterface*
CANInterfaceOpenNamed
(
 string                                 InPortName
)
{
  (void)InPortName;
  return NULL;
}

/*****************************************************************************!
 * Function : CANInterfaceInitialize
 * Purpose  : Setup the interface 
 *****************************************************************************/
void
CANInterfaceInitialize
()
{
}

/*****************************************************************************!
 * Function : CANInterfaceOpenPort
 * Purpose  : Perform mechanics of opening the port and setting the comm params
 *****************************************************************************/
CANInterface*
CANInterfaceOpenPort
(
 string                                 InPortName
)
{
  (void)InPortName;
  return NULL;
}


/*****************************************************************************!
 * Function : CANInterfaceSimpleTransaction
 * Purpose  : Send a simple command to the interface
 *****************************************************************************/
void
CANInterfaceSimpleTransaction
(
 string                                 InString
)
{
}

/*****************************************************************************!
 * Function : CANInterfaceMessageTransaction
 * Purpose  : Send a packet intended to device connection the CAN interface
 *****************************************************************************/
void
CANInterfaceMessageTransaction
(
 uint32_t                               InID,
 uint64_t                               InData,
 uint8_t                                InDataLength
)
{
}

/*****************************************************************************!
 * Function : CANInterfaceMessageSend
 * Purpose  : Send a packet intended to device connection the CAN interface
 *****************************************************************************/
void
CANInterfaceMessageSend
(
 uint32_t                               InID,
 uint64_t                               InData,
 uint8_t                                InDataLength
)
{
}

/*****************************************************************************!
 * Function : CANInterfaceWrite
 * Purpose  : Write a block of data to the device
 *****************************************************************************/
void
CANInterfaceWrite
(
 string                         	InBuffer
)
{
}

/*****************************************************************************!
 * Function : CANInterfaceRead
 * Purpose  : Read  block of data from the CAN Interface
 *****************************************************************************/
bool
CANInterfaceRead
(
 string                                 InBuffer,
 uint16_t                               InBufferMaxSize,
 uint16_t*                              InBufferSize
)
{
  return false;
}    

/*****************************************************************************!
 * Function : CANInterfaceHandleRead
 * Purpose  : Process the responses to a CAN interface command 
 *****************************************************************************/
void
CANInterfaceHandleRead
(
 char                           	InCommand,
 string                         	InBuffer,
 uint16_t                       	InBufferSize
)
{
}

/*****************************************************************************!
 * Function : CANInterfaceHandleMessage
 *****************************************************************************/
void
CANInterfaceHandleMessage
(
 char                           	InCommand,
 string                         	InBuffer,
 uint16_t                       	InBufferSize
)
{
}

/*****************************************************************************!
 * Function : DisplayOSErrorMessage
 *****************************************************************************/
void
DisplayOSErrorMessage
(
 string                                 InString
)
{
}    

/*****************************************************************************!
 * Function : SetMonitorCANProtocol
 *****************************************************************************/
void
SetMonitorCANProtocol
(
 bool                                   InValue
)
{
}

/*****************************************************************************!
 * Function : CANInterfaceDisplayMessage
 *****************************************************************************/
void
CANInterfaceDisplayMessage
(
 uint8_t                                InMessageType,
 uint32_t                               InID,
 uint64_t                               InData,
 uint8_t                                InDataLength
)
{
}
