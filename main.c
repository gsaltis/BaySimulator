/******************************************************************************
 * FILE NAME    : main.c
 * DATE         : January 15 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 ******************************************************************************/

/*****************************************************************************!
 * Global Headers
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include <StringUtils.h>
#include <MemoryManager.h>
#include <ascii.h>

/******************************************************************************!
 * Local Headers
 ******************************************************************************/
#include "CANInterface.h"
#include "ESNACANInterface.h"
#include "CANIOThread.h"
#include "CLIThread.h"
#include "NodeDataStore.h"
#include "ESNACANNodeType.h"
#include "System.h"
#include "ProcessInputMessageThread.h"
#include "NodeManagerThread.h"
#include "HTTPServerThread.h"
#include "main.h"
#include "SystemTime.h"
#include "BayFile.h"
#include "DataFile.h"
#include "DeviceFile.h"
#include "SIMThread.h"
#include "Version.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define DEFAULT_WEB_DIRECTORY   "/etc/baysim/web"
#define APPLICATION_NAME        "Bay Simulator"

/******************************************************************************!
 * Local Functions
 ******************************************************************************/
void
ReadConfigFile
();

void
ParseConfigFileLine
(string InLine);

void
mainProcessArgs
(int arg, char** argv);

void
ReadDataFile
();

void
DisplayUsage
();

void
shutdown
(void);

/******************************************************************************!
 * Local Data
 ******************************************************************************/
string                          mainDataFilename;
string                          mainDeviceFilename = NULL;
BaySet*                         mainBays = NULL;
pthread_t                       cliThread;
pthread_t                       canWriteThread;
pthread_t                       canReadThread;
pthread_t                       processInputThread;
pthread_t                       httpThread;
pthread_t                       simThread;
pthread_t                       nodeManagerThread;
int32_t                         mainSerialNumber = 3161000989;
string                          mainVersion;
string                          mainBuildDate = __DATE__ " " __TIME__;
string                          mainWebBaseDirectory;
CANInterface*                   canInterface;
char*                           mainDSMProductNumber = "524982";
char*                           mainDSMDistOnlyProductNumber = "524983";

/******************************************************************************!
 * Function : main
 ******************************************************************************/
int main(int arg, char** argv)
{
  string                        s;

  CLIVerboseInterface = false;;

  atexit(shutdown);
  mainVersion = StringCopy(GetVersionString());
  //! Set up the system defaults
  CLILogInit();
  SystemInitialize();
  s = SystemInfoGetDataFileName();
  mainDataFilename = StringCopy(s);
  
  //! Process command lines args (which may overide system defaults)
  mainProcessArgs(arg, argv);
  
  //! Initialize the various data stores
  NodeDataStoreInit();
  mainBays = CreateBaySet();
  //bay = CreateBay("Bay 1", BayTypeSupplemental, 1);
  //BaySetAddBay(mainBays, bay);
  if ( mainDeviceFilename ) {
    DeviceFileRead(mainDeviceFilename);
  }

  //! Set the the CAN Interface
  canInterface = CANInterfaceInit(MAIN_CAN_INTERFACE_NAME);
  
  //! Start up the worker threads
  CANIOInit();
  HTTPServerInit();
  ProcessInputMessageInit();
  NodeManagerInit();
  CLILogDisplayInitMessage();
  CLIThreadInit();

  ReadDataFile();
  pthread_create(&processInputThread, NULL, ProcessInputMessageThread, NULL);
  pthread_create(&httpThread, NULL, HTTPServerThread, NULL);
  if ( canInterface ) {
    pthread_create(&canReadThread, NULL, CANIOThread, canInterface);
  }
  pthread_create(&nodeManagerThread, NULL, NodeManagerThread, NULL);
  pthread_create(&cliThread, NULL, CLIThread, NULL);
  pthread_create(&simThread, NULL, SIMThread, NULL);

  //! Wait for the threads to finish
  pthread_join(processInputThread, NULL);
  pthread_join(httpThread, NULL);
  pthread_join(canReadThread, NULL);
  pthread_join(canWriteThread, NULL);
  pthread_join(nodeManagerThread, NULL);
  pthread_join(cliThread, NULL);
  pthread_join(simThread, NULL);
  return EXIT_SUCCESS;
}

/******************************************************************************!
 * Function : mainProcessArgs
 ******************************************************************************/
void
mainProcessArgs
(
 int                            argc,
 char**                         argv
)
{
  int                           i;

  if ( argc < 2 ) {
    return;
  }
  for (i = 1; i < argc; i++) {
    string command = argv[i];

    if ( StringEqual("-h", command) || StringEqual("-?", command) || StringEqual("--help", command) ) {
      DisplayUsage();
      exit(EXIT_SUCCESS);
    }

    if ( StringEqual("-D", command) || StringEqual("--devices", command) ) {
      i++;
      if ( i >= argc ) {
        fprintf(stderr, "%s requires a data filename\r\n", command);
        exit(EXIT_FAILURE);
      }
      if ( mainDeviceFilename ) {
        FreeMemory(mainDeviceFilename);
      }
      mainDeviceFilename = StringCopy(argv[i]);
      continue;
    }
    if ( StringEqual ("-V", command) || StringEqual("--version", command) ) {
      fprintf(stdout, "Version : %s : %s\n", mainVersion, mainBuildDate);
      exit(EXIT_SUCCESS);
    } 
    if ( StringEqual("-d", command) || StringEqual("--data", command) ) {
      i++;
      if ( i >= argc ) {
        fprintf(stderr, "%s requires a data filename\r\n", command);
        exit(EXIT_FAILURE);
      }
      if ( mainDataFilename ) {
        FreeMemory(mainDataFilename);
      }
      mainDataFilename = StringCopy(argv[i]);
      continue;
    }
    
    if ( StringEqual("-v", command) || StringEqual("--verbose", command) ) {
      CLIVerboseInterface = true;
      continue;
    }

	if ( StringEqualsOneOf(command, "-L", "--logfile", NULL) ) {
	  i++;
	  if ( i == argc ) {
		fprintf(stderr, "%s requires a filename\n", command);
		DisplayUsage();
		exit(EXIT_FAILURE);
	  }

	  CLILogFilenameSet(argv[i]);
	  continue;
	}
    if ( StringEqual("-l", command) || StringEqual("--log", command) ) {
      CLIWriteLog = true;
      continue;
    }

    if ( StringEqual("-w", command) || StringEqual("--webdir", command) ) {
      i++;
      if ( i >= argc ) {
        fprintf(stderr, "%s requires a web base directory name\r\n", command);
        exit(EXIT_FAILURE);
      }
      if ( mainWebBaseDirectory ) {
        FreeMemory(mainWebBaseDirectory);
      }
      mainWebBaseDirectory = StringCopy(argv[i]);
      continue;
    }
    fprintf(stderr, "%s is not a recognized command\r\n", command);
    exit(EXIT_FAILURE);
  }
}

/******************************************************************************!
 * Function : ReadDataFile
 ******************************************************************************/
void
ReadDataFile
()
{
  string                        fileName = "BaySim.txt";
  FILE*                         file;
  struct stat                   statbuf;
  uint32_t                      bytesRead;
  string                        buffer;  // Free Me
  StringList*                   strings; // Free Me
  string                        tag; // Free Me
  string                        value; // FreeMe
  int                           i;
 
  file = fopen(fileName, "rb");
  if ( NULL == file ) {
    fprintf(stderr, "Could not open %s\n", fileName);
    return;
  }
  stat(fileName, &statbuf);
  
  buffer = (string)GetMemory(statbuf.st_size+1);
  bytesRead = fread(buffer, 1, statbuf.st_size, file);
  if ( bytesRead != statbuf.st_size ) {
    fprintf(stderr, "Read %s error\n", fileName);
    FreeMemory(buffer);
    fclose(file);
    return;
  }
  fclose(file);
  strings = StringSplit(buffer, "\r\n", true);
  FreeMemory(buffer);
  for ( i = 0; i < strings->stringCount; i++) {
    string s = strings->strings[i];
    StringList*                 tagValue;
    tagValue = StringSplit(s, ":", true);
    if ( tagValue->stringCount != 2 ) {
      StringListDestroy(tagValue);
      FreeMemory(buffer);
      StringListDestroy(strings);
      return;
    }
    tag = StringTrim(tagValue->strings[0]);
    value = StringTrim(tagValue->strings[1]);
    if ( StringEqualNoCase("SerialNumber", tag) ) {
      mainSerialNumber = StringToU32(value);
    }
    FreeMemory(tag);
    FreeMemory(value);
    StringListDestroy(tagValue);
  }
  StringListDestroy(strings);
}

/******************************************************************************!
 * Function : GetNextSerialNumber
 ******************************************************************************/
uint32_t
GetNextSerialNumber
()
{
  uint32_t                              highestNumber = 0;
  uint16_t                              i;
  RectifierType*                        rect;
  DSMType*                              dsm;
  
  for (i = 0; i < mainBays->bayCount; i++) {
    Bay*                                bay = mainBays->bays[i];
    for ( dsm = BayGetFirstDSM(bay); dsm; dsm = BayGetNextDSM(bay, dsm) ) {
      if ( dsm->parentNode->serialNumber > highestNumber ) {
        highestNumber = dsm->parentNode->serialNumber;
      }
    }
    for ( rect = BayGetFirstRectifier(bay); rect; rect = BayGetNextRectifier(bay, rect) ) {
      if ( rect->parentNode->serialNumber > highestNumber ) {
        highestNumber = rect->parentNode->serialNumber;
      }
    }
  }
  if ( highestNumber >= mainSerialNumber ) {
    mainSerialNumber = highestNumber + 1;
  }
  
  return mainSerialNumber++;
}

/******************************************************************************!
 * Function : DisplayUsage
 ******************************************************************************/
void
DisplayUsage()
{
  printf("Usage : baysim -D --devices filename   Specifies the devices filename\n"
         "               -v --verbosei           Verbose operation\n"
         "               -w --webdir directory   Specifies the web directory\n"
         "               -V --version            Display the version\n"
		 "               -L filename             Specifies the log filename\n"
         "               -l --logi               Specifies the use of the log file\n"
         "               -h -? --help            Displays this information\n");
}

/*****************************************************************************!
 * Function : shutdown
 *****************************************************************************/
void
shutdown
(void)
{
  CLIDisplayMessage("Bay Simulator exiting\n");
}
