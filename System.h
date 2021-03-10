/*****************************************************************************
 * FILE NAME    : System.h
 * DATE         : January 21 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _system_h_
#define _system_h_

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

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type
 *****************************************************************************/
typedef struct _System
{
    string                      name;
    bool                        verboseStartup;
    string                      dataDir;
    string                      configFilename;
    string                      dataFilename;
} System;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
SystemInitialize
();

bool
SystemFileExists
(string InFilename);

bool
SystemInfoGetVerboseStartup
();

string
SystemInfoGetConfigFileName
();

string
SystemInfoGetDataFileName
();

#endif /* _system_h_*/
