/*****************************************************************************
 * FILE NAME    : DeviceFile.c
 * DATE         : June 06 2019
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
#include <sys/types.h>
#include <unistd.h>
#include <StringUtils.h>
#include <MemoryManager.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "main.h"
#include "CLIThread.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Function : DeviceFileRead
 *****************************************************************************/
bool
DeviceFileRead
(string InFileName)
{
  FILE*                                 file;
  struct stat                           statbuf;
  uint32_t                              filesize, bytesRead;
  string                                buffer;
  StringList*                           lines;
  string                                line;

  uint16_t                              i;
  
  file = fopen(InFileName, "rb");
  if ( NULL == file ) {
    printf("Could not open \"%s\"\n", InFileName);
    return false;
  }
  stat(InFileName, &statbuf);
  filesize = statbuf.st_size;
  buffer = GetMemory(filesize + 1);
  bytesRead = fread(buffer, 1, filesize, file);
  if ( bytesRead != filesize ) {
    printf("Error reading \"%s\" bytes read: %d   file size : %d\n", InFileName, bytesRead, filesize);
    FreeMemory(buffer);
    fclose(file);
    return false;
  }
  buffer[filesize] = 0x00;
  fclose(file);

  lines = StringSplit(buffer, "\r\n", true);
  FreeMemory(buffer);

  ClearBay(mainBays->bays[0]);

  // Process each line of the file. Treat the line as though
  // a user had typed in the string on the command line
  for ( i = 0; i < lines->stringCount; i++ ) {
    line = StringTrim(lines->strings[i]);
    ProcessCommandLine(line, strlen(line));
    FreeMemory(line);
  }
  StringListDestroy(lines);
  return true;
}


