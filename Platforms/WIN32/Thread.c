/*****************************************************************************
 * FILE NAME    : Thread.c
 * DATE         : February 21 2019
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
#include <unistd.h>
#include <pthread.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "Thread.h"
#include "MemoryManager.h"
#include "String.h"

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void*
ThreadWorker
(void* InParameters);

/*****************************************************************************!
 * Local Type : Thread
 *****************************************************************************/
struct _Thread
{
  string				name;
  pthread_t				threadHandle;
  ThreadHandle				externalHandle;
  struct _Thread*			next;
};
typedef struct _Thread Thread;

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
Thread*
threadHead = NULL;

Thread*
threadLast = NULL;

ThreadHandle
threadNextExternalHandle = 1;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
Thread*
CreateThreadHolder
(string InName);

/*****************************************************************************!
 * Function : ThreadCreate
 *****************************************************************************/
ThreadHandle
ThreadCreate(string InThreadName, void (*InThread)(void*), void* InArg)
{
 
  void*					p[2];
  Thread*				thread;

  thread = CreateThreadHolder(InThreadName);
  p[0] = InThread;
  p[1] = InArg;
 
  pthread_create(&thread->threadHandle, NULL, ThreadWorker, (void*)p);
  return thread->externalHandle;
}

/*****************************************************************************!
 * Function : ThreadSleep
 *****************************************************************************/
void
ThreadSleep
(
 uint32_t 				InMilliseconds
)
{
  usleep(InMilliseconds * 1000);
}

/*****************************************************************************!
 * Function : ThreadWorker
 *****************************************************************************/
void*
ThreadWorker
(
 void*					InParameters
)
{
  void**				parameters;
  void*					(*function)(void*);
  void*					functionParameters;

  if ( NULL == InParameters ) {
    return NULL;
  }

  parameters = (void**)InParameters;
  function = (void*(*)(void*))parameters[0];
  functionParameters = (void*)parameters[1];

  function(functionParameters);
  return NULL;
}

/******************************************************************************!
 * Function : CreateThreadHolder
 ******************************************************************************/
Thread*
CreateThreadHolder
(
 string					InName
)
{
  Thread*				thread;

  thread = (Thread*)GetMemory(sizeof(Thread));
  thread->name = StringCopy(InName);
  thread->externalHandle = threadNextExternalHandle++;
  if ( threadHead == NULL ) {
    threadHead = thread;
    threadLast = thread;
  } else {
    threadLast->next = thread;
    threadLast = thread;
  }
  return thread;
}

