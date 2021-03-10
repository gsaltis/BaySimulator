/*****************************************************************************
 * FILE NAME    : Mutex.c
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
#include <pthread.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "Mutex.h"
#include "MemoryManager.h"

/*****************************************************************************!
 * Local Type : MutexType
 *****************************************************************************/
struct _MutexType
{
  string			name;
  Mutex				externalHandle;
  pthread_mutex_t		handle;
  struct _MutexType*            next;
};
typedef struct _MutexType MutexType;

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
MutexType*
mutexHead = NULL;

MutexType*
mutexLast = NULL;

Mutex
mutexNextMutex = 1;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
MutexType*
CreateMutexHolder
(string InName);

/*****************************************************************************!
 * Function : MutexCreate
 *****************************************************************************/
Mutex
MutexCreate
(
 string 			InName
)
{
  MutexType*			mutex = CreateMutexHolder(InName);
  return mutex->externalHandle;
}

/*****************************************************************************!
 * Function : MutexGet
 *****************************************************************************/
void
MutexGet
(
 Mutex 				InMutex
)
{
  MutexType*			mutex;

  for ( mutex = mutexHead; mutex; mutex = mutex->next ) {
    if ( mutex->externalHandle == InMutex ) {
      break;
    }
  }
  if ( NULL == mutex ) {
    return;
  }
  pthread_mutex_lock(&(mutex->handle));
}

/*****************************************************************************!
 * Function : MutexRelease
 *****************************************************************************/
void
MutexRelease
(Mutex InMutex)
{
  MutexType*			mutex;

  for ( mutex = mutexHead; mutex; mutex = mutex->next ) {
    if ( mutex->externalHandle == InMutex ) {
      break;
    }
  }
  if ( NULL == mutex ) {
    return;
  }
  pthread_mutex_unlock(&(mutex->handle));
}

/******************************************************************************!
 * Function : CreateMutexHolder
 ******************************************************************************/
MutexType*
CreateMutexHolder
(
 string					InName
)
{
  MutexType*				mutexType;

  mutexType = (MutexType*)GetMemory(sizeof(MutexType));

  mutexType->externalHandle = mutexNextMutex++;
  mutexType->name = StringCopy(InName);
  mutexType->next = NULL;

  if ( mutexHead == NULL ) {
    mutexHead = mutexType;
    mutexLast = mutexType;
  } else {
    mutexLast->next = mutexType;
    mutexLast = mutexType;
  }
  return mutexType;
}

