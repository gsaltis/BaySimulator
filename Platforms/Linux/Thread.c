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
#include <MemoryManager.h>
#include <StringUtils.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "Thread.h"

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

Thread*
ThreadFind
(ThreadHandle InHandle);

/*****************************************************************************!
 * Imported Functions
 *****************************************************************************/
extern int usleep(unsigned int usec);

/*****************************************************************************!
 * Function : ThreadCreate
 *****************************************************************************/
ThreadHandle
ThreadCreate(string InThreadName, void (*InThread)(void*), void* InArg)
{
 
  void*					p[2];
  Thread*				thread;

  printf("%s %d : %s\n", __FILE__, __LINE__, InThreadName);
  thread = CreateThreadHolder(InThreadName);
  p[0] = InThread;
  p[1] = InArg;
 
  pthread_create(&thread->threadHandle, NULL, ThreadWorker, (void*)p);
  return thread->externalHandle;
}

/*****************************************************************************!
 * Function : ThreadsJoin
 *****************************************************************************/
void
ThreadsJoin()
{
  Thread*                       thread;
  
  thread = threadHead;
  printf("%s %d\n", __FILE__, __LINE__);
  while (thread) {
    printf("%s %d : %s\n", __FILE__, __LINE__, thread->name);
    pthread_join(thread->threadHandle, NULL);
    thread = thread->next;
  }
  printf("%s %d\n", __FILE__, __LINE__);
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

  printf("%s %d\n", __FILE__, __LINE__);
  if ( NULL == InParameters ) {
    printf("%s %d\n", __FILE__, __LINE__);
    return NULL;
  }

  printf("%s %d\n", __FILE__, __LINE__);
  parameters = (void**)InParameters;
  function = (void*(*)(void*))parameters[0];
  functionParameters = (void*)parameters[1];

  function(functionParameters);
  printf("%s %d\n", __FILE__, __LINE__);
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

/******************************************************************************!
 * Function : ThreadGetFirstHandle
 ******************************************************************************/
ThreadHandle
ThreadGetFirstHandle()
{
  if ( threadHead == NULL ) {
    return 0;
  }
  return threadHead->externalHandle;
}

/******************************************************************************!
 * Function : ThreadGetNextHandle
 ******************************************************************************/
ThreadHandle
ThreadGetNextHandle
(
 ThreadHandle                           InHandle
)
{
  Thread*				thread;

  thread = ThreadFind(InHandle);
  if ( thread == NULL ) {
    return 0;
  }
  if ( thread->next == NULL ) {
    return 0;
  }
  
  return thread->next->externalHandle;
}


/******************************************************************************!
 * Function : ThreadGetName
 ******************************************************************************/
const string
ThreadGetName
(
 ThreadHandle				InHandle
)
{
  Thread*				thread;

  thread = ThreadFind(InHandle);
  if ( NULL == thread ) {
    return NULL;
  }
  return thread->name;
}

/******************************************************************************!
 * Function : ThreadFind
 ******************************************************************************/
Thread*
ThreadFind
(
 ThreadHandle				InHandle
)
{
  Thread*				thread;
  if ( InHandle == 0 ) {
    return NULL;
  }
  thread = threadHead;
  while (thread && thread->externalHandle != InHandle) {
    thread = thread->next;
  }
  return thread;
}

