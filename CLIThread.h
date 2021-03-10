/*****************************************************************************
 * FILE NAME    : CLIThread.h
 * DATE         : January 21 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _clithread_h_
#define _clithread_h_

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
#define ColorRed                        "\e[1;31m"
#define ColorWhite                      "\e[0;37m"
#define HeadingStyle                  "\e[1;7;37m"
#define ColorGreen                      "\e[32m"
#define ColorYellow                     "\e[33m"
#define ColorBoldYellow                 "\e[1;33m"

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/
extern bool CLIVerboseInterface;
extern bool CLIWriteLog;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void*
CLIThread
(void* InParameters);

void
CLIThreadInit();

void
ProcessArgs
(char** InArgs, int InArgsCount);

void
CLILogMessage
(char* format, ...);

void
CLILogInit
();

void
ProcessCommandLine
(string InCommandBuffer, int InCommandBufferIndex);

void
CLIDisplayMessage
(char* format, ...);

void
CLIDisplayConfirmMessage
(char* format, ...);

void
CLILogFilenameSet
(string InLogFilename);

void
CLILogDisplayInitMessage
();

#endif /* _clithread_h_*/
