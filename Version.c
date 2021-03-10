/*****************************************************************************
 * FILE NAME    : Version.c
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
#include <StringUtils.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "Version.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
static uint8_t                  VersionMajor = VERSION_MAJOR;
static uint8_t                  VersionMinor = VERSION_MINOR;
static uint8_t                  VersionRelease = VERSION_RELEASE;
static uint8_t                  VersionBuild = VERSION_BUILD;
static char                     VersionString[32];

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Function : GetVersionMajor
 *****************************************************************************/
uint8_t
GetVersionMajor
()
{
  return VersionMajor;
}

/*****************************************************************************!
 * Function : GetVersionMinor
 *****************************************************************************/
uint8_t
GetVersionMinor
()
{
  return VersionMinor;
}

/*****************************************************************************!
 * Function : GetVersionString
 *****************************************************************************/
string
GetVersionString
()
{
  sprintf(VersionString, "%d.%d.%d.%d", VersionMajor, VersionMinor, VersionRelease, VersionBuild);
  return VersionString;
}


