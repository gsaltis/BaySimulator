/*****************************************************************************
 * FILE NAME    : Version.h
 * DATE         : January 21 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _version_h_
#define _version_h_

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
#define VERSION_MAJOR     1
#define VERSION_MINOR     1
#define VERSION_RELEASE   0
#define VERSION_BUILD     4

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
uint8_t
GetVersionMajor
();

uint8_t
GetVersionMinor
();

string
GetVersionString
();

#endif /* _version_h_*/