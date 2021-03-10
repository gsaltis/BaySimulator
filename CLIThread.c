/*****************************************************************************
 FILE NAME      : CLIThread.c
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Corp.
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <strings.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <StringUtils.h>
#include <MemoryManager.h>
#include <ascii.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "CLIThread.h"
#include "NodeDataStore.h"
#include "Simulator.h"
#include "ESNACANNodeType.h"
#include "CANInterface.h"
#include "Version.h"
#include "System.h"
#include "main.h"
#include "Thread.h"
#include "Mutex.h"
#include "HTTPServerThread.h"
#include "SystemTime.h"
#include "CANWriteThread.h"
#include "BayType.h"
#include "DeviceInterface.h"
#include "ShuntType.h"
#include "DeviceFile.h"
#include "CANIOThread.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define COMMAND_BUFFER_SIZE             128
#define COMMAND_LINE_MAX_ARGS           16
#define COMMAND_HISTORY_SIZE            16
#define LOGFILE_NAME                    "/var/log/baysim.log"

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
static char                             CommandBuffer[COMMAND_BUFFER_SIZE+1];
static char                             CommandLineClear[COMMAND_BUFFER_SIZE+1];
static uint32_t                         CommandBufferIndex = -1;
static uint32_t                         CommandBufferSize = 0;
static char*                            CommandPrompt = "Command : ";
static char                             CommandHistory[COMMAND_HISTORY_SIZE][COMMAND_BUFFER_SIZE+1];
static int                              CommandHistoryIndex = 0;
static int                              CommandHistoryIndexLast = 0;
bool                                    CLIVerboseInterface = false;
bool                                    CLIWriteLog = false;
string					CLILogFilename = NULL;
string					CLILogFilenameDefault = LOGFILE_NAME;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/
ThreadHandle
CLIThreadHandle;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void
Initialize();

static void
GetCLICommandLine();

static void
DisplayPrompt();

static void
CommandDataStore
(char** InArgs, int InArgsCount);

static void
CommandHelp
(char** InArgs, int InArgsCount);

static void
CommandRead
(char** InArgs, int InArgsCount);

static void
CommandSave
(char** InArgs, int InArgsCount);

static void
StoreCommandHistory();

static void
DisplayNodeInformation();

static void
CommandNodeSetCustomerText
(char** InArgs, int InArgsCount);

static void
CommandDSMSetSerialNumber
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetSerialNumber
(char** InArgs, int InArgsCount);

static void
CommandNodeSetPartNumber
(char** InArgs, int InArgsCount);

static void
CommandNodeSetMiscInfo
(char** InArgs, int InArgsCount);

static void
CommandNode
(char** InArgs, int InArgsCount);

static void
CommandQuit
(char** InArgs, int InArgsCount);

static void
CommandSimulator
(char** InArgs, int InArgsCount);

static void
CommandSimulatorStop
();

static void
CommandSimulatorStart
();

static void
CommandSimulatorCAN
(char** InArgs, int InArgsCount);

static void
CommandSimulatorCANStop
();

static void
CommandSimulatorCANStart
();

static void
CommandSimulatorCANRestart
();

static void
CommandSimulatorPause
();

static void
CommandSimulatorShow
(char** InArgs, int InArgsCount);

static void
CommandSimulatorVersion
(char** InArgs, int InArgsCount);

static void
CommandSimulatorShowPackets
(char** InArgs, int InArgsCount);

static void
CommandNodeSet
(char** InArgs, int InArgsCount);

static void
CommandNodeSetAppRevision
(char** InArgs, int InArgsCount);

static void
CommandNodeSetBootRevision
(char** InArgs, int InArgsCount);

static void
CommandRectifierRemove
(char** InArgs, int InArgsCount);

static void
CommandRectifierAdd
(char** InArgs, int InArgsCount);

static void
CommandDSMAdd
(char** InArgs, int InArgsCount);

static void
CommandDSMRemove
(char** InArgs, int InArgsCount);

void
CommandDSMShow
(char** InArgs, int InArgsCount);

static void
CommandRectifier
(char** InArgs, int InArgsCount);

static void
CommandRectifierShow
(char** InArgs, int InArgsCount);

static void
CommandRectifierSet
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetSerialNumber
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetInputVolts
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetRatedCurrent
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetMaxCurrentCapacity
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetOutputVolts
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetCurrent
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetParams
(char** InArgs, int InArgsCount);

static void
CommandRectifierSetAlarm
(char** InArgs, int InArgsCount);


static void
CommandRectifierSetAlarmStats
(char** InArgs, int InArgsCount);

static void
CommandDSM
(char** InArgs, int InArgsCount);

static void
CommandDSMSet
(char** InArgs, int InArgsCount);

static void
CommandDSMSetSerialNumber
(char** InArgs, int InArgsCount);

static void
CommandDSMSetShuntStatus
(char** InArgs, int InArgsCount);

static void
CommandDSMSetShuntCurrents
(char** InArgs, int InArgsCount);

static void
CommandDSMSetShunt
(char** InArgs, int InArgsCount);

static void
CommandDSMSetID2Bits
(char** InArgs, int InArgsCount);

static void
CommandDSMSetIDBits
(char** InArgs, int InArgsCount);

static void
CommandDSMSetBattVolts
(char** InArgs, int InArgsCount);

static void
CommandDSMSetBattTemps
(char** InArgs, int InArgsCount);

static void
CommandDSMSetStatus
(char** InArgs, int InArgsCount);

static void
CommandDSMSetBusVolts
(char** InArgs, int InArgsCount);

static void
CommandTest();

static void
CommandDebug
(char** InArgs, int InArgsCount);

static void
CommandDebugCANIF
(char** InArgs, int InArgsCount);

static void
CommandDebugDumpIO
(char** InArgs, int InArgsCount);

static void
CommandDebugSet
(char** InArgs, int InArgsCount);

static void
CommandDebugSetDumpOutMessages
(char** InArgs, int InArgsCount);

static void
CommandSystem
(char** InArgs, int InArgsCount);

static void
CommandSystem24Volts
(char** InArgs, int InArgsCount);

static void
CommandSystemDownSlope
(char** InArgs, int InArgsCount);

static void
CommandSystemLoadShare
(char** InArgs, int InArgsCount);

static void
CommandSystemFahrenheit
(char** InArgs, int InArgsCount);

static void
CommandSystemPowerShare
(char** InArgs, int InArgsCount);

static void
CommandSystemPowerShareCapacity
(char** InArgs, int InArgsCount);

static void
CommandSend
(char** InArgs, int InArgsCount);

static void
CommandSendNode
(char** InArgs, int InArgsCount);


static void
CommandBay
(char** InArgs, int InArgsCount);

static void
CommandBayShow
(char** InArgs, int InArgsCount);

static void
CommandBaySet
(char** InArgs, int InArgsCount);

static void
CommandBaySetName
(char** InArgs, int InArgsCount);

static void
CommandBayAdd
(char** InArgs, int InArgsCount);

static void
CommandBayShowAll
();

static void
CommandDebugThreads
(char** InArgs, int InArgsCount);

void
CommandRectifierOn
(char** InArgs, int InArgsCount);

void
CommandRectifierOff
(char** InArgs, int InArgsCount);

void
CommandDist
(char** InArgs, int InArgsCount);

void
CommandDistAdd
(char** InArgs, int InArgsCount);

void
CommandDistRemove
(char** InArgs, int InArgsCount);

void
CommandDistSet
(char** InArgs, int InArgsCount);

void
CommandDistSetOpen
(char** InArgs, int InArgsCount);

void
CommandDistSetClose
(char** InArgs, int InArgsCount);

void
CommandDistSetCurrent
(char** InArgs, int InArgsCount);

void
CommandDistList
(char** InArgs, int InArgsCount);

/*****************************************************************************!
 * Function : CLIThreadInit
 *****************************************************************************/
void
CLIThreadInit()
{
}

/*****************************************************************************!
 * Function : CLILogFilenameSet
 *****************************************************************************/
void
CLILogFilenameSet
(string InLogFilename)
{
  if ( InLogFilename == NULL ) {
	return;
  }

  if ( CLILogFilename ) {
	FreeMemory(CLILogFilename);
  }

  CLILogFilename = StringCopy(InLogFilename);
}

/*****************************************************************************!
 * Function : CLIThread
 *****************************************************************************/
void*
CLIThread
(
 void*                                  InParameters
)
{
  (void)InParameters;
  Initialize();
  printf("\r\n%sBay Simulator : %s%s\r\n", ColorGreen, mainVersion, ColorWhite);
  while (1)
  {
    DisplayPrompt();
    GetCLICommandLine();
    ProcessCommandLine(CommandBuffer, CommandBufferIndex);
    CommandBuffer[0] = NUL;
    CommandBufferSize = 0;
    CommandBufferIndex = 0;
  }
}

/*****************************************************************************!
 * Function : GetCLICommandLine
 *****************************************************************************/
void
GetCLICommandLine
()
{
  int                         n;

  CommandBufferSize = 0;
  CommandBuffer[0] = NUL;

  fgets(CommandBuffer, COMMAND_BUFFER_SIZE, stdin);
  n = strlen(CommandBuffer);     
  if ( n > 0 ) {
    if ( n == 1 && CommandBuffer[0] == '\n' ) {
      if ( CANWriteDisplayPacket ) {
        CANWriteDisplayPacket = false;
      }
    } else {
      CommandBuffer[n-1] = NUL;
      CommandBufferSize = n-1;
      CommandBufferIndex = CommandBufferSize;
    }
  } 
}

/*****************************************************************************!
 * Function : CLILogInit
 *****************************************************************************/
void
CLILogInit
()
{
  CLILogFilename = StringCopy(CLILogFilenameDefault);
}

/*****************************************************************************!
 * Function : CLILogDisplayInitMessage
 *****************************************************************************/
void
CLILogDisplayInitMessage
()
{
  FILE* file = fopen(CLILogFilename, "wb");
  fprintf(file, "Bay Simulator Log Initialize\n");
  fclose(file);
}

/*****************************************************************************!
 * Function : Initialize
 *****************************************************************************/
void
Initialize()
{
  CommandBufferIndex      = 0;
  CommandHistoryIndex     = -1;
  CommandHistoryIndexLast = -1;
    
  memset(CommandHistory, NUL, ((COMMAND_BUFFER_SIZE+1) * COMMAND_HISTORY_SIZE));
  memset(CommandBuffer, 0x00, COMMAND_BUFFER_SIZE+1);
  memset(CommandLineClear, ' ', COMMAND_BUFFER_SIZE);
  CommandLineClear[COMMAND_BUFFER_SIZE] = 0x00;
}

/*****************************************************************************!
 * Function : DisplayPrompt
 *****************************************************************************/
void
DisplayPrompt()
{
  CLIDisplayMessage("\r%s", CommandLineClear);
  CLIDisplayMessage("\r%s%s", CommandPrompt, CommandBuffer);
}

/*****************************************************************************!
 * Function : ProcessCommandLine 
 *****************************************************************************/
void
ProcessCommandLine
(string InCommandBuffer, int InCommandBufferIndex)
{
  char*                               args[COMMAND_LINE_MAX_ARGS];
  int                                 args_counter = 0;
  int                                 i;
  char*                               commandStart;
  char                                buffer[COMMAND_BUFFER_SIZE+1];
  int                                 stringLen;
  enum {
    IN_START,
    IN_STRING_START,
    IN_STRING_END,
    IN_STRING,
    IN_CHARS,
    IN_SPACE
  } mode = IN_START;

  strcpy(buffer, InCommandBuffer);
  StoreCommandHistory();
  commandStart = NULL;
  for (i = 0; i < InCommandBufferIndex; i++) {
    switch (mode) {
      case IN_START : {
        if ( buffer[i] == ' ' ) {
          mode = IN_SPACE;
        } else if (buffer[i] == '\"' ) {
          stringLen = 0;
          mode = IN_STRING_START;
        } else {
          mode = IN_CHARS;
          commandStart = &(buffer[i]);
        }
        break;
      }
      case IN_SPACE : {
        if ( buffer[i] == '\"' ) {
          stringLen = 0;
          mode = IN_STRING_START;
        } else {
          mode = IN_CHARS;
          commandStart = &(buffer[i]);
        }
        break;
      }
      case IN_STRING_START : {
        if ( buffer[i] == '\"' ) {
          mode = IN_STRING_END;
        } else {
          commandStart = &(buffer[i]);
          stringLen++;
          mode = IN_STRING;
        }
        break;
      }
      case IN_STRING_END : {
        if ( buffer[i] != ' ' ) {
          CLIDisplayMessage("\r\nInvalid Command Line\r\n");
          return;
        }
        if ( stringLen != 0 ) {
          if ( args_counter >= COMMAND_LINE_MAX_ARGS ) {
            CLIDisplayMessage("\r\nToo many command line args - Maximum is %d\r\n", COMMAND_LINE_MAX_ARGS);
            return;
          }
          args[args_counter] = commandStart;
          args_counter++;
        }
        mode = IN_SPACE;
        break;
      }
      case IN_STRING : {
        if ( buffer[i] == '\"' ) {
          buffer[i] = 0x00;
          mode = IN_STRING_END;
        }
        break;
      }
      case IN_CHARS : {
        if ( buffer[i] == ' ' ) {
          buffer[i] = NUL;
          if ( args_counter >= COMMAND_LINE_MAX_ARGS ) {
            CLIDisplayMessage("\r\nToo many command line args - Maximum is %d\r\n", COMMAND_LINE_MAX_ARGS);
            return;
          }
          args[args_counter] = commandStart;
          args_counter++;
          mode = IN_SPACE;
        } else {
          if ( buffer[i] == '\"' ) {
            CLIDisplayMessage("\r\nInvalid Command Line\r\n");
            return;
          }
        }
        break;
      }
    }
  }

  if ( mode == IN_CHARS || mode == IN_STRING_END ) {
    if ( args_counter >= COMMAND_LINE_MAX_ARGS ) {
      CLIDisplayMessage("\r\nToo many command line args - Maximum is %d\r\n", COMMAND_LINE_MAX_ARGS);
      return;
    }
    args[args_counter] = commandStart;
    args_counter++;
  }
  else if ( mode == IN_STRING_START || mode == IN_STRING ) {
    CLIDisplayMessage("\r\nUnterminated string\r\n");
    return;
  }
  ProcessArgs(args, args_counter);
}

/*****************************************************************************!
 * Function : ProcessArgs
 *****************************************************************************/
void
ProcessArgs
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount == 0 ) {
    return;
  }

  if ( !strcasecmp(InArgs[0], "help") ) {
    CommandHelp(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "quit") ) {
    CommandQuit(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "sim") ) {
    CommandSimulator(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "datastore") ) {
    CommandDataStore(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "node") ) {
    CommandNode(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "bay") ) {
    CommandBay(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "rect") ) {
    CommandRectifier(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "dist") ) {
    CommandDist(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "dsm") ) {
    CommandDSM(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "system") ) {
    CommandSystem(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "debug") ) {
    CommandDebug(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "send") ) {
    CommandSend(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "test") ) {
    CommandTest();
  } else if ( !strcasecmp(InArgs[0], "read") ) {
    CommandRead(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[0], "save") ) {
    CommandSave(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandDist
 *****************************************************************************/
void
CommandDist
(char** InArgs, int InArgsCount)
{
  if ( InArgsCount < 2 ) {
    return;
  }
  if ( !strcasecmp(InArgs[1], "add") ) {
    CommandDistAdd(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "remove") ) {
    CommandDistRemove(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "set") ) {
    CommandDistSet(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandDistAdd
 *****************************************************************************/
void
CommandDistAdd
(char** InArgs, int InArgsCount)
{
  if ( InArgsCount < 7 ) {
    return;
  }

  if ( DeviceDistAdd(InArgs[2], InArgs[3], InArgs[4], InArgs[5], InArgs[6]) ) {
    CLIDisplayConfirmMessage("%s Amp %s added to Bay:1 DSM:%s\n", InArgs[6], InArgs[3], InArgs[2]);
  }
}

/*****************************************************************************!
 * Function : CommandDistAdd
 *****************************************************************************/
void
CommandDistRemove
(char** InArgs, int InArgsCount)
{
  if ( InArgsCount < 4 ) {
    return;
  }

  if ( DeviceDistRemove(InArgs[2], InArgs[3]) ) {
    CLIDisplayConfirmMessage("slot %s has been removed from Bay:1 DSM:%s\n", InArgs[3], InArgs[2]);
  }
}

/*****************************************************************************!
 * Function : CommandDistSet
 *****************************************************************************/
void
CommandDistSet
(char** InArgs, int InArgsCount)
{
  if ( InArgsCount < 5 ) {
    return;
  }
  if ( !strcasecmp(InArgs[4], "open") ) {
    CommandDistSetOpen(InArgs, InArgsCount);
  } else if ( StringEqual(InArgs[4], "close") || StringEqual(InArgs[4], "closed") ) {
    CommandDistSetClose(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[4], "current") ) {
    CommandDistSetCurrent(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandDSMShow
 *****************************************************************************/
void
CommandDSMShow
(char** InArgs, int InArgsCount)
{
  uint8_t                               unitNumber;
  Bay*                                  bay;
  DSMType*                              dsm;

  if ( InArgsCount < 3 ) {
    return;
  }
  unitNumber = atoi(InArgs[2]);
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return;
  }

  dsm = BayFindDSM(bay, unitNumber);
  if ( NULL == dsm ) {
    return;
  }
  NodeDisplay(dsm->parentNode);
  DSMDisplay(dsm);
}
 
/*****************************************************************************!
 * Function : CommandDistSetOpen
 *****************************************************************************/
void
CommandDistSetOpen
(char** InArgs, int InArgsCount)
{
  uint8_t                               unitNumber, slotIndex, index;
  Bay*                                  bay;
  DSMType*                              dsm;

  if ( InArgsCount < 5 ) {
    return;
  }
  unitNumber = atoi(InArgs[2]);
  slotIndex = atoi(InArgs[3]);
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    printf("Could not find bay\r\n");
    return;
  }

  dsm = BayFindDSM(bay, unitNumber);
  if ( NULL == dsm ) {
    printf("Could not find DSM %d\r\n", unitNumber);
    return;
  }
  if ( slotIndex < 1 || slotIndex > DSM_MAX_SHUNTS ) {
    printf("%d is an invalid slot\r\n", slotIndex);
    return;
  }
  index = slotIndex - 1;

  if ( !dsm->shunts[index].occupied ) {
    printf("slot %d is not occupied\r\n", slotIndex);
    return;
  }

  dsm->shunts[index].power = SHUNT_POWER_OPENED;
  CLIDisplayConfirmMessage("Slot %d:%d is open\n", unitNumber, slotIndex);
}

/*****************************************************************************!
 * Function : CommandDistSetClose
 *****************************************************************************/
void
CommandDistSetClose
(char** InArgs, int InArgsCount)
{
  uint8_t                               unitNumber, slotIndex, index;
  Bay*                                  bay;
  DSMType*                              dsm;

  if ( InArgsCount < 5 ) {
    return;
  }
  unitNumber = atoi(InArgs[2]);
  slotIndex = atoi(InArgs[3]);
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    printf("Could not find bay\r\n");
    return;
  }

  dsm = BayFindDSM(bay, unitNumber);
  if ( NULL == dsm ) {
    printf("Could not find DSM %d\r\n", unitNumber);
    return;
  }
  if ( slotIndex < 1 || slotIndex > DSM_MAX_SHUNTS ) {
    printf("%d is an invalid slot\r\n", slotIndex);
    return;
  }
  index = slotIndex - 1;

  if ( !dsm->shunts[index].occupied ) {
    printf("slot %d is not occupied\r\n", slotIndex);
    return;
  }

  dsm->shunts[index].power = SHUNT_POWER_CLOSED;
  CLIDisplayConfirmMessage("Slot %d:%d is closed\n", unitNumber, slotIndex);
}

/*****************************************************************************!
 * Function : CommandDistSetCurrent
 *****************************************************************************/
void
CommandDistSetCurrent
(char** InArgs, int InArgsCount)
{
  uint8_t                               unitNumber, slotIndex, index;
  uint16_t                              current;
  Bay*                                  bay;
  DSMType*                              dsm;

  if ( InArgsCount < 6 ) {
    return;
  }

  unitNumber = atoi(InArgs[2]);
  slotIndex = atoi(InArgs[3]);
  current = atoi(InArgs[5]);
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    printf("Could not find bay\r\n");
    return;
  }

  dsm = BayFindDSM(bay, unitNumber);
  if ( NULL == dsm ) {
    printf("Invalid DSM Number %d\r\n", unitNumber);
    return;
  }
  if ( slotIndex < 1 || slotIndex > DSM_MAX_SHUNTS ) {
    printf("Invalid slot Number %d\r\n", slotIndex);
    return;
  }
  index = slotIndex - 1;

  if ( !dsm->shunts[index].occupied ) {
    printf("Slot %d:%d is not occupied\r\n", unitNumber, slotIndex);
    return;
  }

  dsm->shunts[index].current = current;
  CLIDisplayConfirmMessage("Slot %d:%d current set to %d\r\n", unitNumber, slotIndex, current);
}

/*****************************************************************************!
 * Function : CommandQuit
 *****************************************************************************/
void
CommandQuit
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  (void)InArgs;
  (void)InArgsCount;
  exit(EXIT_SUCCESS);
}

/*****************************************************************************!
 * Function : CommandSend
 *****************************************************************************/
void
CommandSend
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 2 ) {
    return;
  }
    
  if ( !strcasecmp(InArgs[1], "node") ) {
    CommandSendNode(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandSimulator
 *****************************************************************************/
void
CommandSimulator
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  char*                               subCommand;
    
  if ( InArgsCount < 2 ) {
    return;
  }
  subCommand = InArgs[1];

  if ( !strcasecmp(subCommand, "start") ) {
    CommandSimulatorStart();
    CLIDisplayConfirmMessage("\r\nSimulator Started\r\n");
    return;
  }
  if ( !strcasecmp(subCommand, "stop") ) {
    CommandSimulatorStop();
    CLIDisplayConfirmMessage("\r\nSimulator Stopped\r\n");
    return;
  }

  if ( !strcasecmp(subCommand, "pause" ) ) {
    CommandSimulatorPause();
    CLIDisplayConfirmMessage("\r\nSimulator Pause\r\n");
    return;
  }

  if ( !strcasecmp(subCommand, "show" ) ) {
    CommandSimulatorShow(InArgs, InArgsCount);
    return;
  }

  if ( StringEqualNoCase(subCommand, "can") ) {
    CommandSimulatorCAN(InArgs, InArgsCount);
    return;
  }
  if ( !strcasecmp(subCommand, "version" ) ) {
    CommandSimulatorVersion(InArgs, InArgsCount);
    return;
  }
}

/*****************************************************************************!
 * Function : CommandSimulatorPause
 *****************************************************************************/
void
CommandSimulatorPause
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  SimulatorPause();
}

/*****************************************************************************!
 * Function : CommandSimulatorStop
 *****************************************************************************/
void
CommandSimulatorStop
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  SimulatorStop();

}

/*****************************************************************************!
 * Function : CommandSimulatorStart
 *****************************************************************************/
void
CommandSimulatorStart
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  SimulatorStart();
}

/*****************************************************************************!
 * Function : CommandSimulatorVerison
 *****************************************************************************/
void
CommandSimulatorVersion
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  (void)InArgs;
  (void)InArgsCount;

  printf("\r\nSimulator Version : %s\r\n", GetVersionString());
}

/*****************************************************************************!
 * Function : CommandSimulatorShow
 *****************************************************************************/
void
CommandSimulatorShow
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 3 ) {
    return;
  }
  if ( StringEqual(InArgs[2], "packets") ) {
    CommandSimulatorShowPackets(InArgs, InArgsCount);
  }
}

/******************************************************************************!
 * Function : CommandSimulatorShowPackets
 ******************************************************************************/
void
CommandSimulatorShowPackets
(char** InArgs, int InArgsCount)
{
  if ( InArgsCount < 4 ) {
    return;
  }
  if ( StringEqualNoCase(InArgs[3], "on") ) {
    CANWriteDisplayPacket = true;
  } else if ( StringEqualNoCase(InArgs[3], "off") ) {
    CANWriteDisplayPacket = false;
  }
  CLIDisplayMessage("Dump Packets : %s\r\n", CANWriteDisplayPacket ? "On" : "Off");
}

/*****************************************************************************!
 * Function : CommandSendNode
 *****************************************************************************/
void
CommandSendNode
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  ESNACANMessageType                  message;
  int                                 base;
  int                                 i, n;
    
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( !strncmp(InArgs[2], "0x", 2) || !strncmp(InArgs[2], "0X", 2) ) {
    base = 16;
  }
  message.id.whole = strtol(InArgs[2], NULL, base);
  for (i = 0, n = 3; n < InArgsCount; i++, n++) {
    char* arg = InArgs[n];
    int s = strlen(arg);
    if ( s == 2 ) {
      message.data.byte[i] = strtol(InArgs[n], NULL, 16);
    } else if ( s == 8 ) {
      message.data.byte[i] = strtol(InArgs[n], NULL, 2);
    }
  }
  message.length = i;
  CANTransmitNodeMessage(&message);
#endif    
}

/*****************************************************************************!
 * Function : CommandTest
 *****************************************************************************/
void
CommandTest()
{
}

/*****************************************************************************!
 * Function : CommandRectifier
 *****************************************************************************/
void
CommandRectifier
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 2 ) {
    return;
  }

  if ( !strcasecmp(InArgs[1], "add") ) {
    CommandRectifierAdd(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "set") ) {
    CommandRectifierSet(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "on")) {
    CommandRectifierOn(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "off") ) {
    CommandRectifierOff(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "show") ) {
    CommandRectifierShow(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "remove") ) {
    CommandRectifierRemove(InArgs, InArgsCount);
  }
}

/******************************************************************************!
 * Function : CommandRectifierRemove
 ******************************************************************************/
void
CommandRectifierRemove
(
 char**                         InArgs,
 int                            InArgsCount
)
{
  uint8_t                       unit;
  RectifierType*                rectifier;
  Bay*                          bay;
  
  if ( InArgsCount < 3 ) {
    return;
  }
  unit  = atoi(InArgs[2]);
  
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( bay == NULL ) {
    return;
  }
  rectifier = BayFindRectifier(bay, unit);
  if ( rectifier == NULL ) {
    return;
  }
  BayRemoveRectifier(bay, rectifier);
  CLIDisplayConfirmMessage("Rectifier %d removed\n", unit);
}
 
/******************************************************************************!
 * Function : CommandRectifierOn
 ******************************************************************************/
void
CommandRectifierOn
(
 char**                         InArgs,
 int                            InArgsCount
)
{
  uint8_t                       unit;
  RectifierType*                rectifier;
  Bay*                          bay;
  
  if ( InArgsCount < 3 ) {
    return;
  }

  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( bay == NULL ) {
    return;
  }
  
  if ( StringEqualNoCase(InArgs[2], "all") ) {
    BaySetAllRectifiersOn(bay);
    CLIDisplayConfirmMessage("All rectifiers are on\r\n");
  }

  unit  = atoi(InArgs[2]);
  rectifier = BayFindRectifier(bay, unit);
  if ( rectifier == NULL ) {
    return;
  }
  RectifierTurnOn(rectifier);
  HTTPSendRectifierPower(1, unit, true);
  CLIDisplayConfirmMessage("Rectifier %d is on\r\n", unit);
}

/******************************************************************************!
 * Function : CommandRectifierShow
 ******************************************************************************/
void
CommandRectifierShow
(
 char**                         InArgs,
 int                            InArgsCount
)
{
  uint8_t                       unit;
  RectifierType*                rectifier;
  Bay*                          bay;
  
  if ( InArgsCount < 3 ) {
    return;
  }
  unit  = atoi(InArgs[2]);
  
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( bay == NULL ) {
    return;
  }
  rectifier = BayFindRectifier(bay, unit);
  if ( rectifier == NULL ) {
    return;
  }
  NodeDisplay(rectifier->parentNode);
  RectifierDisplay(rectifier);
}

/******************************************************************************!
 * Function : CommandRectifierOff
 ******************************************************************************/
void
CommandRectifierOff
(
 char**                         InArgs,
 int                            InArgsCount
)
{
  uint8_t                       unit;
  RectifierType*                rectifier;
  Bay*                          bay;
  
  if ( InArgsCount < 3 ) {
    return;
  }
  
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( bay == NULL ) {
    return;
  }
  if ( StringEqualNoCase(InArgs[2], "all") ) {
    BaySetAllRectifiersOff(bay);
    CLIDisplayConfirmMessage("All rectifiers are off\r\n");
  }

  unit  = atoi(InArgs[2]);
  rectifier = BayFindRectifier(bay, unit);
  if ( rectifier == NULL ) {
    return;
  }
  RectifierTurnOff(rectifier);
  HTTPSendRectifierPower(1, unit, false);
  CLIDisplayConfirmMessage("Rectifier %d is off\r\n", unit);
}

/*****************************************************************************!
 * Function : CommandDSM
 *****************************************************************************/
void
CommandDSM
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 2 ) {
    return;
  }

  if ( !strcasecmp(InArgs[1], "set") ) {
    CommandDSMSet(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "add") ) {
    CommandDSMAdd(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "remove") ) {
    CommandDSMRemove(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "show") ) {
    CommandDSMShow(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandDSMSet
 *****************************************************************************/
void
CommandDSMSet
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( !strcasecmp(InArgs[2], "shunt") ) {
    CommandDSMSetShunt(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "serial") ) {
    CommandDSMSetSerialNumber(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "id2bits") ) {
    CommandDSMSetID2Bits(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "idbits") ) {
    CommandDSMSetIDBits(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "battvolts") ) {
    CommandDSMSetBattVolts(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "batttemps") ) {
    CommandDSMSetBattTemps(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "status") ) {
    CommandDSMSetStatus(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "busv") ) {
    CommandDSMSetBusVolts(InArgs, InArgsCount);
  }       
}

/*****************************************************************************!
 * Function : CommandDSMSetShunt
 *****************************************************************************/
void
CommandDSMSetShunt
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 4 ) {
    return;
  }
  if ( !strcasecmp(InArgs[3], "status") ) {
    CommandDSMSetShuntStatus(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[3], "currents") ) {
    CommandDSMSetShuntCurrents(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandDSMSetShuntCurrents
 *****************************************************************************/
void
CommandDSMSetShuntCurrents
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint16_t                            startNumber;
  uint16_t                            currents[4];
  uint32_t                            nodeNumber;
  NodeType*                           node;

  if ( InArgsCount < 10 ) {
    return;
  }
  nodeNumber = atoi(InArgs[4]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }
  startNumber = StringToInteger(InArgs[5], 16);
  for (int i = 0; i < 4; i++) {
    currents[i] = StringToInteger(InArgs[6+i], 16);
  }
  // ESNASendDSMShuntCurrents(nodeNumber, startNumber, currents);
  CLIDisplayConfirmMessage("\r\nShunt currents set for DSM %d\r\n\r\n", nodeNumber);
#endif    
}
    
/*****************************************************************************!
 * Function : CommandDSMSetShuntStatus
 *****************************************************************************/
void
CommandDSMSetShuntStatus
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint32_t                            hibits, lobits;
  uint32_t                            nodeNumber;
  NodeType*                           node;
    
  if ( InArgsCount < 7 ) {
    return;
  }
  if ( strcasecmp(InArgs[3], "status") ) {
    return;
  }
  nodeNumber = atoi(InArgs[4]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n\r\n", nodeNumber);
    return;
  }
  hibits = StringToInteger(InArgs[5], 24);
  lobits = StringToInteger(InArgs[6], 24);
  //ESNASendDSMShuntStatus(nodeNumber, hibits, lobits);
  CLIDisplayConfirmMessage("\r\nShunt status set for DSM %d\r\n\r\n", nodeNumber);
#endif
}

/*****************************************************************************!
 * Function : CommandDSMSetID2Bits
 *****************************************************************************/
void
CommandDSMSetID2Bits
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint32_t                            nodeNumber;
  NodeType*                           node;
  uint32_t                            bits;
    
  if ( InArgsCount < 5 ) {
    return;
  }
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }

  bits = StringToInteger(InArgs[4], 32);
  //ESNASendDSMFuseBreakerStatus_2(nodeNumber, bits);
  CLIDisplayConfirmMessage("\r\nID bits set 2 set for DSM %d\r\n\r\n", nodeNumber);
#endif    
}

/*****************************************************************************!
 * Function : CommandDSMSetIDBits
 *****************************************************************************/
void
CommandDSMSetIDBits
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint32_t                            nodeNumber;
  NodeType*                           node;
  uint32_t                            bits0, bits1;
    
  if ( InArgsCount < 6 ) {
    return;
  }
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }

  bits0 = StringToInteger(InArgs[4], 32);
  bits1 = StringToInteger(InArgs[5], 32);
  //ESNASendDSMFuseBreakerStatus_0_1(nodeNumber, bits0, bits1);
  CLIDisplayConfirmMessage("\r\nID bits set 0 and 1 set for DSM %d\r\n\r\n", nodeNumber);
#endif
}

/*****************************************************************************!
 * Function : CommandDSMSetBattVolts
 *****************************************************************************/
void
CommandDSMSetBattVolts
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint32_t                            nodeNumber;
  NodeType*                           node;
  uint16_t                            battmV[4];
        
  if ( InArgsCount < 8 ) {
    return;
  }
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }

  for (int i = 0; i < 4; i++){
    battmV[0] = atoi(InArgs[4+i]);
  }
  //ESNASendDSMBatteryVoltages(nodeNumber, battmV);
  CLIDisplayConfirmMessage("\r\nBattery voltages set for DSM %d\r\n\r\n", nodeNumber);
#endif
}
 
/*****************************************************************************!
 * Function : CommandDSMSetBattTemps
 *****************************************************************************/
void
CommandDSMSetBattTemps
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint32_t                            nodeNumber;
  NodeType*                           node;
  uint16_t                            battTemps[4];
  if ( InArgsCount < 8 ) {
    return;
  }
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }

  for (int i = 0; i < 4; i++){
    battTemps[0] = atoi(InArgs[4+i]);
  }
  //ESNASendDSMBatteryTemps(nodeNumber, battTemps);
  CLIDisplayConfirmMessage("\r\nBattery temperature set for DSM %d\r\n\r\n", nodeNumber);
#endif
}
 
/*****************************************************************************!
 * Function : CommandDSMSetStatus
 *****************************************************************************/
void
CommandDSMSetStatus
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint32_t                            nodeNumber;
  NodeType*                           node;
  uint16_t                            statusbits;
  uint32_t                            alarms;
  uint16_t                            battshuntstatus;
    
  if ( InArgsCount < 7 ) {
    return;
  }
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }
  statusbits = StringToInteger(InArgs[4], 16);
  alarms = StringToInteger(InArgs[5], 32);
  battshuntstatus = StringToInteger(InArgs[6], 16);
  //ESNASendDSMStatusAlarms(nodeNumber, statusbits, alarms, battshuntstatus);
  CLIDisplayConfirmMessage("\r\nAlarms set for DSM %d\r\n\r\n", nodeNumber);
#endif
}
 
/*****************************************************************************!
 * Function : CommandDSMSetBusVolts
 *****************************************************************************/
void
CommandDSMSetBusVolts
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint32_t                            nodeNumber;
  NodeType*                           node;
  uint16_t                            bus1volts, bus2volts;
    
  if ( InArgsCount < 6 ) {
    return;
  }
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }
  bus1volts = StringToInteger(InArgs[4], 16);
  bus2volts = StringToInteger(InArgs[4], 16);
  //ESNASendDSMBusVoltages(nodeNumber, bus1volts, bus2volts);
  CLIDisplayConfirmMessage("\r\nBus voltages set for DSM %d\r\n\r\n", nodeNumber);
#endif
} 

/*****************************************************************************!
 * Function : CommandRectifierSet
 *****************************************************************************/
void
CommandRectifierSet
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 3 ) {
    return;
  }
  if ( !strcasecmp(InArgs[2], "ivolt") ) {
    CommandRectifierSetInputVolts(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "ovolt") ) {
    CommandRectifierSetOutputVolts(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "serial") ) {
    CommandRectifierSetSerialNumber(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "current") ) {
    CommandRectifierSetCurrent(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "params") ) {
    CommandRectifierSetParams(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "update") ) {
    CommandRectifierSetAlarmStats(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "currmax" ) ) {
    CommandRectifierSetMaxCurrentCapacity(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[2], "ratedcurr") ) {
    CommandRectifierSetRatedCurrent(InArgs, InArgsCount);
  } else if ( StringEqualNoCase(InArgs[2], "alarm") ) {
    CommandRectifierSetAlarm(InArgs, InArgsCount);
  } else {
    CLIDisplayMessage("\r\nInvalid rect set subcommand \"%s\"\r\n", InArgs[2]);
  }
}

/*****************************************************************************!
 * Function : CommandRectifierSetAlarm
 *****************************************************************************/
void
CommandRectifierSetAlarm
(char** InArgs, int InArgsCount)
{
  uint8_t                               unitNumber;
  string                                alarmName;
  bool                                  alarmValue;
  Bay*                                  bay;
  RectifierType*                        rect;
  int                                   i;
  
  if ( InArgsCount < 6 ) {
    return;
  }
  unitNumber = atoi(InArgs[3]);

  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return;
  }
  rect = BayFindRectifier(bay, unitNumber);
  if ( NULL == rect ) {
    return;
  }
  for (i = 4; i < InArgsCount; i += 2 ) {
    if ( i + 2 > InArgsCount ) {
      break;
    }
    alarmName = InArgs[i];
    if ( StringEqualNoCase(InArgs[i+1], "true") || StringEqual(InArgs[i+1], "1") ) {
      alarmValue = true;
    } else if ( StringEqualNoCase(InArgs[i+1], "false") || StringEqual(InArgs[i+1], "0") ) {
      alarmValue = false;
    } else {
      break;
    }
    RectifierSetAlarmByName(rect, alarmName, alarmValue);
  }
  CLIDisplayConfirmMessage("Alarms set\n");
}

/*****************************************************************************!
 * Function: CommandRectifierSetMaxCurrentCapacity
 *****************************************************************************/
void
CommandRectifierSetMaxCurrentCapacity
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  uint16_t                            unitNumber;
  uint16_t                            maxCurr;
  NodeType*                           node;

  if ( InArgsCount < 6 ) {
    return;
  }

  unitNumber = atoi(InArgs[4]);
  maxCurr = atoi(InArgs[5]);

  node = NodeDataStoreFindExisting(NODE_TYPE_RECT, unitNumber);
  if ( node == NULL ) {
    CLIDisplayMessage("\r\nNode %d not found\n", unitNumber);
    return;
  }    

  node->rectifier.maxCurrentCapacity = maxCurr;

  CLIDisplayConfirmMessage("Current Capacity set for node %d\r\n", unitNumber); 
}

/*****************************************************************************!
 * Function : CommandRectifierSetRatedCurrent
 *****************************************************************************/
void
CommandRectifierSetRatedCurrent
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  uint16_t                            unitNumber;
  uint16_t                            maxCurr;
  NodeType*                           node;

  if ( InArgsCount < 5 ) {
    return;
  }

  unitNumber = atoi(InArgs[3]);
  maxCurr = atoi(InArgs[4]);

  node = NodeDataStoreFindExisting(NODE_TYPE_RECT, unitNumber);
  if ( node == NULL ) {
    CLIDisplayMessage("\r\nNode %d not found\n", unitNumber);
    return;
  }    

  node->rectifier.ratedOutputCurrent= maxCurr;

  CLIDisplayConfirmMessage("Rated Current set for node %d\r\n", unitNumber); 
}

/*****************************************************************************!
 * Function : CommandRectifierSetParams
 *****************************************************************************/
void
CommandRectifierSetParams
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint16_t                            nodeNumber;
  uint16_t                            volts;
  uint16_t                            limit;
  uint16_t                            hvs;
  uint8_t                             flags;
  NodeType*                           node;

  if ( InArgsCount < 8 ) {
    return;
  }
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }

  volts = atoi(InArgs[4]);
  limit = atoi(InArgs[5]);
  hvs   = atoi(InArgs[6]);
  flags = StringToInteger(InArgs[7], 8);

  //ESNASendRectNormalParameters(nodeNumber, volts, limit, hvs, flags);
  CLIDisplayConfirmMessage("\r\nNode %d normal parameters\r\n", nodeNumber);
#endif
}

/*****************************************************************************!
 * Function : CommandRectifierSetAlarmStats
 *****************************************************************************/
void
CommandRectifierSetAlarmStats
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint16_t                            nodeNumber;
  uint32_t                            alarms;
  uint16_t                            current;
  uint8_t                             status;
  int                                 base;
  NodeType*                           node;

  char*                               statusString;
  char*                               alarmsString;
  char*                               currentString;

  if ( InArgsCount < 7 ) {
    return;
  }
  currentString = InArgs[6];
  alarmsString =  InArgs[4];
  statusString =  InArgs[5];
  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nNode number must be a non-0 number\r\n");
    return;
  }
  node = NodeDataStoreGetNode(nodeNumber);
  if ( NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node %d\r\n", nodeNumber);
    return;
  }
    
  //!
  base = 10;
  if ( !strncmp("0x", alarmsString, 2) || !strncmp("0X", alarmsString, 2)) {
    base = 16;
  }
  alarms = strtol(alarmsString, NULL, base);

  //!
  base = 10;
  if ( !strncmp("0x", statusString, 2) || strncmp("0X", statusString, 2) ) {
    base = 16;
  }
  status = strtol(statusString, NULL, base);

  //!
  base = 10;
  if ( !strncmp("0x", currentString, 2) || strncmp("0X", currentString, 2) ) {
    base = 16;
  }
  current = strtol(currentString, NULL, base);
    
  ESNASendRectUpdate(nodeNumber, alarms, status, current);
  CLIDisplayConfirmMessage("\r\nNode %d update\r\n", nodeNumber);
#endif
}

/*****************************************************************************!
 * Function : CommandRectifierSetInputVolts
 *****************************************************************************/
void
CommandRectifierSetInputVolts
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint16_t                            nodeNumber;
  uint16_t                            volts;
  uint16_t                            designator;
  NodeType*                           node;
    
  if ( InArgsCount < 6 ) {
    return;
  }

  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nInvalid NodeNumber \"%s\"\r\n", InArgs[3]);
    return;
  }

  node = NodeDataStoreGetNode(nodeNumber);
  if (NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node \"%s\"\r\n", InArgs[3]);
    return;
  }
    
  volts = atoi(InArgs[4]);
  if ( strlen(InArgs[5]) != 2 ) {
    return;
  }

  if ( !strcasecmp(InArgs[5], "ac") ) {
    designator = 0x4143;
  } else if ( !strcasecmp(InArgs[5], "dc") ) {
    designator = 0x4443;
  } else {
    CLIDisplayMessage("\r\nInvalid voltage type \"%s\"\r\n", InArgs[5]);
    return;
  }
  CLIDisplayConfirmMessage("\r\nNode %d Input Voltage set\r\n", nodeNumber);
  ESNASendRectRatedInputVoltage(nodeNumber, volts, designator);
#endif
}
    
/*****************************************************************************!
 * Function : CommandRectifierSetOutputVolts
 *****************************************************************************/
void
CommandRectifierSetOutputVolts
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  uint16_t                            nodeNumber;
  uint16_t                            volts;
  uint16_t                            designator;
  NodeType*                           node;
    
  if ( InArgsCount < 6 ) {
    return;
  }

  nodeNumber = atoi(InArgs[3]);
  if ( nodeNumber == 0 ) {
    CLIDisplayMessage("\r\nInvalid NodeNumber \"%s\"\r\n", InArgs[3]);
    return;
  }

  node = NodeDataStoreGetNode(nodeNumber);
  if (NULL == node ) {
    CLIDisplayMessage("\r\nCould not find node \"%s\"\r\n", InArgs[3]);
    return;
  }
    
  volts = atoi(InArgs[4]);
  if ( strlen(InArgs[5]) != 2 ) {
    return;
  }

  if ( !strcasecmp(InArgs[5], "ac") ) {
    designator = 0x4143;
  } else if ( !strcasecmp(InArgs[5], "dc") ) {
    designator = 0x4443;
  } else {
    CLIDisplayMessage("\r\nInvalid voltage type \"%s\"\r\n", InArgs[5]);
    return;
  }
  CLIDisplayConfirmMessage("\r\nNode %d Output Voltage set\r\n", nodeNumber);
  ESNASendRectRatedOutputVoltage(nodeNumber, volts, designator);
#endif
}
    
/*****************************************************************************!
 * Function : CommandRectifierSetCurrent
 *****************************************************************************/
void
CommandRectifierSetCurrent
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  Bay*                                  bay;
  RectifierType*                        rectifier;
  uint8_t                               unitNumber;
  uint32_t                              current;

  if ( InArgsCount < 5 ) {
    return;
  }
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return;
  }
  current = atoi(InArgs[4]);  
  if ( StringEqualNoCase(InArgs[3], "all") ) {
    BaySetAllRectifierCurrents(bay, (float)current);
    CLIDisplayConfirmMessage("All Rectifier currents set to %.0f\r\n", (float)current);
    return;
  }
  unitNumber = atoi(InArgs[3]);
  rectifier = BayFindRectifier(bay, unitNumber);
  if ( NULL == rectifier ) {
    return;
  }

  rectifier->outputCurrent = (float)current;
  CLIDisplayConfirmMessage("Rectifier:%d current set to %.03f\r\n", unitNumber, rectifier->outputCurrent);
}

/*****************************************************************************!
 * Function : CommandDebug
 *****************************************************************************/
static void
CommandDebug
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 2 ) {
    return;
  }
  if ( strcasecmp(InArgs[1], "set") == 0 ) {
    CommandDebugSet(InArgs, InArgsCount);
  } else if ( strcasecmp(InArgs[1], "canif") == 0 ) {
    CommandDebugCANIF(InArgs, InArgsCount);
  } else if ( strcasecmp(InArgs[1], "threads") == 0 ) {
    CommandDebugThreads(InArgs, InArgsCount);
  } else if ( strcasecmp(InArgs[1], "dumpio" ) == 0 ) {
    CommandDebugDumpIO(InArgs, InArgsCount);
  }
}

/******************************************************************************!
 * Function : CommandDebugDumpIO
 ******************************************************************************/
void
CommandDebugDumpIO
(
 char**                         InArgs,
 int                            InArgsCount
)
{
  if ( InArgsCount != 3 ) {
    return;
  }
  if ( strcasecmp(InArgs[2], "yes") == 0 || strcasecmp(InArgs[2], "on") == 0 ) {
    //CANInterfaceDumpIOSet(true);
  } else if ( strcasecmp(InArgs[2], "no") || strcasecmp(InArgs[2], "off") == 0 ) {
    //CANInterfaceDumpIOSet(false);
  }
}

/*****************************************************************************!
 * Function ; CommandDebugThreads
 *****************************************************************************/
void
CommandDebugThreads
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount != 2 ) {
    return;
  }

  ThreadHandle                          threadHandle;
  string                                threadName;

  for ( threadHandle = ThreadGetFirstHandle(); 
        threadHandle != 0;
        threadHandle = ThreadGetNextHandle(threadHandle) ) {
    threadName = ThreadGetName(threadHandle);
    CLIDisplayMessage("\rThread %3d : %s\r\n", threadHandle, threadName);
  }
}

/*****************************************************************************!
 * Function ; CommandDebugCANIF
 *****************************************************************************/
void
CommandDebugCANIF
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  char                                status[2];
  if ( GetCANInterfaceStatus(status) ) {
    CLIDisplayMessage("\r\n\r\nCAN Status : %d%d\r\n\r\n", status[0], status[1]);
  } else {
    CLIDisplayMessage("Could not get CAN Interface Status\n");
  }
#endif
}

/*****************************************************************************!
 * Function : CommandDebugSet
 *****************************************************************************/
void
CommandDebugSet
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 3 ) {
    return;
  }
  if ( strcasecmp(InArgs[2], "dumpoutmessages") == 0 ) {
    CommandDebugSetDumpOutMessages(InArgs, InArgsCount);
  } 
}

/*****************************************************************************!
 * Function : CommandDebugSetDumpOutMessages
 *****************************************************************************/
void
CommandDebugSetDumpOutMessages
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  bool                                value;
    
  if ( InArgsCount != 4 ) {
    return;
  }
  if ( !strcasecmp(InArgs[3], "true") ) {
    value = true;
  } else if ( !strcasecmp(InArgs[3], "false") ) {
    value = false;
  } else {
    return;
  }
  SetDumpOutMessages(value);    
  CLIDisplayMessage(" Dump Out Messages : %s\r\n", value ? "On" : "Off");
#endif
}

/*****************************************************************************!
 * Function : CommandSystem 
 *****************************************************************************/
static void
CommandSystem
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  if ( InArgsCount < 2 ) {
    CLIDisplayMessage(" System :\r\n");
    CLIDisplayMessage(" Downslope compensation : %s\r\n", systemConfig.Flags.bit.DisableDownSlope ? "false" : "true");
    CLIDisplayMessage(" Load Sharing           : %s\r\n", systemConfig.Flags.bit.DisableLoadShare ? "false" : "true");
    CLIDisplayMessage(" Power Share            : %s\r\n", systemConfig.Flags.bit.PowerShare ? "true" : "false");
    CLIDisplayMessage(" Temp in Fahrenheit     : %s\r\n", systemConfig.Flags.bit.TempInFahrenheit ? "true" : "false");
    CLIDisplayMessage(" System is 24 Volts     : %s\r\n", systemConfig.Flags.bit.SystemIs24Volts ? "true" : "false");
    CLIDisplayMessage(" Power Share Capacity   : %d\r\n", systemConfig.PowerShareCapacity);
    return;
  }
#endif

  if ( !strcasecmp(InArgs[1], "24volts") ) {
    CommandSystem24Volts(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "downslope") ) {
    CommandSystemDownSlope(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "loadshare") ) {
    CommandSystemLoadShare(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "fahrenheit") ) {
    CommandSystemFahrenheit(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "powershare") ) {
    CommandSystemPowerShare(InArgs, InArgsCount);
  } else if ( !strcasecmp(InArgs[1], "powersharecap") ) {
    CommandSystemPowerShareCapacity(InArgs, InArgsCount);
  } else {
    CLIDisplayMessage("\"%s\" is an unknown system command\r\n", InArgs[1]);
  }
}

/*****************************************************************************!
 * Function : CommandSystem24Volts
 *****************************************************************************/
static void
CommandSystem24Volts
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( !strcasecmp(InArgs[2], "true") ) {
    systemConfig.Flags.bit.SystemIs24Volts = 1;
  } else if ( !strcasecmp(InArgs[2], "false") ) {
    systemConfig.Flags.bit.SystemIs24Volts = 0;
  }
#endif
}

/*****************************************************************************!
 * Function : CommandSystemDownSlope
 *****************************************************************************/
static void
CommandSystemDownSlope
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( !strcasecmp(InArgs[2], "true") ) {
    systemConfig.Flags.bit.DisableDownSlope = 0;
  } else if ( !strcasecmp(InArgs[2], "false") ) {
    systemConfig.Flags.bit.DisableDownSlope = 1;
  }
#endif
}

/*****************************************************************************!
 * Function : CommandSystemLoadShare
 *****************************************************************************/
static void
CommandSystemLoadShare
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( !strcasecmp(InArgs[2], "true") ) {
    systemConfig.Flags.bit.DisableLoadShare = 0;
  } else if ( !strcasecmp(InArgs[2], "false") ) {
    systemConfig.Flags.bit.DisableLoadShare = 1;
  }
#endif
}

/*****************************************************************************!
 * Function : CommandSystemPowerShare
 *****************************************************************************/
static void
CommandSystemPowerShare
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( !strcasecmp(InArgs[2], "true") ) {
    systemConfig.Flags.bit.PowerShare = 1;
  } else if ( !strcasecmp(InArgs[2], "false") ) {
    systemConfig.Flags.bit.PowerShare = 0;
  }
#endif
}

/*****************************************************************************!
 * Function : CommandSystemPowerShareCapacity
 *****************************************************************************/
static void
CommandSystemPowerShareCapacity
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  if ( InArgsCount < 3 ) {
    return;
  }

  uint16_t                            n = atoi(InArgs[2]);
  systemConfig.PowerShareCapacity = n;
#endif
}

/*****************************************************************************!
 * Function : CommandSystemFahrenheit
 *****************************************************************************/
static void
CommandSystemFahrenheit
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( !strcasecmp(InArgs[2], "true") ) {
    systemConfig.Flags.bit.TempInFahrenheit = 1;
  } else if ( !strcasecmp(InArgs[2], "false") ) {
    systemConfig.Flags.bit.TempInFahrenheit = 0;
  }
#endif
}

/*****************************************************************************!
 * Function : CommandHelp
 *****************************************************************************/
void
CommandHelp
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  (void)InArgs;
  (void)InArgsCount;

  CLIDisplayMessage("\r\nHELP\r\n");
  CLIDisplayMessage("  %sbay%s add type number                                              - Add Bay\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %sbay%s show                                                         - List Bays\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %snode%s                                                             - List active nodes\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %snode%s set apprev unit major minor bugFix {build}                  - Update Node application revision\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %snode%s set bootrev unit major minor bugFix {build}                 - Update Node boot loader revision\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %snode%s set ctext nodeNumber \"text\"                                 - Set Customer Text\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %snode%s set misc nodeNumber desc serial slave unit                  - Set the misc information for a node\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %snode%s set partn nodeNumber number                                 - Set Customer Text\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %srect%s add unitNumber                                              - Add rectifier to data store\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %srect%s show unitNumber                                             - Show rectifier\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %srect%s set current {unitNumber | all}  current                     - Set rectifier output current\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %srect%s set alarm unitnumber [alarm {true|false}]+                  - Set rectifier alarms\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %srect%s set serial unitNumber serialNumber                          - Set node serial number\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %srect%s on { unitNumber | all }                                     - Turn on a rectifier\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %srect%s off { unitNumber | all }                                    - Turn off a rectifier\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %sdsm%s add unitNumber                                               - Add DSM to data store\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %sdsm%s remove unitNumber                                            - Remove DSM\r\n", ColorRed, ColorWhite); 
  CLIDisplayMessage("  %sdsm%s show unitNumber                                              - Show DSM\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %sdsm%s set serial unitNumber serialNumber                           - Set node serial number\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %sdist%s add unitNumber slotNumber type pole current                 - Add a distribution point to a DSM\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %sdist%s remove unitNumber slotNumber                                - Remove a distribution point from a DSM\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %sdist%s set unitNumber slotNumber {open|close|current value}        - Set a distribution point\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %ssystem%s                                                           - Display settable system config values\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssystem%s downslope {true | false}                                  - Enable/Disable downslope compensation\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssystem%s powershare {true | false}                                 - Enable/Disable power sharing\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssystem%s loadshare {true | false}                                  - Enable/Disable load sharing\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssystem%s fahrenheit {true | false}                                 - Set temp mearsurements to fahrenheit\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssystem%s 24volts {true | false}                                    - Set system to 24 volts\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %sdebug%s set dumpoutmessages {true | false}                         - Dump the outbound ESNA messages to the nodes\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %ssend%s {BCM|NODE} ext bytes                                        - Send BCM|RECT message (bytes are 2 hex digits or 8 binary digits)\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %sread%s node NodeNumber address byteCount                           - Send a memory read request to a node\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("\r\n");
  CLIDisplayMessage("  %ssim%s version                                                      - Display Version Number\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s start                                                        - Start the simulator\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s pause                                                        - Pause the simulator\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s stop                                                         - Stop and reset the simulator\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s show                                                         - Show the current state of the simulator\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s show packets on|off                                          - Display outgoing/incoming ESNA CAN packets\r\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s CAN stop                                                     - Turn off ESNA CAN Procesing\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s CAN start                                                    - Turn on ESNA CAN procsssing (if not already started)\n", ColorRed, ColorWhite);
  CLIDisplayMessage("  %ssim%s CAN restart                                                  - Restart ESNA CAN processing\n", ColorRed, ColorWhite);
}

/*****************************************************************************!
 * Function : CommandDataStore
 *****************************************************************************/
void
CommandDataStore
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  uint16_t                            nodeCount;
    
  (void)InArgs;
  (void)InArgsCount;

  nodeCount = NodeDataStoreGetCount();

  CLIDisplayMessage("\r\n%d Node%s\r\n", nodeCount, nodeCount != 1 ? "s" : "");
}

/*****************************************************************************!
 * Function : CommandBay
 *****************************************************************************/
void
CommandBay
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 2 ) {
    return;
  }

  if ( strcasecmp(InArgs[1], "show") == 0 ) {
    CommandBayShow(InArgs, InArgsCount);
  } else if ( strcasecmp(InArgs[1], "add") == 0 ) {
    CommandBayAdd(InArgs, InArgsCount);
  } else if ( StringEqualNoCase(InArgs[1], "set") ) {
    CommandBaySet(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandBaySet
 *****************************************************************************/
void
CommandBaySet
(char** InArgs, int InArgsCount)
{
  if ( InArgsCount < 3 ) {
    return;
  }
  if ( StringEqualNoCase(InArgs[2], "name") ) {
    CommandBaySetName(InArgs, InArgsCount);
  }
}

/*****************************************************************************!
 * Function : CommandBaySetName
 *****************************************************************************/
void
CommandBaySetName
(char** InArgs, int InArgsCount)
{
  if ( InArgsCount < 4 ) {
    return;
  }
  Bay*                                  bay;

  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return;
  }
  BaySetName(bay, InArgs[3]);
  CLIDisplayConfirmMessage("Bay %d name set to : %s\n", 1, InArgs[3]);
}


/******************************************************************************!
 * Function : CommandBayAdd
 ******************************************************************************/
void
CommandBayAdd
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  BayType                               type = BayTypeSupplemental;
  string                                typeString;
  int                                   index = mainBays->bayCount;
  char                                  bayName[8];
  Bay*                                  bay;

  if ( InArgsCount < 4 ) {
    return;
  }
  typeString = InArgs[2];
  index = atoi(InArgs[3]);
  if ( StringEqualNoCase(typeString, "Supplemental") ) {
    type = BayTypeSupplemental;
  } else if ( StringEqualNoCase(typeString, "Distribution") ) {
    type = BayTypeDistribution;
  }

  if ( mainBays->bayCount + 1 > BAY_MAX_COUNT ) {
    CLIDisplayMessage("Can only have up to %d Bay%s", BAY_MAX_COUNT, BAY_MAX_COUNT > 1 ? "s" : "");
    return;
  }
  sprintf(bayName, "Bay %d", index);
  bay = CreateBay(bayName, type, 1);
  BaySetAddBay(mainBays, bay);
  CLIDisplayConfirmMessage("\r\nAdding %s\r\n", bayName);
}

/******************************************************************************!
 * Function : CommandBayShow
 ******************************************************************************/
void
CommandBayShow
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount == 2 ) {
    CommandBayShowAll();
    return;
  }
}

/*****************************************************************************!
 * Function : CommandBayShowAll
 *****************************************************************************/
void
CommandBayShowAll()
{
  int                                 i;

  CLIDisplayMessage("      INDEX                  NAME          TYPE  DSMS  RECTIFIERS\n");    
  for (i = 0; i < mainBays->bayCount; i++) {
    CLIDisplayMessage("Bay : %5d  %20s  %s  %4d  %10d\n", mainBays->bays[i]->index,
                      mainBays->bays[i]->name,
                      BayTypeToString(mainBays->bays[i]->type), 
                      BayGetDSMCount(mainBays->bays[i]),
                      BayGetRectifierCount(mainBays->bays[i]));
  }
}

/*****************************************************************************!
 * Function : CommandNode
 *****************************************************************************/
void
CommandNode
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  char*                               subCommand;

  if ( InArgsCount < 2 ) {
    DisplayNodeInformation();
    return;
  }

  subCommand = InArgs[1];

  if ( !strcasecmp(subCommand, "set") ) {
    CommandNodeSet(InArgs, InArgsCount);
    return;
  }

  CLIDisplayMessage("\r\n%s is invalid subcommand to Node\r\n", subCommand);
}    

/*****************************************************************************!
 * Function : CommandNodeSet
 *****************************************************************************/
void
CommandNodeSet
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  char*                               setCommand;
  if ( InArgsCount < 3 ) {
    return;
  }

  setCommand = InArgs[2];
  if ( !strcasecmp(setCommand, "apprev") ) {
    CommandNodeSetAppRevision(InArgs, InArgsCount);
    return;
  }
  if ( !strcasecmp(setCommand, "bootrev") ) {
    CommandNodeSetBootRevision(InArgs, InArgsCount);
    return;
  }
  if ( !strcasecmp(setCommand, "partn" ) ) {
    CommandNodeSetPartNumber(InArgs, InArgsCount);
    return;
  }
  if ( !strcasecmp(setCommand, "ctext" ) ) {
    CommandNodeSetCustomerText(InArgs, InArgsCount);
    return;
  }
  if ( !strcasecmp(setCommand, "misc" ) ) {
    CommandNodeSetMiscInfo(InArgs, InArgsCount);
    return;
  }

  CLIDisplayMessage("\r\n%s is invalid subcommand to Node Set\r\n", setCommand);
}

/*****************************************************************************!
 * Function : CommandNodeSetBootRevision
 *****************************************************************************/
void
CommandNodeSetBootRevision
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  ESNANodeStatus                      status;
  uint8_t                             unitNumber;
  uint8_t                             major, minor, bugFix, build;
    
  if ( InArgsCount < 8 ) {
    return;
  }

  unitNumber = atoi(InArgs[3]);
  major = atoi(InArgs[4]);
  minor = atoi(InArgs[5]);
  bugFix = atoi(InArgs[6]);
  build = atoi(InArgs[7]);

  if ( !NodeDataStoreNodeExists(NODE_TYPE_NONE, unitNumber) ) {
    CLIDisplayMessage("\r\nNode Unit:%d does not exist\r\n", unitNumber);
    return;
  }
  status = ESNANodeSetBootVersion(unitNumber, major, minor, bugFix, build);
  if ( status == ESNA_NODE_STATUS_OK ) {
    CLIDisplayConfirmMessage("\r\nNode Boot Version Set\r\n");
  }
#endif
}
            
/*****************************************************************************!
 * Function : CommandNodeSetAppRevision
 *****************************************************************************/
void
CommandNodeSetAppRevision
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
#if 0
  ESNANodeStatus                      status;
  uint8_t                             unitNumber;
  uint8_t                             major, minor, bugFix, build;
    
  if ( InArgsCount < 9 ) {
    return;
  }

  unitNumber = atoi(InArgs[3]);
  major = atoi(InArgs[4]);
  minor = atoi(InArgs[5]);
  bugFix = atoi(InArgs[6]);
  build = atoi(InArgs[7]);

  if ( !NodeDataStoreNodeExists(NODE_TYPE_NONE, unitNumber) ) {
    CLIDisplayMessage("\r\nNode Unit:%d does not exist\r\n", unitNumber);
    return;
  }
  status = ESNANodeSetAppVersion(unitNumber, major, minor, bugFix, build);
  if ( status == ESNA_NODE_STATUS_OK ) {
    CLIDisplayConfirmMessage("\r\nNode Application Version Set\r\n");
  }
#endif
}
            
/*****************************************************************************!
 * Function : CommandRectifierAdd
 * Purpose  : Add a rectifier to a bay
 *****************************************************************************/
void
CommandRectifierAdd
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  uint8_t                             unitNumber;
  ESNANodeStatus                      status;        
  Bay*                                bay;
  RectifierType*                      rectifier;

  if ( InArgsCount < 3 ) {
    return ;
  }
            
  // Get the bay and rectifier id components
  unitNumber = atoi(InArgs[2]);

  bay = BaySetFindBayByIndex(mainBays, 1);

  if ( BayRectifierExistsAtSlot(bay, unitNumber) ) {
      CLIDisplayMessage("A rectifier exists in slot %d\r\n", unitNumber);
      return;
  }
    
  //! If the node exists, we are done
  if ( NodeDataStoreFindExistingInBay(NODE_TYPE_RECT, 1, unitNumber) ) {
    CLIDisplayMessage("\r\nNode Exists\r\n");
    return;
  }
    
  // Add the rectifier
  status = ESNANodeRectifierAdd(1, unitNumber);
   
    
  if ( status != ESNA_NODE_STATUS_OK ) {
    printf("\r\nCould not add Node : %d\n",status);
    return;
  }
  CLIDisplayConfirmMessage("%sRectifier %d added to Bay 1%s\r\n", ColorYellow, unitNumber, ColorWhite);
  rectifier = BayFindRectifier(bay, unitNumber);
  if ( NULL == rectifier ) {
    return;
  }
  rectifier->power = true;
}

/*****************************************************************************!
 * Function : CommandDSMAdd
 *****************************************************************************/
void
CommandDSMAdd
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  Bay*                                  bay;
  string                                productNumber;
  char                                  serialNumberString[16];
  string                                maxPointsPerBus;
  if ( InArgsCount < 3 ) {
    return;
  }

  bay = BaySetGetFirstBay(mainBays);
  if ( NULL == bay ) {
    return;
  }
  if ( bay->type == BayTypeSupplemental ) {
    productNumber = mainDSMProductNumber;
    maxPointsPerBus = "24";
  } else {
    productNumber = mainDSMDistOnlyProductNumber;
    maxPointsPerBus = "12";
  }
  sprintf(serialNumberString, "%d", GetNextSerialNumber());
  if ( DeviceDSMAdd(InArgs[2],  serialNumberString, productNumber, maxPointsPerBus) ) {
    CLIDisplayConfirmMessage("%sDSM %s added to Bay 1%s\r\n", ColorYellow, InArgs[2], ColorWhite);
  }
}

/*****************************************************************************!
 * Function : CommandDSMRemove
 *****************************************************************************/
void
CommandDSMRemove
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
  if ( InArgsCount < 3 ) {
    return;
  }

  if ( DeviceDSMRemove(InArgs[2]) ) {
    CLIDisplayConfirmMessage("%sDSM %s remove from Bay 1%s\r\n", ColorYellow, InArgs[2], ColorWhite);
  }
}

/*****************************************************************************!
 * Function : StoreCommandHistory
 *****************************************************************************/
void
StoreCommandHistory()
{
  CommandHistoryIndex++;
  if ( CommandHistoryIndex >= COMMAND_HISTORY_SIZE ) {
    CommandHistoryIndex = 0;
  }
  CommandHistoryIndexLast = CommandHistoryIndex;
  strcpy(CommandHistory[CommandHistoryIndex], CommandBuffer);
}

/*****************************************************************************!
 * Function : DisplayNodeInformation
 *****************************************************************************/
void
DisplayNodeInformation()
{
  NodeType*                             node;
  int                                   i;
  RectifierType*                        rect;
  Bay*                                  bay;
  DSMType*                              dsm;
  string                                startColor;
  
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return;
  }

  CLIDisplayMessage("\r\n%s%5s                                                                      %s\r\n", HeadingStyle, bay->name, ColorWhite);
  if ( bay->maxRectifierCount > 0 ) {
    CLIDisplayMessage("%sRECTIFIERS                                                                 %s\r\n", HeadingStyle, ColorWhite);
    CLIDisplayMessage("%sUNIT   SERIAL #      APP REV     BOOT REV  ADD  ST  ON LINE  POWER  CURRENT%s\r\n", HeadingStyle, ColorWhite);
    for (i = 0; i < BAY_MAX_RECTIFIER_COUNT; i++ ) {
      rect = bay->rectifiers[i];
      if ( rect == NULL ) {
        CLIDisplayMessage("%4d\r\n", i + 1);
        continue;
      }
      
      node = rect->parentNode;
      startColor = ColorGreen;
      if ( !node->rectifier.power ) {
        startColor = ColorYellow;
      } else if ( node->nodeTimedOut ) {
        startColor = ColorRed;
      }
      CLIDisplayMessage("%4d  %s%8u  %2d.%02d.%02d %02d  %2d.%02d.%02d.%02d  %3d  %2d %s    %s     %4.0f%s\r\n",
                        node->location.unitNumber,
                        startColor,
                        node->serialNumber,
                        node->revisionInfo.appMajor,
                        node->revisionInfo.appMinor,
                        node->revisionInfo.appBugFix,
                        node->revisionInfo.appBuild,
                        node->revisionInfo.bootMajor,
                        node->revisionInfo.bootMinor,
                        node->revisionInfo.bootBugFix,
                        node->revisionInfo.bootBuild,
                        node->ESNACANAddress,
                        node->startUpState,
                        !node->nodeTimedOut ? "    True" : "   False",
                        node->rectifier.power ? " On" : "Off",
                        node->rectifier.outputCurrent,
                        ColorWhite);
    }
    CLIDisplayMessage("\r\n");
  }
  CLIDisplayMessage("%sDSMS                                                                       %s\r\n", HeadingStyle, ColorWhite);
  CLIDisplayMessage("%sUNIT   SERIAL #      APP REV     BOOT REV  ADD  ST  ON LINE                %s\r\n", HeadingStyle, ColorWhite);
  for (i = 0; i < bay->maxDSMCount; i++ ) {
    dsm = bay->dsms[i];
    if ( dsm == NULL ) {
      CLIDisplayMessage("%4d\r\n", i + 1);
      continue;
    }
    node = dsm->parentNode;
    startColor = ColorGreen;
    if ( node->nodeTimedOut ) {
      startColor = ColorRed;
    }
    CLIDisplayMessage("%4d  %s%8u  %2d.%02d.%02d %02d  %2d.%02d.%02d.%02d  %3d  %2d %s%s\r\n",
                      node->location.unitNumber,
                      startColor,
                      node->serialNumber,
                      node->revisionInfo.appMajor,
                      node->revisionInfo.appMinor,
                      node->revisionInfo.appBugFix,
                      node->revisionInfo.appBuild,
                      node->revisionInfo.bootMajor,
                      node->revisionInfo.bootMinor,
                      node->revisionInfo.bootBugFix,
                      node->revisionInfo.bootBuild,
                      node->ESNACANAddress,
                      node->startUpState,
                      !node->nodeTimedOut ? "    True" : "   False",
                      ColorWhite);
  }
  CLIDisplayMessage("\r\n");
}

/*****************************************************************************!
 * Function : CommandDSMSetSerialNumber
 *****************************************************************************/
void
CommandDSMSetSerialNumber
(char** InArgs, int InArgsCount)
{
  Bay*                                  bay;
  uint8_t                               unitNumber;
  DSMType*                              dsm;
  uint32_t                              serialNumber;

  if ( InArgsCount < 5 ) {
    return;
  }

  unitNumber = atoi(InArgs[3]);
  serialNumber = atoi(InArgs[4]);
  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return;
  }
  dsm = BayFindDSM(bay, unitNumber);
  if ( NULL == dsm ) {
    return;
  }
  
  dsm->parentNode->serialNumber = serialNumber;
  CLIDisplayConfirmMessage("Bay:1 DSM:%d serial number set to %d\r\n", unitNumber, serialNumber);
}
 
/*****************************************************************************!
 * Function : CommandRectifierSetSerialNumber
 *****************************************************************************/
void
CommandRectifierSetSerialNumber
(char** InArgs, int InArgsCount)
{
  RectifierType*                        rect;
  Bay*                                  bay;
  uint8_t                               unitNumber;
  uint32_t                              serialNumber;

  if ( InArgsCount < 5 ) {
    return;
  }

  unitNumber = atoi(InArgs[3]);
  serialNumber = atoi(InArgs[4]);

  bay = BaySetFindBayByIndex(mainBays, 1);
  if ( NULL == bay ) {
    return;
  }
  rect = BayFindRectifier(bay, unitNumber);
  if ( NULL == rect ) {
    return;
  }
  
  rect->parentNode->serialNumber = serialNumber;
  CLIDisplayConfirmMessage("Rectifier:%d serial number set to %d\r\n", unitNumber, serialNumber);
}
 
/*****************************************************************************!
 * Function : CommandNodeSetCustomerText
 *****************************************************************************/
void
CommandNodeSetCustomerText
(
 char**                         InArgs,
 int                            InArgsCount
)
{
}

/*****************************************************************************!
 * Function : CommandNodeSetPartNumber
 *****************************************************************************/
void
CommandNodeSetPartNumber
(
 char**                         InArgs,
 int                            InArgsCount
)
{
}

/*****************************************************************************!
 * Function : CommandNodeSetMiscInfo 
 *****************************************************************************/
void
CommandNodeSetMiscInfo
(
 char**                                 InArgs,
 int                                    InArgsCount
)
{
}

/*****************************************************************************!
 * Function : CommandRead
 *****************************************************************************/
static void
CommandRead
(char** InArgs, int InArgsCount)
{
  string                                filename;
  bool                                  rvalue;
  
  if ( InArgsCount < 2 ) {
    return;
  }
  filename = InArgs[1];

  rvalue = DeviceFileRead(filename);
  if ( rvalue ) {
    CLIDisplayMessage("%s read\n", filename);
  }
}

/*****************************************************************************!
 * Function : CommandSave
 *****************************************************************************/
static void
CommandSave
(char** InArgs, int InArgsCount)
{
  if ( mainDeviceFilename ) {
    if ( BaySaveFile(mainBays->bays[0], mainDeviceFilename) ) {
      CLIDisplayMessage("File \"%s\" saved\n", mainDeviceFilename);
    }
  }
}

/*****************************************************************************!
 * Function : CLIDisplayMessage
 *****************************************************************************/
void
CLIDisplayMessage
(char* format, ...)
{
  va_list                       la;

  if ( CLIWriteLog ) {
    FILE*                       file = fopen(CLILogFilename, "a+b");
    va_start(la, format);
    vfprintf(file, format, la);
    fclose(file);
  }
  if ( CLIVerboseInterface ) {
    va_start(la, format);
    vprintf(format, la);
    va_end(la);
  }
}

/*****************************************************************************!
 * Function : CLIDisplayConfirmMessage
 *****************************************************************************/
void
CLIDisplayConfirmMessage
(char* format, ...)
{
  va_list                       la;

  if ( CLIWriteLog ) {
    FILE*                       file = fopen(CLILogFilename, "a+b");
    va_start(la, format);
    vfprintf(file, format, la);
    fclose(file);
  }
  if ( CLIVerboseInterface ) {
    va_start(la, format);
    printf("%s", ColorYellow);
    vprintf(format, la);
    printf("%s", ColorWhite);
    va_end(la);
  }
}

/*****************************************************************************!
 * Function : CLILogMessage
 *****************************************************************************/
void
CLILogMessage
(char* format, ...)
{
  va_list                       la;

  if ( CLIVerboseInterface ) {
    va_start(la, format);
    printf("\r\n");
    vprintf(format, la);
    printf("%s%s", CommandPrompt, CommandBuffer);
    fflush(stdout);
    va_end(la);
  }

  FILE*                 file = fopen(CLILogFilename, "a+b");
  va_start(la, format);
  vfprintf(file, format, la);
  fclose(file);
}

/*****************************************************************************!
 * Function : CommandSimulatorCAN
 *****************************************************************************/
 static void
CommandSimulatorCAN
(char** InArgs, int InArgsCount)
{
  string                              command;
  if ( InArgsCount < 3 ) {
    return;
  }

  command = InArgs[2];
  if ( StringEqualNoCase(command, "stop") ) {
    CommandSimulatorCANStop();
    return;
  }
  if ( StringEqualNoCase(command, "start") ) {
    CommandSimulatorCANStart();
    return;
  }
  if ( StringEqualNoCase(command, "restart") ) {
    CommandSimulatorCANRestart();
    return;
  }
}
/*****************************************************************************!
 * Function : CommandSimulatorCANStop
 *****************************************************************************/
static void
CommandSimulatorCANStop
()
{

}

/*****************************************************************************!
 * Function : CommandSimulatorCANStart
 *****************************************************************************/
static void
CommandSimulatorCANStart
()
{

}

/*****************************************************************************!
 * Function : CommandSimulatorCANRestart
 *****************************************************************************/
static void
CommandSimulatorCANRestart
()
{
  CANIOThreadRestart(); 
}


