/******************************************************************************!
 * FILE         : CANInterface.c
 * PROJECT      : Bay Simulator
 * FILE DATE    : June 20 2019
 ******************************************************************************/

/******************************************************************************!
 * Global Headers
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <StringUtils.h>
#include <MemoryManager.h>

/******************************************************************************!
 * Local Headers
 ******************************************************************************/
#include "CANInterface.h"
#include "CLIThread.h"

/******************************************************************************!
 * Local Macros
 ******************************************************************************/
#define CAN_DEFAULT_TIMEOUT   1

/******************************************************************************!
 * Local Functions
 ******************************************************************************/
static uint8_t
CANInterfaceReadTimeout
(CANInterface* InInterface, uint32_t* InID, uint64_t* InData, uint8_t* InDataLength, uint8_t InTimeout);

/******************************************************************************!
 * Local Data
 ******************************************************************************/

/******************************************************************************!
 * Function : CANInterfaceInit
 ******************************************************************************/
CANInterface*
CANInterfaceInit
(string InInterfaceName)
{
  struct ifreq ifr;
  struct sockaddr_can addr;
  int canSocket = 0;
  CANInterface*                         canInterface;
  memset(&ifr, 0x0, sizeof(ifr));
  memset(&addr, 0x0, sizeof(addr));
  
  /* open CAN_RAW socket */
  canSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if ( canSocket == -1 ) {
    fprintf(stderr, "Socket open failed.\n");
    return NULL;
  }

  /* convert interface string into interface index */
  strcpy(ifr.ifr_name, InInterfaceName);
  if ( -1 == ioctl(canSocket, SIOCGIFINDEX, &ifr) ) {
    fprintf(stderr, "Could not get CAN interface %s\n", InInterfaceName);
    return NULL;
  }

  /* setup address for bind */
  addr.can_ifindex = ifr.ifr_ifindex;
  addr.can_family= PF_CAN;

  /* bind socket to the can0 interface */
  if ( -1 == bind(canSocket, (struct sockaddr *)&addr, sizeof(addr)) ) {
    fprintf(stderr, "Could not bind to socket\n");
    return NULL;
  }
  canInterface = (CANInterface*)GetMemory(sizeof(CANInterface));
  canInterface->socket = canSocket;
  canInterface->portName = StringCopy(InInterfaceName);
  canInterface->opened = true;

  return canInterface;
}

/******************************************************************************!
 * Function : CANInterfaceClose
 ******************************************************************************/
void
CANInterfaceClose
(CANInterface* InInterface)
{
  if ( NULL == InInterface ) {
    return;
  }

  close(InInterface->socket);
  FreeMemory(InInterface->portName);
  FreeMemory(InInterface);
}

/******************************************************************************!
 * Function : CANInterfaceWrite
 ******************************************************************************/
void
CANInterfaceWrite
(CANInterface* InInterface, uint32_t InID, uint64_t InData, uint8_t InDataLength)
{
  struct can_frame                      frame;

  if ( NULL == InInterface ) {
    return;
  }
  memset(&frame, 0x0, sizeof(frame));
  frame.can_id = InID| CAN_EFF_FLAG;
  memcpy(&(frame.data), &InData, sizeof(frame.data));
  frame.can_dlc = InDataLength;
  write(InInterface->socket, &frame, sizeof(frame));
}

/******************************************************************************!
 * Function : CANInterfaceRead
 ******************************************************************************/
uint8_t
CANInterfaceRead
(CANInterface* InInterface, uint32_t* InID, uint64_t* InData, uint8_t* InDataLength)
{
  return CANInterfaceReadTimeout(InInterface, InID, InData, InDataLength, CAN_DEFAULT_TIMEOUT);
}

/******************************************************************************!
 * Function : CANInterfaceReadTimeout
 ******************************************************************************/
uint8_t
CANInterfaceReadTimeout
(CANInterface* InInterface, uint32_t* InID, uint64_t* InData, uint8_t* InDataLength, uint8_t InTimeout)
{
  fd_set                                readSet;
  struct timeval                        tv;
  struct can_frame                      frame;
  int                                   retval;
  int                                   bytesRead;
  
  if ( NULL == InInterface ) {
    return CAN_READ_ERROR;
  }
  FD_ZERO(&readSet);
  FD_SET(InInterface->socket, &readSet);
  tv.tv_sec = InTimeout;
  tv.tv_usec = 0;

  retval = select(InInterface->socket + 1, &readSet, NULL, NULL, &tv);
  switch (retval) {
    case 1 : {
      bytesRead = read(InInterface->socket, &frame, sizeof(frame));
      if ( bytesRead == sizeof(frame) ) {
        *InID = frame.can_id & 0x7FFFFFFF;
        memcpy(InData, &(frame.data), sizeof(frame.data));
        *InDataLength = frame.can_dlc;
        return CAN_READ_OK;
      }
      break;
    }

    case 0 : {
      return CAN_READ_TIMEOUT;
      break;
    }

    case -1 : {
      return CAN_READ_ERROR;
      break;
    }

  }
  return CAN_READ_UNKNOWN;
}

/******************************************************************************!
 * Function : CANInterfaceTransaction
 ******************************************************************************/
uint8_t
CANInterfaceTransaction
(CANInterface* InInterface, uint32_t InID, uint64_t InData, uint8_t InDataLength,
 uint32_t* InReadID, uint64_t* InReadData, uint8_t* InReadDataLength,
 uint8_t InRetries, uint8_t InTimeout)
{
  int                                   i, n;

  CANInterfaceWrite(InInterface, InID, InData, InDataLength);
  for ( i = 0; i < InRetries; i++ ) {
    n = CANInterfaceReadTimeout(InInterface, InReadID, InReadData, 
                                InReadDataLength, InTimeout);
    if ( n == CAN_READ_OK ) {
      return CAN_READ_OK;
    }
    if ( n == CAN_READ_ERROR ) {
      return CAN_READ_ERROR;
    }
  }
  return CAN_READ_TIMEOUT;
}

/******************************************************************************!
 * Function : CANInterfaceReset
 ******************************************************************************/
void
CANInterfaceReset
()
{
  char*                                 stopInterface = "bash ./slcan_remove";
  char*                                 startInterface = "bash ./slcan_add";

  CLILogMessage("Stopping the CAN Interface\n");
  system (stopInterface);
  sleep(2);
  CLILogMessage("Starting the CAN Interface\n");
  system(startInterface);
  sleep(3);
}
