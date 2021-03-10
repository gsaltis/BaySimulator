/*****************************************************************************
 * FILE NAME    : DSMElementType.h
 * DATE         : February 06 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _dsmelementtype_h_
#define _dsmelementtype_h_

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

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define DSM_ELEMENT_TYPE_FUSE                   0x01
#define DSM_ELEMENT_TYPE_MISC_CODE              0x02
#define DSM_ELEMENT_TYPE_BREAKER                0x03
#define DSM_ELEMENT_TYPE_BULLET                 0x05
#define DSM_ELEMENT_TYPE_EMPTY                  0x07

/*****************************************************************************!
 * Exported Type : DSMElementType
 *****************************************************************************/
struct _DSMElementType
{
    uint8_t                     type;
    uint16_t                    rating;
    uint16_t                    alarmBits;                
};

typedef struct _DSMElementType DSMElementType;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/

#endif /* _dsmelementtype_h_*/
