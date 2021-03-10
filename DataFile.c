/*****************************************************************************
 * FILE NAME    : DataFile.c
 * DATE         : April 23 2019
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
#include <sys/stat.h>
#include <ctype.h>
#include <MemoryManager.h>
#include <StringUtils.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "DataFile.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
bool
DataFileSectionEnd
(string InBuffer);

string
DataFileSectionStart
(string InBuffer);

/*****************************************************************************!
 * Function : DataFileCreate
 *****************************************************************************/
DataFile*
DataFileCreate
()
{
  DataFile*                             dataFile;

  dataFile = (DataFile*)GetMemory(sizeof(DataFile));

  memset(dataFile, 0x00, sizeof(DataFile));
  return dataFile;
}

/*****************************************************************************!
 * Function : DataFileRead
 *****************************************************************************/
DataFile*
DataFileRead
(string InFilename)
{
  FILE*                                 file;
  string                                buffer;
  string                                bufferPointer;
  struct stat                           statbuf;
  DataFile*                             dataFile;
  uint32_t                              bytesRead;
  StringList*                           sections;
  string                                section;
  uint32_t                              n;
  string                                start, end;
  
  file = fopen(InFilename, "rb");
  if ( NULL == file ) {
    return NULL;
  }

  stat(InFilename, &statbuf);
  buffer = GetMemory(statbuf.st_size + 1);
  bytesRead = fread(buffer, 1, statbuf. st_size, file);
  if ( bytesRead != statbuf.st_size ) {
    FreeMemory(buffer);
    return NULL;
  }

  buffer[bytesRead] = 0x00;
  dataFile = DataFileCreate();
  
  bufferPointer = buffer;

  sections = StringListCreate();
  while (*bufferPointer) {
    bufferPointer = DataFileSectionStart(bufferPointer);
    if ( *bufferPointer == 0x00 ) {
      break;
    }
    while (*bufferPointer && isspace(*bufferPointer) ) {
      bufferPointer++;
    }
    start = bufferPointer;
    end = bufferPointer;
    while ( !DataFileSectionEnd(end) ) {
      end++;
    }
    n = (end - start) + 1;
    
    section = StringNCopy(start, n);
    StringListAppend(sections, section);
    bufferPointer = end;
  }

  dataFile->buffer = buffer;
  dataFile->bufferLength = bytesRead;
  dataFile->sections = sections;

  return dataFile;
}

/*****************************************************************************!
 * Function : DataFileSave
 *****************************************************************************/
void
DataFileSave
(string InFilename)
{


}

/*****************************************************************************!
 * Function : DataFileSectionEnd
 *****************************************************************************/
bool
DataFileSectionEnd
(string InBuffer)
{
  if ( *InBuffer == 0x00 ) {
    return true;
  }
  
  if ( InBuffer[0] == 0x0A && InBuffer[1] == 0x0A ) {
    return true;
  }
  
  if ( InBuffer[0] == 0x0D &&
       InBuffer[1] == 0x0A &&
       InBuffer[2] == 0x0D &&
       InBuffer[3] == 0x0A ) {
    return true;
  }
  return false;
}

/*****************************************************************************!
 * Function : DataFileSectionEnd
 *****************************************************************************/
string
DataFileSectionStart
(string InBuffer)
{
  if ( NULL == InBuffer ) {
    return NULL;
  }
  while (*InBuffer && (isspace(*InBuffer) || 
                       *InBuffer == 0x0a || *InBuffer == 0x0D) ) {
    InBuffer++;
  }
  return InBuffer;
}
