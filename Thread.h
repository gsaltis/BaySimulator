/*****************************************************************************
 * FILE NAME    : Thread.h
 * DATE         : February 21 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/
#ifndef _thread_h_
#define _thread_h_

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
 * Exported Data
 *****************************************************************************/
typedef uint32_t			ThreadHandle;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
ThreadHandle
ThreadCreate
(string InThreadName, void (*InThread)(void*), void* InArg);

void
ThreadSleep
(uint32_t InMilliseconds);

ThreadHandle
ThreadGetFirstHandle();

ThreadHandle
ThreadGetNextHandle
(ThreadHandle InHandle);

void
ThreadsJoin();

const string
ThreadGetName
(ThreadHandle InHandle);

#endif /* _thread_h_*/
