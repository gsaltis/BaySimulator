/******************************************************************************
 * FILE NAME    : BayFile.c
 * DATE         : March 04 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 ******************************************************************************/

/******************************************************************************!
 * Global Headers
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <MemoryManager.h>
#include <StringUtils.h>

/******************************************************************************!
 * Local Headers
 ******************************************************************************/
#include "BayFile.h"
#include "BayType.h"
#include "main.h"

/******************************************************************************!
 * Local Functions
 ******************************************************************************/
void
BayFileRead
(BayFile* InBayFile);

void
BayFileReadAdd
(BayFile* InBayFile);

void
BayFileReadAddBay
(BayFile* InBayFile);

void
BayFileReadAddRectifier
(BayFile* InBayFile);

/******************************************************************************!
 * Function : BayFileOpen
 * Purpose  : Create a BayFile struct and populate it from a file
 ******************************************************************************/
BayFile*
BayFileOpen
(
 string                                         InFileName
)
{
  FILE*                                         file;
  BayFile*                                      bayFile;
  struct stat                                   statbuf;
  char*                                         buffer;
  uint32_t                                      bytesRead;
  
  if ( NULL == InFileName ) {
    return NULL;      
  }
  file = fopen(InFileName, "rb");

  //! We failed to open the file
  if ( NULL == file ) {
    return NULL;
  }
  
  stat(InFileName, &statbuf);  
  
  // Read the file contents
  buffer = (char*)GetMemory(statbuf.st_size);
  bytesRead = fread(buffer, 1, statbuf.st_size, file);
  if ( bytesRead != statbuf.st_size ) {
    //! This is an error
    fclose(file);
    FreeMemory(buffer);
    return NULL;
  }
  
  //! Create. set up, and return the bay file
  bayFile = (BayFile*)GetMemory(sizeof(BayFile));
  bayFile->buffer = buffer;
  bayFile->bufferSize = bytesRead;
  bayFile->currentBufferIndex = 0;
  bayFile->filename = StringCopy(InFileName);
  bayFile->firstEntry = NULL;
  bayFile->lastEntry = NULL;
  
  return bayFile;
}

/*****************************************************************************!
 * Function : BayFileRead
 * Purpose  : Process the contents of a bay file
 *****************************************************************************/
void
BayFileRead
(
 BayFile*                       InBayFile
)
{
  if ( NULL == InBayFile ) {
    return;
  }
  while(InBayFile->currentBufferIndex < InBayFile->bufferSize ) {
    char                        command;
    
    command = InBayFile->buffer[InBayFile->currentBufferIndex++];
    
    if ( command == BAY_FILE_COMMAND_ADD ) {
      BayFileReadAdd(InBayFile);
    }
  }
}

/*****************************************************************************!
 * Function : BayFileReadAdd
 * Purpose  : Process the contents of a bay file ADD Command
 *****************************************************************************/
void
BayFileReadAdd
(
 BayFile*                       InBayFile
)
{
  char                        type;
  type = InBayFile->buffer[InBayFile->currentBufferIndex++];
  if ( type == BAY_FILE_TYPE_BAY ) {
    BayFileReadAddBay(InBayFile);
  } else if ( type == BAY_FILE_TYPE_RECTIFIER ) {
    BayFileReadAddRectifier(InBayFile);
  }
}

/*****************************************************************************!
 * Function : BayFileReadAddBay
 * Purpose  : Process the contents of a bay file ADD BAY Command
 *****************************************************************************/
void
BayFileReadAddBay
(
 BayFile*                       InBayFile
)
{
  uint8_t                       bayIndex, bayType;
  uint16_t                      blockLength;
  string                        bayName;   // Free me
  uint16_t                      nameLen;
  Bay*                          bay;
  bayIndex = InBayFile->buffer[InBayFile->currentBufferIndex++];
  blockLength = (InBayFile->buffer[InBayFile->currentBufferIndex] << 8) +
               (InBayFile->buffer[InBayFile->currentBufferIndex+1]);
  
  InBayFile->currentBufferIndex += 2;
  
  bayType = InBayFile->buffer[InBayFile->currentBufferIndex++];
  nameLen = blockLength - 1;
  bayName = StringNCopy(&(InBayFile->buffer[InBayFile->currentBufferIndex]), nameLen);
  bay = CreateBay(bayName, (BayType)bayType, bayIndex);
  
  InBayFile->currentBufferIndex += nameLen;
  FreeMemory(bayName);
  
  BaySetAddBay(mainBays, bay);
}

/*****************************************************************************!
 * Function : BayFileReadAddRectifier
 * Purpose  : Process the contents of a bay file ADD RECTIFIER Command
 *****************************************************************************/
void
BayFileReadAddRectifier
(
 BayFile*                       InBayFile
)
{
  
}

/*****************************************************************************!
 * Function : BayFileWrite
 * Purpose  : 
 *****************************************************************************/
void
BayFileWrite
(BayFile* InBayFile)
{
  FILE*                             file;
  BayFileEntry*                     entry;
  
  if ( NULL == InBayFile ) {
    return;
  }
  file = fopen(InBayFile->filename, "wb+");
  if ( NULL == file ) {
    fprintf(stderr, "Could not open output file %s\r\n", InBayFile->filename);
    return;
  }
  
  // Write the bays first
  entry = InBayFile->firstEntry;
  while (entry) {
    if ( entry->type == BAY_FILE_TYPE_BAY ) {
      uint16_t                  length;
      fwrite(&entry->type, 1, 1, file);
      fwrite(&entry->command, 1, 1, file);
      if ( entry->command == BAY_FILE_COMMAND_ADD ) {
        length = strlen(entry->BayAdd.bayName) + 1;
        fwrite(&entry->BayAdd.bayIndex, 1, 1, file);
        fwrite(&length, 1, 2, file);
        fwrite(&entry->BayAdd.bayType, 1, 1, file);
        fwrite(&entry->BayAdd.bayName, 1, length-1, file);
      }
    }
    entry = entry->next;
  }
  
  entry = InBayFile->firstEntry;
  while (entry) {
    if ( entry->type == BAY_FILE_TYPE_RECTIFIER ) {
      fwrite(&entry->type, 1, 1, file);
      fwrite(&entry->command, 1, 1, file);
      if ( entry->command == BAY_FILE_COMMAND_ADD ) {
        fwrite(&entry->RectifierAdd.bayIndex, 1, 1, file);
        fwrite(&entry->RectifierAdd.unitNumber, 1, 1, file);
        fwrite(&entry->RectifierAdd.shelfNumber, 1, 1, file);
      }
    }
  }
  fclose(file);
}

/*****************************************************************************!
 * Function : BayFileEntryAdd
 * Purpose  : Add an entry to a bay file
 *****************************************************************************/
void
BayFileEntryAdd
(BayFile* InBayFile, BayFileEntry* InEntry)
{
  if ( NULL == InBayFile || NULL == InEntry ) {
    return;
  }
  if ( InBayFile->firstEntry == NULL ) {
    InBayFile->firstEntry = InEntry;
  } else {
    InBayFile->lastEntry->next = InEntry;
  }
  InBayFile->lastEntry = InEntry;
}

/*****************************************************************************!
 * Function : BayFileEntyrCreateBayAdd
 *****************************************************************************/
BayFileEntry*
BayFileEntryCreateBayAdd
(
 uint8_t                        InBayIndex,
 uint8_t                        InBayType,
 string                         InBayName
 )
{
  BayFileEntry*                 returnEntry;
  returnEntry = (BayFileEntry*)GetMemory(sizeof(BayFileEntry));
  returnEntry->command = BAY_FILE_COMMAND_ADD;
  returnEntry->type = BAY_FILE_TYPE_BAY;
  returnEntry->BayAdd.bayIndex = InBayIndex;
  returnEntry->BayAdd.bayType = InBayType;
  returnEntry->BayAdd.bayName = StringCopy(InBayName);
  return returnEntry;
}

/*****************************************************************************!
 * Function : BayFileEntyrCreateRectifierAdd
 *****************************************************************************/
BayFileEntry*
BayFileEntryCreateRectifierAdd
(uint8_t InBayIndex, uint8_t InUnitNumber, uint8_t InShelfNumber) 
{
  BayFileEntry*                 returnEntry;
  
  returnEntry = (BayFileEntry*)GetMemory(sizeof(BayFileEntry));
  returnEntry->command = BAY_FILE_COMMAND_ADD;
  returnEntry->type = BAY_FILE_TYPE_RECTIFIER;
  returnEntry->RectifierAdd.bayIndex = InBayIndex;
  returnEntry->RectifierAdd.unitNumber = InUnitNumber;
  returnEntry->RectifierAdd.shelfNumber = InShelfNumber;
  
  return returnEntry;
}

/*****************************************************************************!
 * Function : BayFileAddBay
 *****************************************************************************/
void
BayFileAddBay
(
 BayFile*                       InBayFile,
 Bay*                           InBay
)
{
  BayFileEntry*                 fileEntry;
  
  if ( NULL == InBayFile || NULL == InBay ) {
    return;
  }
  fileEntry = BayFileEntryCreateBayAdd(InBay->index, InBay->type, InBay->name);
  BayFileEntryAdd(InBayFile, fileEntry);
}

/*****************************************************************************!
 * Function : BayFileAddRectifier
 *****************************************************************************/
void
BayFileAddRectifier
(
 BayFile*                       InBayFile,
 RectifierType*                 InRectifier
)
{
   
   
}
