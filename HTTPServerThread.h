/*****************************************************************************
 * FILE NAME    : HTTPServerThread.h
 * DATE         : January 24 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _httpserverthread_h_
#define _httpserverthread_h_

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
#include "string.h"
#include "BayType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define HTTP_SEND_CAN_PACKET_DIRECTION_OUT      1
#define HTTP_SEND_CAN_PACKET_DIRECTION_IN       2

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
HTTPServerInit();

void
HTTPSendCANPacketString
(string InCANPacketString);

void
HTTPSendCANPacket
(uint8_t InDirection, uint32_t InID, uint64_t InData, uint8_t InDataLength);

void
HTTPSendNodeCANAddress
(uint8_t InNodeType, uint8_t InBayIndex, uint8_t InUnitNumber, uint8_t InCANAddress);

void*
HTTPServerThread
(void* InParameters);

void
HTTPSendBayAdd
(Bay* InBay);

void
HTTPSendRectifierAdd
(uint8_t InUnit, uint8_t InBay);

void
HTTPSendRectifierPower
(uint8_t InBayIndex, uint8_t InUnitNumber, bool InPower);

#endif /* _httpserverthread_h_*/
