/*****************************************************************************
 * FILE NAME    : main.h
 * DATE         : February 04 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _main_h_
#define _main_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <StringUtils.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "BayType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define MAIN_CAN_INTERFACE_NAME     "slcan0"

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/
extern
BaySet*
mainBays;

extern int32_t
mainSerialNumber;

extern string
mainDeviceFilename;

extern string
mainWebBaseDirectory;

extern string
mainVersion;

extern pthread_t
canReadThread;

extern char*
mainDSMProductNumber;

extern char*
mainDSMDistOnlyProductNumber;
/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
uint32_t
GetNextSerialNumber
();

#endif /* _main_h_*/
