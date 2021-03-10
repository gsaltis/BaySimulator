/*****************************************************************************
 * FILE NAME    : System.c
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

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "System.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
System
SystemInfo;

/*****************************************************************************!
 * Function : SystemInitialize
 *****************************************************************************/
void
SystemInitialize
()
{
  SystemInfo.name = StringCopy("BaySim");
  SystemInfo.dataDir = StringCopy("/etc/baysim/");
  SystemInfo.dataFilename = StringConcat(SystemInfo.dataDir, "baysim.dat");
  SystemInfo.configFilename = StringConcat(SystemInfo.dataDir, "baysim.ini");
  SystemInfo.verboseStartup = true;
}

/*****************************************************************************!
 * Function : SystemFileExists
 *****************************************************************************/
bool
SystemFileExists
(
 string                         InFilename
)
{
  FILE*                         file;
  
  if ( InFilename == NULL || *InFilename == 0x00 ) {
    return false;
  }
  file = fopen(InFilename, "r");
  if ( NULL == file ) {
    return false;
  }
  fclose(file);
  return true;
}

bool
SystemInfoGetVerboseStartup
()
{
  return SystemInfo.verboseStartup;
}

string
SystemInfoGetConfigFileName()
{
  return SystemInfo.configFilename;
}

string
SystemInfoGetDataFileName()
{
  return SystemInfo.dataFilename;
}
