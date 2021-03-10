/*****************************************************************************
 * FILE NAME    : BayFile.h
 * DATE         : March 04 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _bayfile_h_
#define _bayfile_h_

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
#include "StringUtils.h"
#include "BayType.h"
#include "RectifierType.h"

/*****************************************************************************!
 * Exported Type : BayFileEntryCommand
 *****************************************************************************/
enum _BayFileEntryCommand {
  BAY_FILE_COMMAND_NONE           = 0,
  BAY_FILE_COMMAND_ADD,
  BAY_FILE_COMMAND_UPDATE
};
typedef enum _BayFileEntryCommand BayFileEntryCommand;

/*****************************************************************************!
 * Exported Type : BayFileEntryType
 *****************************************************************************/
enum _BayFileEntryType {
  BAY_FILE_TYPE_NONE              = 0,
  BAY_FILE_TYPE_BAY,
  BAY_FILE_TYPE_RECTIFIER,
  BAY_FILE_TYPE_DSM
};
typedef enum _BayFileEntryType BayFileEntryType;

/*****************************************************************************!
 * Exported Type : BayFileEntry
 *****************************************************************************/
struct _BayFileEntry {
  BayFileEntryCommand           command;
  BayFileEntryType              type;
  union {
    struct {
      uint8_t                   bayIndex;
      uint8_t                   shelfNumber;
      uint8_t                   unitNumber;
    } RectifierAdd;
    struct {
      uint8_t                   bayIndex;
      BayType                   bayType;
      string                    bayName;      // Free me
    } BayAdd;
  };
  struct _BayFileEntry*         next;
  
};
typedef struct _BayFileEntry BayFileEntry;

/*****************************************************************************!
 * Exported Type : BayFile
 *****************************************************************************/
struct _BayFile {
  string                        filename;
  char*                         buffer;
  uint32_t                      bufferSize;
  uint32_t                      currentBufferIndex;
  BayFileEntry*                 firstEntry;
  BayFileEntry*                 lastEntry;
};
typedef struct _BayFile BayFile;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
BayFile*
BayFileOpen
(string InFileName);

void
BayFileProcess
(BayFile* InBayFile);


void
BayFileWrite
(BayFile* InBayFile);

void
BayFileAddBay
(BayFile* InBayFile, Bay* InBay);

void
BayFileAddRectifier
(BayFile* InBayFile, RectifierType* InRectifier);

#endif // _bayfile_h_
