/*****************************************************************************
 * FILE NAME    : HTTPServerThread.c
 * DATE         : January 24 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2019 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <mongoose.h>
#include <json.h>
#include <StringUtils.h>
#include <MemoryManager.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ESNACANInterface.h"
#include "ESNACANNodeType.h"
#include "BayType.h"
#include "main.h"
#include "NodeDataStore.h"
#include "HTTPServerThread.h"
#include "CANInterface.h"
#include "Thread.h"
#include "Mutex.h"
#include "System.h"
#include "SystemTime.h"
#include "DeviceInterface.h"
#include "DeviceFile.h"
#include "RectifierType.h"
#include "CLIThread.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define HTTP_SERVER_SEND_QUEUE_SIZE             40
#define HTTP_SEND_TYPE_CAN_PACKET               1
#define HTTP_SEND_TYPE_NODE_ADDRESS             2
#define HTTP_SEND_ADD_BAY                       3
#define HTTP_SEND_ADD_RECTIFIER                 4
#define HTTP_SEND_CLEAR_DISPLAY                 5
#define HTTP_SEND_SET_POWER                     6
#define HTTP_SEND_TYPE_CAN_PACKET_STRING        7
#define HTTP_SEND_SERIAL_NUMBER                 8
#define HTTP_SEND_INIT                          9
#define HTTP_SEND_INFO_RECTIFIER                10
#define HTTP_SEND_STATUS                        11

/*****************************************************************************!
 * Local Type : HTTPServerSendType
 *****************************************************************************/
struct _HTTPServerSendType {
  int                                   type;
  union {
    struct {
      uint8_t                           direction;
      uint32_t                          id;
      uint64_t                          data;
      uint8_t                           dataLength;
    } CANProtocol;

    struct {
      char                              packet[256];
    } CANProtocolString;
   
    struct {
      RectifierType*                    rectifier;
    } RectifierInfo; 

    struct {
      Bay*                              bay;
    } BayAdd;
    
    struct {
      uint8_t                           unit;
      uint8_t                           bay;
    } RectifierAdd;
    
    struct {
      uint8_t                           unit;
      uint8_t                           bay;
      bool                              power;
    } RectifierSetPower;
    struct {
      uint8_t                           nodeType;
      uint8_t                           bayIndex;
      uint8_t                           unitNumber;
      uint8_t                           canAddress;
    } NodeAddress;
    uint32_t                            serialNumber;
  };
  
};
typedef struct _HTTPServerSendType HTTPServerSendType;
    
/*****************************************************************************!
 * Local Data
 *****************************************************************************/
#ifdef NEED_LOCAL_HTTP_SERVER
static const char *
s_http_port = "8000";
#endif

static const char *
s_websocket_port = "8001";

static struct mg_serve_http_opts
s_http_server_opts;

static Mutex 
SendQueueMutex;

static HTTPServerSendType
SendQueue[HTTP_SERVER_SEND_QUEUE_SIZE];

static uint8_t
SendQueueCurrent = 0;

static uint8_t
SendQueueHead = 0;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void
WebSocketEventHandler
(struct mg_connection *nc, int ev, void *p);

static void
HTTPEventHandler
(struct mg_connection * nc, int ev, void * p);
 
static void
HandleWebSocketRequest
(struct mg_connection* InConnection, const struct mg_str InMessage);

static void
HandleWebSocketStatusSend
(struct mg_connection* InConnection);

static void
HandleWebSocketSend
(struct mg_connection* InConnection);

void
HandleJSONAddCommand
(json_value* InValue);

void
HandleJSONRemoveCommand
(json_value* InValue);

void
HandleJSONRemoveShunt
(json_value* InValue);

void
HandleJSONAddRectifier
(json_value* InJSONDoc);

void
HandleJSONAddDSM
(json_value* InJSONDoc);

void
HandleJSONRemoveDSM
(json_value* InJSONDoc);

void
HandleJSONRemoveRectifier
(json_value* InJSONDoc);

void
HandleJSONAddShunt
(json_value* InJSONDoc);

void
HandleJSONUpdateShunt
(json_value* InJSONDoc);

void
HandleJSONOpenCommand
(json_value* InJSONDoc);

void
HandleJSONCloseCommand
(json_value* InJSONDoc);

void
HandleJSONAddBay
(json_value* InJSONDoc);

void
HandleJSONUpdateCommand
(json_value* InValue);

void
HandleJSONUpdateRectifier
(json_value* InJSONDoc);

void
HandleJSONUpdateDSM
(json_value* InJSONDoc);

void
HandleJSONUpdateBay
(json_value* InJSONDoc);

void
HandleJSONSetCommand
(json_value* InJSONDoc);

void
HandleJSONGetRectifierCommand
(json_value* InJSONDoc);

void
HandleJSONGetCommand
(json_value* InJSONDoc);

void
HandleJSONSaveCommand
(json_value* InJSONDoc);

void
HandleJSONReadCommand
(json_value* InJSONDoc);

void
HandleWebSocketCANPacketSend
(struct mg_connection* InConnection, HTTPServerSendType* InSendQueue);

void
HandleWebSocketCANPacketStringSend
(struct mg_connection* InConnection, HTTPServerSendType* InSendQueue);

void
HandleWebSocketCANAddressSend
(struct mg_connection* InConnection, HTTPServerSendType* InSendQueue);

void
HandleWebSocketBayAddSend
(struct mg_connection* InConnection, HTTPServerSendType* InSendQueue);

void
HandleWebSocketRectifierAddSend
(struct mg_connection* InConnection, HTTPServerSendType* InSendQueue);

void
HandleWebSocketRectifierInfoSend
(struct mg_connection* InConnection, HTTPServerSendType* InSendQueue);

void
HandleWebSocketSetPowerSend
(struct mg_connection* InConnection, HTTPServerSendType* InSendQueue);

void
HandleWebSocketClearDisplaySend
(struct mg_connection* InConnection);

void
HandleWebSocketInitSend
(struct mg_connection* InConnection);

void
SetRectifierPower
(json_value* InJSONDoc);

void
HandleAnnounceFromPage
(json_value* InJSONDoc);

void
HTTPSendStartingSerialNumber
();

void
HTTPSendClearDisplay();

void
HTTPSendInit();

void
SendQueueCurrentAdvance();

void
SendQueueHeadAdvance();

bool
SendQueueCurrentFull();

void
HTTPSendRectifierInfo
(RectifierType* InRectifier);

uint32_t
FindJSONEntryInt
(json_value* InJSONDoc, string InTag);

string
FindJSONEntryString
(json_value* InJSONDoc, string InTag);

void
HandleJSONUpdateDSMShunts
(json_value* InJSONDoc, DSMType* InDSM);

void
HandleJSONUpdateDSMShunt
(json_value* InJSONDoc, DSMType* InDSM, uint32_t InShuntIndex, uint32_t InCount);

void
HandleWebSocketSerialNumberSend
(struct mg_connection* InConnection, HTTPServerSendType*  InSend);

void
HTTPSendWebsocketFrame
(struct mg_connection* InConnection, string InBuffer, uint16_t InBufferLen);

void
HandleStatusRequest
();

/*****************************************************************************!
 * Function : HTTPServerInit
 *****************************************************************************/
void
HTTPServerInit
()
{
    SendQueueMutex = MutexCreate("HTTPServerMutex");
    // ThreadCreate("HTTP Server", HTTPServerThread, NULL);
}

/*****************************************************************************!
 * Function : HTTPServerThread
 *****************************************************************************/
void*
HTTPServerThread
(
 void*                                  InParameters
)
{

    struct mg_mgr                       mgr;
    struct mg_connection *              nc_websocket;
    
    mg_mgr_init(&mgr, NULL);
#ifdef NEED_LOCAL_HTTP_SERVER
    struct mg_connection *              nc;
    CLILogMessage("Starting web server on port %s\n", s_http_port);
    nc = mg_bind(&mgr, s_http_port, HTTPEventHandler);
    if (nc == NULL) {
        printf("Failed to create http listener\n");
        return (void*)1;
    }
    
    //! Set up HTTP server parameters
    mg_set_protocol_http_websocket(nc);
#endif

    nc_websocket = mg_bind(&mgr, s_websocket_port, WebSocketEventHandler);
    if (nc_websocket == NULL) {
        printf("Failed to create websocket listener\n");
            return NULL; 
    }
    
    CLILogMessage("Starting websocket server on port %s\r\n", s_websocket_port);
    
    //! Set up WEBSocket server parameters
    mg_set_protocol_http_websocket(nc_websocket);
    s_http_server_opts.document_root = mainWebBaseDirectory;
    s_http_server_opts.enable_directory_listing = "yes";
    
    while (1) {
        mg_mgr_poll(&mgr, 20);        
    }
    return NULL;
}

#ifdef NEED_LOCAL_HTTP_SERVER
/*****************************************************************************!
 * Function : HTTPEventHandler
 *****************************************************************************/
static void
HTTPEventHandler
(
 struct mg_connection *                 nc,
 int                                    ev,
 void *                                 p
)
{
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}
#endif // NEED_LOCAL_HTTP_SERVER

/*****************************************************************************!
 * Function : WebSocketEventHandler
 *****************************************************************************/
static void
WebSocketEventHandler
(
 struct mg_connection *                 nc,
 int                                    ev,
 void *                                 ev_data
)
{
    switch (ev) {
        case MG_EV_WEBSOCKET_FRAME: {
            struct websocket_message *wm = (struct websocket_message *) ev_data;
            /* New websocket message. Tell everybody. */
            struct mg_str d = {(char *) wm->data, wm->size};
            HandleWebSocketRequest(nc, d);
            break;
        }
        case MG_EV_POLL : {
            HandleWebSocketSend(nc);
            break;
        }
    }
}

/*******************************************************************************!
 * Function : HandleWebSocketRequest
 * Purpose  : Handle a request from the webpage
 *******************************************************************************/
static void
HandleWebSocketRequest
(
 struct mg_connection*                  InConnection,
 const struct mg_str                    InMessage
)
{
  int                                   i;
  json_value*                           jsonDoc;

  string s = StringNCopy((char*)InMessage.p, InMessage.len);
  FreeMemory(s);
  jsonDoc = json_parse((const json_char*)InMessage.p, (size_t)InMessage.len);
  if ( jsonDoc == NULL ) {
    return;
  }

  if ( jsonDoc->type != json_object ) {
    return ;
  }
  for ( i = 0 ; i < jsonDoc->u.object.length; i++) {
      json_object_entry*                j;
      json_value*                       value;
      
      j = &(jsonDoc->u.object.values[i]);
      if ( StringEqual(j->name, "command") ) {
          value = j->value;

          if ( StringEqualNoCase(value->u.string.ptr, "status") ) {
            HandleStatusRequest(jsonDoc);
            break;
          }
          
          if ( StringEqualNoCase(value->u.string.ptr, "announce") ) {
            HandleAnnounceFromPage(jsonDoc);
            break;
          }
          
          if ( StringEqualNoCase(value->u.string.ptr, "save") ) {
            HandleJSONSaveCommand(jsonDoc);
            break;
          }
          
          if ( StringEqualNoCase(value->u.string.ptr, "read") ) {
            HandleJSONReadCommand(jsonDoc);
            break;
          }
          
          if ( StringEqualNoCase(value->u.string.ptr, "add") ) {
            HandleJSONAddCommand(jsonDoc);
            break;
          }
          if ( StringEqualNoCase(value->u.string.ptr, "remove") ) {
            HandleJSONRemoveCommand(jsonDoc);
            break;
          }
          if ( StringEqualNoCase(value->u.string.ptr, "update") ) {
            HandleJSONUpdateCommand(jsonDoc);
            break;
          }
          if ( StringEqualNoCase(value->u.string.ptr, "open") ) {
            HandleJSONOpenCommand(jsonDoc);
            break;
          }
          if ( StringEqualNoCase(value->u.string.ptr, "closed") ) {
            HandleJSONCloseCommand(jsonDoc);
            break;
          }
          if ( StringEqualNoCase(value->u.string.ptr, "get" ) ) {
            HandleJSONGetCommand(jsonDoc);
            break;
          }
          if ( StringEqualNoCase(value->u.string.ptr, "set" ) ) {
            HandleJSONSetCommand(jsonDoc);
            break;
          }
      }
  }
}

/*****************************************************************************!
 * Function : HandleJSONSaveCommand
 *****************************************************************************/
void
HandleJSONSaveCommand
(json_value* InJSONDoc)
{
  if ( mainDeviceFilename ) {
    BaySaveFile(mainBays->bays[0], mainDeviceFilename);
  }
}

/*****************************************************************************!
 * Function : HandleJSONReadCommand
 *****************************************************************************/
void
HandleJSONReadCommand
(json_value* InJSONDoc)
{
  if ( mainDeviceFilename ) {
    DeviceFileRead(mainDeviceFilename);
  }
}

/*****************************************************************************!
 * Function : HandleJSONCloseCommand
 *****************************************************************************/
void
HandleJSONCloseCommand
(json_value* InJSONDoc)
{
  string                                unitNumber, shuntIndex;

  shuntIndex = FindJSONEntryString(InJSONDoc, "shuntIndex");
  unitNumber = FindJSONEntryString(InJSONDoc, "unitNumber");

  DeviceDistSetClose(unitNumber, shuntIndex);
}

/*****************************************************************************!
 * Function : HandleJSONOpenCommand
 *****************************************************************************/
void
HandleJSONOpenCommand
(json_value* InJSONDoc)
{
  string                                unitNumber, shuntIndex;
  shuntIndex = FindJSONEntryString(InJSONDoc, "shuntIndex");
  unitNumber = FindJSONEntryString(InJSONDoc, "unitNumber");

  DeviceDistSetOpen(unitNumber, shuntIndex);
}

/******************************************************************************!
 * Function : HandleAnnounceFromPage
 ******************************************************************************/
void
HandleAnnounceFromPage
(
 json_value*                    InJSONDoc
)
{
  int                                   i;
  HTTPSendInit();
  for ( i = 0; i < mainBays->bayCount; i++) {
    HTTPSendBayAdd(mainBays->bays[i]); 
  }
}

/*****************************************************************************!
 * Function : HandleJSONGetCommand
 *****************************************************************************/
void
HandleJSONGetCommand
(json_value* InJSONDoc)
{
  string                                type;

  type = FindJSONEntryString(InJSONDoc, "type");
  if ( StringEqualNoCase(type, "rectifier") ){
    HandleJSONGetRectifierCommand(InJSONDoc);
  }
}

/*****************************************************************************!
 * Function : HandleJSONSetRectifierCommand
 *****************************************************************************/
void
HandleJSONGetRectifierCommand
(json_value* InJSONDoc)
{
  uint8_t                               bayIndex, unitNumber;
  Bay*                                  bay;
  RectifierType*                        rectifier;

  bayIndex = FindJSONEntryInt(InJSONDoc, "bayIndex");
  unitNumber = FindJSONEntryInt(InJSONDoc, "unitNumber");

  bay = BaySetFindBayByIndex(mainBays, bayIndex);
  if ( bay == NULL ) {
    return;
  }

  rectifier = BayFindRectifier(bay, unitNumber);

  if ( NULL == rectifier ) {
    return;
  }
  HTTPSendRectifierInfo(rectifier);
}


/*****************************************************************************!
 * Function : HandleJSONSetCommand
 *****************************************************************************/
void
HandleJSONSetCommand
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
  if ( NULL == InJSONDoc ) {
    return;
  }

  for ( i = 0; i < InJSONDoc->u.object.length; i++) {
    json_object_entry*                  j;

    j = &(InJSONDoc->u.object.values[i]);
    if ( StringEqual(j->name, "canprotocolmonitor") ) {
      // SetMonitorCANProtocol(StringEqualNoCase(j->value->u.string.ptr, "true"));
    } else if ( StringEqual(j->name, "rectifierPower") ) {
      SetRectifierPower(InJSONDoc);
    }
  }
}

/*****************************************************************************!
 * Function : SetRectifierPower
 *****************************************************************************/
void
SetRectifierPower
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
  int8_t                                unitNumber, bayIndex;
  bool                                  power;
  RectifierType*                        rectifier;
  
  unitNumber = -1;
  bayIndex = -1;
  power = false;;
  for ( i = 0; i < InJSONDoc->u.object.length; i++) {
    json_object_entry*                  j;

    j = &(InJSONDoc->u.object.values[i]);
    if ( StringEqual(j->name, "unitNumber") ) {
      unitNumber = (int8_t)j->value->u.integer;
    } else  if ( StringEqual(j->name, "bayIndex") ) {
      bayIndex = (int8_t)j->value->u.integer;
    } else  if ( StringEqual(j->name, "rectifierPower") ) {
      power = StringEqualNoCase(j->value->u.string.ptr, "on");
    }
  }
  if ( unitNumber == -1 || bayIndex == -1 ) {
    return;
  }

  rectifier = BayFindRectifier(BaySetFindBayByIndex(mainBays, bayIndex), unitNumber);
  if ( rectifier == NULL ) {
    return;
  }

  if ( power ) {
    RectifierTurnOn(rectifier);
  } else {
    RectifierTurnOff(rectifier);
  }
}

/*****************************************************************************!
 * Function : HandleJSONUpdateCommand
 *****************************************************************************/
void
HandleJSONUpdateCommand
(
 json_value*                            InJSONDoc
)
{
  string                                typeString;

  typeString = FindJSONEntryString(InJSONDoc, "type");
  if ( NULL == typeString ) {
    return;
  }
  
  if ( StringEqualNoCase(typeString, "Rectifier") ) {
    HandleJSONUpdateRectifier(InJSONDoc);
  } else if ( StringEqualNoCase(typeString, "Bay") ) {
    HandleJSONUpdateBay(InJSONDoc);
  } else if ( StringEqualNoCase(typeString, "DSM" ) ) {
    HandleJSONUpdateDSM(InJSONDoc);
  } else if ( StringEqualNoCase(typeString, "shunt" ) ) {
    HandleJSONUpdateShunt(InJSONDoc);
  }
}

/*****************************************************************************!
 * Function : HandleJSONUpdateRectifier
 *****************************************************************************/
void
HandleJSONUpdateRectifier
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
    uint8_t                             unitNumber, bayIndex;
    RectifierType*                      rectifier;
    float                               outputCurrent;
    uint16_t                            ratedCurrent, ratedOutputVoltage, ratedInputVoltage;
    string                              customerText;
    Bay*                                bay;
    NodeType*                           node;
    uint32_t				serialNumber;
    uint8_t				partNumber[NODE_PART_NUMBER_SIZE+1];
    int					partNumberLength = 0;

    unitNumber = -1;

    customerText = NULL;
    for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
        json_object_entry*                j;
        json_value*                       value;
        j = &(InJSONDoc->u.object.values[i]);
        if (StringEqual(j->name, "unitNumber") ) {
            value = j->value;
            unitNumber = value->u.integer;
        } else if ( StringEqual(j->name, "bayIndex") ) {
          value = j->value;
          bayIndex = value->u.integer;
        } else if ( StringEqual(j->name, "outputCurrent") ) {
            outputCurrent = (j->value->u.integer);
        } else if ( StringEqual(j->name, "ratedCurrent") ) {
          ratedCurrent = (uint16_t)j->value->u.integer;
        } else if ( StringEqual(j->name, "ratedInputVoltage") ) {
            ratedInputVoltage = (uint16_t)atoi(j->value->u.string.ptr);
        } else if ( StringEqual(j->name, "ratedOutputVoltage") ) {
            ratedOutputVoltage = (uint16_t)atoi(j->value->u.string.ptr);
        } else if ( StringEqual(j->name, "customerText") ) {
            customerText = j->value->u.string.ptr;
	} else if ( StringEqual(j->name, "serialNumber") ) {
	    serialNumber = atoi(j->value->u.string.ptr);
        } else if ( StringEqual(j->name, "productNumber") ) {
	    if ( j->value->u.string.length > NODE_PART_NUMBER_SIZE ) {
		partNumberLength = NODE_PART_NUMBER_SIZE;
	    } else {
		partNumberLength = j->value->u.string.length;
	    }
	    memcpy(partNumber, j->value->u.string.ptr, partNumberLength);
	    partNumber[partNumberLength] = 0x00;
	}
    }
    bay = BaySetFindBayByIndex(mainBays, bayIndex);
    if ( NULL == bay ) {
      return;
    }
    rectifier = BayFindRectifier(bay, unitNumber);
    if ( NULL == rectifier ) {
        return;
    }
    
    
    node = rectifier->parentNode;
    rectifier->outputCurrent = outputCurrent;
    rectifier->ratedOutputCurrent = ratedCurrent;
    rectifier->ratedOutputCurrentType = RatedTypeDC;
    rectifier->ratedOutputVoltage = ratedOutputVoltage;
    rectifier->ratedOutputVoltageType = RatedTypeDC;
    rectifier->ratedInputVoltage = ratedInputVoltage;
    rectifier->ratedInputVoltageType = RatedTypeAC;
    rectifier->parentNode->serialNumber = serialNumber;
    if ( partNumberLength > 0 ) {
	memset(rectifier->parentNode->partNumber, 0x00, NODE_PART_NUMBER_SIZE+1);
	memcpy(rectifier->parentNode->partNumber, partNumber, partNumberLength);
    }
    if ( customerText ) {
        int n = strlen(customerText);
        if ( n > NODE_CUSTOMER_TEXT_SIZE_TOTAL ) {
            n = NODE_CUSTOMER_TEXT_SIZE_TOTAL;
        }
        memcpy(node->customerText, customerText, n);
        node->customerText[n] = 0x00;
    }
    rectifier->alarms.allAlarms = 0;
    for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
        json_object_entry*                j;
        j = &(InJSONDoc->u.object.values[i]);
        if ( StringEqual(j->name, "ACH") ) {
            rectifier->alarms.ach = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "ACL") ) {
            rectifier->alarms.acl = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "CBO") ) {
            rectifier->alarms.cbo = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "CNV") ) {
            rectifier->alarms.cnv = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "DHV") ) {
            rectifier->alarms.dhv = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "DCU") ) {
            rectifier->alarms.dcu = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "DTS") ) {
            rectifier->alarms.dts = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "EMS") ) {
            rectifier->alarms.ems = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "FMA") ) {
            rectifier->alarms.fma = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "INH") ) {
            rectifier->alarms.inh = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "NPD") ) {
            rectifier->alarms.npd = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "PHL") ) {
            rectifier->alarms.phl = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "PFF") ) {
            rectifier->alarms.pff = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "PHV") ) {
            rectifier->alarms.phv = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "PTS") ) {
            rectifier->alarms.pts = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "RHV") ) {
            rectifier->alarms.rhv = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "SWO") ) {
            rectifier->alarms.swo = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "FMI") ) {
            rectifier->alarms.fmi = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "HAT") ) {
            rectifier->alarms.hat = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "TCL") ) {
            rectifier->alarms.tcl = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "SNS") ) {
            rectifier->alarms.sns = j->value->u.boolean == 1;
        } else if ( StringEqual(j->name, "DNP") ) {
            rectifier->alarms.dnp = j->value->u.boolean == 1;
        }
    }
    if ( rectifier->alarms.allAlarms ) {
      rectifier->alarms.aaa = 1;
    }
}

/*****************************************************************************!
 * Function : HandleJSONUpdateBay
 *****************************************************************************/
void
HandleJSONUpdateBay
(
 json_value*                            InJSONDoc
)
{


}

/*****************************************************************************!
 * Function : HandleJSONUpdateDSM
 *****************************************************************************/
void
HandleJSONUpdateDSM
(
 json_value*                            InJSONDoc
)
{
  uint8_t                               unitNumber, bayIndex;
  NodeType*                             node;
  DSMType*                              dsm;
  uint32_t                              n;
  int                                   i;

  unitNumber  = FindJSONEntryInt(InJSONDoc, "unitNumber");
  bayIndex    = FindJSONEntryInt(InJSONDoc, "bayIndex");

  node = NodeDataStoreFindExistingInBay(NODE_TYPE_DSM, bayIndex, unitNumber);
  if ( NULL == node ) {
    return;
  }

  dsm = &(node->dsm);

  if ( NULL == dsm ) {
    return;
  }
  for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
    json_object_entry*                j;
    json_value*                       value;

    j = &(InJSONDoc->u.object.values[i]);
    value = j->value;
    if ( StringEqual(j->name, "maxPointsPerBus") ) {
      dsm->description.bit.maxPointsOnBus = value->u.integer;
    } else if ( StringEqual(j->name, "productNumber") ) {
      n = value->u.string.length;
      if ( n > NODE_PART_NUMBER_SIZE ) {
        n = NODE_PART_NUMBER_SIZE;
      }
      strncpy((char*)node->partNumber, value->u.string.ptr, n);
      node->partNumber[n] = 0x00;
    } else if ( StringEqual(j->name, "serialNumber") ) {
      node->serialNumber = value->u.integer;
    }
  }
}

/*****************************************************************************!
 * Function : HandleJSONUpdateShunt
 *****************************************************************************/
void
HandleJSONUpdateShunt
(json_value* InJSONDoc)
{
  string                                unitNumber;
  string                                shuntIndex, current;

  unitNumber = FindJSONEntryString(InJSONDoc, "unitNumber");
  shuntIndex = FindJSONEntryString(InJSONDoc, "shuntIndex");
  current = FindJSONEntryString(InJSONDoc, "current");
  DeviceDistSetCurrent(unitNumber, shuntIndex, current);
}

/*****************************************************************************!
 * Function : HandleJSONAddCommand
 *****************************************************************************/
void
HandleJSONAddCommand
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
    for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
        json_object_entry*                j;
        json_value*                       value;
        
        j = &(InJSONDoc->u.object.values[i]);
        if ( StringEqual(j->name, "type") ) {
            value = j->value;
            if ( StringEqualNoCase(value->u.string.ptr, "Rectifier") ) {
                HandleJSONAddRectifier(InJSONDoc);
                break;
            }
            if ( StringEqualNoCase(value->u.string.ptr, "Bay") ) {
                HandleJSONAddBay(InJSONDoc);
                break;
            }
            if ( StringEqualNoCase(value->u.string.ptr, "DSM" ) ) {
                HandleJSONAddDSM(InJSONDoc);
                break;
            }
            if ( StringEqualNoCase(value->u.string.ptr, "Shunt" ) ) {
              HandleJSONAddShunt(InJSONDoc);
              break;
            }
        }
    }
}

/*****************************************************************************!
 * Function : HandleJSONRemoveCommand
 *****************************************************************************/
void
HandleJSONRemoveCommand
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
    for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
        json_object_entry*                j;
        json_value*                       value;
        
        j = &(InJSONDoc->u.object.values[i]);
        if ( StringEqual(j->name, "type") ) {
            value = j->value;
            if ( StringEqualNoCase(value->u.string.ptr, "Shunt" ) ) {
              HandleJSONRemoveShunt(InJSONDoc);
              break;
            }
            if ( StringEqualNoCase(value->u.string.ptr, "DSM" ) ) {
              HandleJSONRemoveDSM(InJSONDoc);
              break;
            }
            if ( StringEqualNoCase(value->u.string.ptr, "rectifier" ) ) {
              HandleJSONRemoveRectifier(InJSONDoc);
              break;
            }
        }
    }
}

/*****************************************************************************!
 * Function : HandleJSONAddShunt
 *****************************************************************************/
void
HandleJSONAddShunt
(
 json_value*                            InJSONDoc
)
{
  string shuntIndex, shuntSize, shuntType;
  string unitNumber, maxCurrent;

  maxCurrent = FindJSONEntryString(InJSONDoc, "shuntMaxCurrent");
  shuntType =  FindJSONEntryString(InJSONDoc, "shuntType");
  unitNumber =  FindJSONEntryString(InJSONDoc, "unitNumber");
  shuntIndex = FindJSONEntryString(InJSONDoc, "shuntIndex");
  shuntSize = FindJSONEntryString(InJSONDoc, "shuntSize");

  DeviceDistAdd(unitNumber, shuntIndex, shuntType, shuntSize, maxCurrent);
}

/*****************************************************************************!
 * Function : HandleJSONRemoveShunt
 *****************************************************************************/
void
HandleJSONRemoveShunt
(
 json_value*                            InJSONDoc
)
{
  string shuntIndex, unitNumber;

  unitNumber =  FindJSONEntryString(InJSONDoc, "unitNumber");
  shuntIndex = FindJSONEntryString(InJSONDoc, "shuntIndex");

  DeviceDistRemove(unitNumber, shuntIndex);
}

/*****************************************************************************!
 * Function : HandleJSONRemoveDSM
 *****************************************************************************/
void
HandleJSONRemoveDSM
(json_value* InJSONDoc)
{
  DeviceDSMRemove(FindJSONEntryString(InJSONDoc, "unitNumber"));
}

/*****************************************************************************!
 * Function : HandleJSONRemoveRectifier
 *****************************************************************************/
void
HandleJSONRemoveRectifier
(json_value* InJSONDoc)
{
  DeviceRectifierRemove(FindJSONEntryString(InJSONDoc, "unitNumber"));
}

/*****************************************************************************!
 * Function : HandleJSONAddRectifier
 *****************************************************************************/
void
HandleJSONAddRectifier
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
  uint8_t                               unitNumber;
  uint32_t                              serialNumber;
  string                                productNumber;
  string                                customerText;
  NodeType*                             node;
  uint16_t                              n;
  uint16_t                              ratedOutputCurrent;
  
  unitNumber = FindJSONEntryInt(InJSONDoc, "unitNumber");
  if ( BayFindRectifier(BaySetFindBayByIndex(mainBays, 1), unitNumber) ) {
    return;
  }
  for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
    json_object_entry*                  j;
    json_value*                         value;

    j = &(InJSONDoc->u.object.values[i]);
    value = j->value;
    if (StringEqual(j->name, "serialNumber") ) {
      serialNumber = value->u.integer;
    } else if ( StringEqual(j->name, "productNumber") ) {
      productNumber = value->u.string.ptr;
    } else if ( StringEqual(j->name, "customerText") ) {
      customerText = value->u.string.ptr;
    } else if ( StringEqual(j->name, "ratedOutputCurrent") ) {
      ratedOutputCurrent = (uint16_t)value->u.integer;
    }
  }
  ESNANodeRectifierAdd(1, unitNumber);
  node = NodeDataStoreFindExisting(NODE_TYPE_RECT, unitNumber);
  if ( node == NULL ) {
    return;
  }
  node->serialNumber = serialNumber;
  n = strlen(productNumber);
  memcpy(node->partNumber, productNumber, n);
  node->partNumber[n] = 0x00;
  
  n = strlen(customerText);
  if ( n > NODE_CUSTOMER_TEXT_SIZE_TOTAL ) {
    n = NODE_CUSTOMER_TEXT_SIZE_TOTAL;
  }
  node->rectifier.ratedOutputCurrent = ratedOutputCurrent;
  memcpy(node->customerText, customerText, n);
  node->customerText[n] = 0x00;
  ESNACANSendRectifierNodeAddressRequest(unitNumber,
                                         node->rectifier.maxCurrentCapacity, node->rectifier.ratedOutputCurrent, serialNumber);
}

/*****************************************************************************!
 * Function : HandleJSONAddDSM
 *****************************************************************************/
void
HandleJSONAddDSM
(
 json_value*                            InJSONDoc
)
{
  string                                maxPoints, serialNumber, productNumber;
  string                                unitNumber = FindJSONEntryString(InJSONDoc, "unitNumber");

  if ( BayFindDSM(BaySetFindBayByIndex(mainBays, 1), atoi(unitNumber)) ) {
    return;
  }
  maxPoints = FindJSONEntryString(InJSONDoc, "maxPointsPerBus");
  productNumber = FindJSONEntryString(InJSONDoc, "productNumber");
  serialNumber = FindJSONEntryString(InJSONDoc, "serialNumber");

  DeviceDSMAdd(unitNumber, serialNumber, productNumber, maxPoints);
}

/*****************************************************************************!
 * Function : HandleJSONAddBay
 *****************************************************************************/
void
HandleJSONAddBay
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
    Bay*                                bay;
    BayType                             type;
    string                              bayName;
    string                              bayType;
    uint16_t                            index;

    for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
        json_object_entry*                j;
        json_value*                       value;
        
        j = &(InJSONDoc->u.object.values[i]);
        if ( StringEqual(j->name, "name") ) {
            value = j->value;
            bayName = value->u.string.ptr;
        } else if (StringEqual(j->name, "index") ) {
            value = j->value;
            index = value->u.integer;
        } else if (StringEqual(j->name, "bayType") ) {
            value = j->value;
            bayType = value->u.string.ptr;
        }
    }

    if ( bayName == NULL ) {
        return;
    }

    if ( StringEqual(bayType, "Main") ) {
        type = BayTypeMain;
    } else if ( StringEqual(bayType, "Supplemental") ) {
        type = BayTypeSupplemental;
    } else if ( StringEqual(bayType, "Distribution") ) {
        type = BayTypeDistribution;
    } else {
        return;
    }

    bay = CreateBay(bayName, type, index);
    BaySetAddBay(mainBays, bay);
}

/*****************************************************************************!
 * Function : HTTPSendClearDisplay
 *****************************************************************************/
void
HTTPSendClearDisplay()
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_CLEAR_DISPLAY;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HTTPSendInit
 *****************************************************************************/
void
HTTPSendInit()
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_INIT;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HandleStatusRequest
 *****************************************************************************/
void
HandleStatusRequest
()
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_STATUS;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HTTPSendStartingSerialNumber
 *****************************************************************************/
void
HTTPSendStartingSerialNumber()
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_SERIAL_NUMBER;
  SendQueue[SendQueueCurrent].serialNumber = 0x123;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HTTPSendRectifierPower
 *****************************************************************************/
void
HTTPSendRectifierPower
(
 uint8_t                                InBayIndex,
 uint8_t                                InUnitNumber,
 bool                                   InPower
)
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_SET_POWER;
  SendQueue[SendQueueCurrent].RectifierSetPower.bay = InBayIndex;
  SendQueue[SendQueueCurrent].RectifierSetPower.unit = InUnitNumber;
  SendQueue[SendQueueCurrent].RectifierSetPower.power = InPower;
  
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HTTPSendNodeCANAddress
 *****************************************************************************/
void
HTTPSendNodeCANAddress
(
 uint8_t                                InNodeType,
 uint8_t                                InBayIndex,
 uint8_t                                InUnitNumber,
 uint8_t                                InCANAddress
)
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_TYPE_NODE_ADDRESS;
  SendQueue[SendQueueCurrent].NodeAddress.nodeType = InNodeType;
  SendQueue[SendQueueCurrent].NodeAddress.bayIndex = InBayIndex;
  SendQueue[SendQueueCurrent].NodeAddress.unitNumber = InUnitNumber;
  SendQueue[SendQueueCurrent].NodeAddress.canAddress = InCANAddress;
  
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/******************************************************************************!
 * Function : HTTPSendCANPacketString
 ******************************************************************************/
void
HTTPSendCANPacketString
(
 string                         InString
)
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }

  strcpy(SendQueue[SendQueueCurrent].CANProtocolString.packet, InString);
  SendQueue[SendQueueCurrent].type = HTTP_SEND_TYPE_CAN_PACKET_STRING;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HTTPSendCANPacket
 *****************************************************************************/
void
HTTPSendCANPacket
(
 uint8_t                                InDirection,
 uint32_t                               InID,
 uint64_t                               InData,
 uint8_t                                InDataLength
)
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }

  SendQueue[SendQueueCurrent].CANProtocol.id = InID;
  SendQueue[SendQueueCurrent].CANProtocol.data = InData;
  SendQueue[SendQueueCurrent].CANProtocol.direction = InDirection;
  SendQueue[SendQueueCurrent].CANProtocol.dataLength = InDataLength;
  SendQueue[SendQueueCurrent].type = HTTP_SEND_TYPE_CAN_PACKET;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HandleWebSocketSend
 *****************************************************************************/
void
HandleWebSocketSend
(
 struct mg_connection*                  InConnection
)
{
  MutexGet(SendQueueMutex);
  if ( SendQueueCurrent == SendQueueHead ) {
    MutexRelease(SendQueueMutex);
    return;
  }
  if ( SendQueue[SendQueueCurrent].type == HTTP_SEND_TYPE_CAN_PACKET ) {
    HandleWebSocketCANPacketSend(InConnection, &SendQueue[SendQueueHead]);
  } else if ( SendQueue[SendQueueCurrent].type == HTTP_SEND_TYPE_CAN_PACKET_STRING ) {
    HandleWebSocketCANPacketStringSend(InConnection, &SendQueue[SendQueueHead]);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_TYPE_NODE_ADDRESS ) {
    HandleWebSocketCANAddressSend(InConnection, &(SendQueue[SendQueueHead]));
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_INFO_RECTIFIER ) {
    HandleWebSocketRectifierInfoSend(InConnection, &SendQueue[SendQueueHead]);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_ADD_BAY) {
    HandleWebSocketBayAddSend(InConnection, &SendQueue[SendQueueHead]);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_ADD_RECTIFIER) {
    HandleWebSocketRectifierAddSend(InConnection, &SendQueue[SendQueueHead]);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_CLEAR_DISPLAY) {
    HandleWebSocketInitSend(InConnection);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_INIT) {
    HandleWebSocketInitSend(InConnection);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_STATUS) {
    HandleWebSocketStatusSend(InConnection);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_SET_POWER ) {
    HandleWebSocketSetPowerSend(InConnection, &SendQueue[SendQueueHead]);
  } else if ( SendQueue[SendQueueHead].type == HTTP_SEND_SERIAL_NUMBER ) {
    HandleWebSocketSerialNumberSend(InConnection, &SendQueue[SendQueueHead]);
  }
  SendQueueHeadAdvance();
  MutexRelease(SendQueueMutex);
}

/*****************************************************************************!
 * Function : HandleWebSocketRectifierAddSend
 *****************************************************************************/
void
HandleWebSocketRectifierAddSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[256];
  SystemTime                            t;

  SystemGetTime(&t);
  sprintf(outBuffer, 
          "{ "
          "\"command\" : \"add\", "
          "\"value\" : \"rectifier\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"unit\" : %d, "
          "\"bay\"  : %d"
          "}",
          t.hour, t.minute, t.second, t.millisecond,
          InSend->RectifierAdd.unit,
          InSend->RectifierAdd.bay);
  mg_send_websocket_frame(InConnection, WEBSOCKET_OP_TEXT,
                          outBuffer, strlen(outBuffer));
}

/*****************************************************************************!
 * Function : HandleWebSocketClearDisplaySend
 *****************************************************************************/
void
HandleWebSocketClearDisplaySend
(
 struct mg_connection*                  InConnection
)
{
  char                                  outBuffer[256];
  SystemTime                            t;

  SystemGetTime(&t);
  sprintf(outBuffer, 
          "{ "
          "\"command\" : \"cleardisplay\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\""
          "}",
          t.hour, t.minute, t.second, t.millisecond);
  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/*****************************************************************************!
 * Function : HandleWebSocketInitSend
 *****************************************************************************/
void
HandleWebSocketInitSend
(
 struct mg_connection*                  InConnection
)
{
  int                                   i, k;
  char                                  outBuffer[8096];
  string                                bp;
  SystemTime                            t;
  RectifierType*                        rectifier;
  DSMType*                              dsm;
  SystemGetTime(&t);
  sprintf(outBuffer, 
          "{ "
          "\"command\" : \"Init\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"serialNumber\" : %lu, "
          "\"DSMDistOnlyProductNumber\" : \"%s\",\n"
          "\"DSMProductNumber\" : \"%s\",\n",
          t.hour, t.minute, t.second, t.millisecond, (unsigned long int)mainSerialNumber, mainDSMDistOnlyProductNumber, mainDSMProductNumber);
  bp = outBuffer + strlen(outBuffer);

  sprintf(bp, "\"bays\" : [\n");
  bp = outBuffer + strlen(outBuffer);
  for ( i = 0; i < mainBays->bayCount; i++) {
    sprintf(bp, "  {\n");
    bp = outBuffer + strlen(outBuffer);
    
    sprintf(bp, "\"name\" : \"%s\","
            "\"index\" : %d,\n"
            "\"type\" : \"%s\",\n"
            "\"rectifierCount\" : %d,"
            "\"dsmCount\" : %d,",
            mainBays->bays[i]->name,
            mainBays->bays[i]->index,
            BayTypeToString(mainBays->bays[i]->type),
            mainBays->bays[i]->maxRectifierCount,
            mainBays->bays[i]->maxDSMCount);
    bp = outBuffer + strlen(outBuffer);

    sprintf(bp, " \"rectifiers\" : [\n");
    bp = outBuffer + strlen(outBuffer);
    if ( mainBays->bays[i]->maxRectifierCount > 0 ) {
      
      // Output the Rectifiers
      for ( rectifier = BayGetFirstRectifier(mainBays->bays[i]);
            rectifier; ) {
        
        sprintf(bp,
                "{ "
                "\"ProductNumber\" : \"%s\", "
                "\"SerialNumber\" : %d, "
                "\"RatedOutputVolt\" : %d, "
                "\"RatedOutputCurrent\" : %d, "
                "\"RatedInputVolt\" : %d, "
                "\"UnitNumber\" : %d, "
                "\"ESNACANAddress\" : %d, "
                "\"OutputCurrent\" : %f, "
                "\"Power\" : \"%s\", "
                "\"StartUpState\" : %d", 
                rectifier->parentNode->partNumber,
                rectifier->parentNode->serialNumber,
                rectifier->ratedOutputVoltage,
                rectifier->ratedOutputCurrent,
                rectifier->ratedInputVoltage,
                rectifier->parentNode->location.unitNumber,
                rectifier->parentNode->ESNACANAddress,
                rectifier->outputCurrent,
                rectifier->power ? "On" : "Off",
                rectifier->parentNode->startUpState);
        bp = outBuffer + strlen(outBuffer);
        rectifier = BayGetNextRectifier(mainBays->bays[i], rectifier);      
        if ( rectifier ) {
          sprintf(bp, "},\n");
        } else {
          sprintf(bp, "}\n");
        }
        bp = outBuffer + strlen(outBuffer);
      }
    }
    sprintf(bp, "],\n");
    bp = outBuffer + strlen(outBuffer);
    sprintf(bp, " \"dsms\" : [ ");
    bp = outBuffer + strlen(outBuffer);
    for ( dsm = BayGetFirstDSM(mainBays->bays[i]); dsm; ) {
      sprintf(bp, "{"
                  "\"ProductNumber\" : \"%s\","
                  "\"SerialNumber\" : %d, "
                  "\"UnitNumber\" : %d, "
                  "\"ESNACANAddress\" : %d, "
                  "\"Shunts\" : [\n",
                  dsm->parentNode->partNumber,
                  dsm->parentNode->serialNumber,
                  dsm->parentNode->location.unitNumber,
                  dsm->parentNode->ESNACANAddress);
      bp = outBuffer + strlen(outBuffer);
      for ( k = 0; k < DSM_MAX_SHUNTS; k++) {
        Shunt*                          shunt;

        shunt = &(dsm->shunts[k]);
        sprintf(bp, "{ "
                    "\"index\" : %d, "
                    "\"occupied\" : \"%s\"",
                    k+1,
                    shunt->occupied ? "Yes" : "No");
        bp = outBuffer + strlen(outBuffer);
        if ( shunt->occupied ) {
          string shuntType = StringCopy(ShuntTypeToString(shunt->type));
          StringCapitalize(shuntType);
          sprintf(bp,
                  ", \"width\" : %d, "
                  "\"maxCurrent\" : %d, "
                  "\"power\" : \"%s\", "
                  "\"type\" : \"%s\", "
                  "\"current\" : %d",
                  shunt->contactWidth,
                  shunt->maxCurrent,
                  shunt->power == SHUNT_POWER_OPENED ? "Open" : "Closed",
                  shuntType,
                  shunt->current);
          bp = outBuffer + strlen(outBuffer);
        }
                  
        if ( k + 1 < DSM_MAX_SHUNTS ) {
          sprintf(bp, "},\n");
        } else {
          sprintf(bp, "}\n");
        }
        bp = outBuffer + strlen(outBuffer);
      }
      sprintf(bp, " ] ");
      bp = outBuffer + strlen(outBuffer);
      dsm = BayGetNextDSM(mainBays->bays[i], dsm);
      if ( dsm ) {
        sprintf(bp, "},\n");
      } else {
        sprintf(bp, "}\n");
      }
      bp = outBuffer + strlen(outBuffer);
    }
    sprintf(bp, "] ");
    bp = outBuffer + strlen(outBuffer);
    
    if ( i + 1 < mainBays->bayCount ) {
      sprintf(bp, "},\n");
    } else {
      sprintf(bp, "}\n");
    }
    bp = outBuffer + strlen(outBuffer);
  }
  sprintf(bp, "] }");
  bp = outBuffer + strlen(outBuffer);

  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/*****************************************************************************!
 * Function : HandleWebSerialNumberSend
 *****************************************************************************/
void
HandleWebSocketSerialNumberSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[256];
  sprintf(outBuffer, 
          "{ "
          "\"command\" : \"serialNumber\", "
          "\"value\" : %d "
          "}", InSend->serialNumber);
  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/*****************************************************************************!
 * Function : HandleWebSocketSetPowerSend
 *****************************************************************************/
void
HandleWebSocketSetPowerSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[256];
  SystemTime                            t;

  SystemGetTime(&t);
  sprintf(outBuffer, 
          "{ "
          "\"command\" : \"set\", "
          "\"value\" : \"rectifier\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"setvalue\" : \"%s\", "
          "\"unitNumber\" : %d, "
          "\"bayIndex\"  : %d"
          "}",
          t.hour, t.minute, t.second, t.millisecond,
          InSend->RectifierSetPower.power ? "on" : "off",         
          InSend->RectifierSetPower.unit,
          InSend->RectifierSetPower.bay
          );
  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/*****************************************************************************!
 * Function : HandleWebSocketRectifierInfoSend
 *****************************************************************************/
void
HandleWebSocketRectifierInfoSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[4096];
  SystemTime                            t;

  SystemGetTime(&t);
  sprintf(outBuffer,
          "{"
          "\"command\" : \"info\", "
          "\"type\" : \"rectifier\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"bayIndex\" : %d, "
          "\"unit\" : %d, "
          "\"serialNumber\" : %d, "
	  "\"productNumber\" : \"%s\", "
          "\"current\" : %d, "
          "\"power\" : \"%s\", "
          "\"canAddress\" : %d, "
          "\"alarms\" : { "
          "\"ACH\" : %s, "
          "\"ACL\" : %s, "
          "\"CBO\" : %s, "
          "\"CNV\" : %s, "
          "\"DCU\" : %s, "
          "\"DTS\" : %s, "
          "\"EMS\" : %s, "
          "\"FMA\" : %s, "
          "\"INH\" : %s, "
          "\"NPD\" : %s, "
          "\"PHL\" : %s, "
          "\"PFF\" : %s, "
          "\"PHV\" : %s, "
          "\"PTS\" : %s, "
          "\"RHV\" : %s, "
          "\"SWO\" : %s, "
          "\"FMI\" : %s, "
          "\"HAT\" : %s, "
          "\"TCL\" : %s, "
          "\"SNS\" : %s, "
          "\"DNP\" : %s, "
          "\"DHV\" : %s "
          "} "
          "}",
          t.hour, t.minute, t.second, t.millisecond,
          InSend->RectifierInfo.rectifier->parentNode->bay->index,
          InSend->RectifierInfo.rectifier->parentNode->location.unitNumber,
          InSend->RectifierInfo.rectifier->parentNode->serialNumber,
	  (char*)InSend->RectifierInfo.rectifier->parentNode->partNumber,
          (int)InSend->RectifierInfo.rectifier->outputCurrent,
          InSend->RectifierInfo.rectifier->power ? "On" : "Off",
          InSend->RectifierInfo.rectifier->parentNode->ESNACANAddress,
          InSend->RectifierInfo.rectifier->alarms.ach ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.acl ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.cbo ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.cnv ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.dcu ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.dts ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.ems ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.fma ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.inh ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.npd ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.phl ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.pff ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.phv ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.pts ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.rhv ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.swo ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.fmi ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.hat ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.tcl ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.sns ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.dnp ? "true" : "false",
          InSend->RectifierInfo.rectifier->alarms.dhv ? "true" : "false");
         HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}
 
/*****************************************************************************!
 * Function : HandleWebSocketBayAddSend
 *****************************************************************************/
void
HandleWebSocketBayAddSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[256];
  SystemTime                            t;
  Bay*                                  bay = InSend->BayAdd.bay;
  
  SystemGetTime(&t);
  sprintf(outBuffer, 
          "{ "
          "\"command\" : \"add\", "
          "\"value\" : \"bay\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"name\" : \"%s\", "
          "\"type\" : \"%s\", "
          "\"index\" : %d"
          "}",
          t.hour, t.minute, t.second, t.millisecond,
          bay->name,
          BayTypeToString(bay->type),
          bay->index);
  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/*****************************************************************************!
 * Function : HandleWebSocketStatusSend
 *****************************************************************************/
static void
HandleWebSocketStatusSend
(struct mg_connection* InConnection)
{
  char                                  outBuffer[4096];
  char*                                 buffer;
  int                                   n;
  SystemTime                            t;
  RectifierType*                        rectifier;
  DSMType*                              dsm;
  
  SystemGetTime(&t);
  buffer = outBuffer;
  n = sprintf(buffer, 
              "{ "
              "\"command\" : \"status\", "
              "\"rectifiers\" : [\n");
  Bay*                                  bay = BaySetFindBayByIndex(mainBays, 1);

  buffer += n;

  //! Handle Rectifiers
  for ( rectifier = BayGetFirstRectifier(bay); rectifier; ) {
    n = sprintf(buffer,
                "{ \"bay\" : %d, \"unit\" : %d, \"power\" : \"%s\", \"comm\" : \"%s\", "
                "\"StartUpState\" : %d, "
                "\"alarms\" : %d, \"current\" : %d}\n",
                rectifier->parentNode->bay->index,
                rectifier->parentNode->location.unitNumber,
                rectifier->power ? "true" : "false",
                rectifier->parentNode->nodeTimedOut ? "false" : "true", // Reversed so meaning true == Yes, the unit is communicating with the BCM
                rectifier->parentNode->startUpState,
                rectifier->alarms.allAlarms,
                (int)rectifier->outputCurrent);
    buffer += n;
    rectifier = BayGetNextRectifier(bay, rectifier);
    if ( rectifier ) {
      n = sprintf(buffer, ", ");
      buffer += n;
    }
  }

  n = sprintf(buffer, "],\n");
  buffer += n;
  
  n = sprintf(buffer, 
              "\"dsms\" : [\n");
  buffer += n;

  //! Handle DSMs
  for ( dsm = BayGetFirstDSM(bay); dsm; ) {
    n = sprintf(buffer, "{ \"bay\" : %d, \"unit\" : %d, \"StartUpState\" : %d, \"shunts\" : [\n",
                dsm->parentNode->bay->index,
                dsm->parentNode->location.unitNumber,
                dsm->parentNode->startUpState);
    buffer += n;

    //! Handle Shunts in DSM
    for (int j = 0; j < DSM_MAX_SHUNTS; j++) {
      Shunt*                    shunt = &(dsm->shunts[j]);
      n = sprintf(buffer, "{ \"shunt\" : %d, \"occupied\" : \"%s\", \"power\" : \"%s\", \"current\" : %d }",
                  j,
                  shunt->occupied ? "true" : "false",
                  shunt->power ? "true" : "false",
                  shunt->current);
      buffer += n;
      if ( j + 1 < DSM_MAX_SHUNTS ) {
        n = sprintf(buffer, ",\n");
      } else {
        n = sprintf(buffer, "\n");
      }
      buffer += n;
    }
    n = sprintf(buffer, "] ");
    buffer += n;
    n = sprintf(buffer, "}\n");
    buffer += n;

    dsm = BayGetNextDSM(bay, dsm);
    if ( dsm ) {
      n = sprintf(buffer, ",\n");
    } else {
      n = sprintf(buffer, "\n");
    }
    buffer += n;  
  }
  n = sprintf(buffer, "]\n");
  buffer += n;
  n = sprintf(buffer, "}\n");
  buffer += n;

  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/******************************************************************************!
 * Function : HandleWebSocketCANPacketStringSend
 ******************************************************************************/
void
HandleWebSocketCANPacketStringSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[512];
  SystemTime                                                t;
  SystemGetTime(&t);
  sprintf(outBuffer,
          "{ "
          "\"command\" : \"canpacketstring\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"packet\" : \"%s\""
          "}",
          t.hour, t.minute, t.second, t.millisecond,
          InSend->CANProtocolString.packet);
  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}
/*****************************************************************************!
 * Function : HandleWebSocketCANPacketSend
 *****************************************************************************/
void
HandleWebSocketCANPacketSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[256];
  SystemTime                            t;

  SystemGetTime(&t);
  sprintf(outBuffer,
          "{ "
          "\"command\" : \"canpacket\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"direction\" : \"%s\", "
          "\"id\" : \"%08x\", "
          "\"data\" : \"%08x%08x\", "
          "\"dataLength\" : %d"
          "}",
          t.hour, t.minute, t.second, t.millisecond,
          InSend->CANProtocol.direction == HTTP_SEND_CAN_PACKET_DIRECTION_OUT ? "OUT" : "IN",
          InSend->CANProtocol.id,
          (uint32_t)((InSend->CANProtocol.data >> 32) & 0xFFFFFFFF),
          (uint32_t)(InSend->CANProtocol.data & 0xFFFFFFFF),
          InSend->CANProtocol.dataLength);
  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/*****************************************************************************!
 * Function : HandleWebSocketCANAddressSend
 *****************************************************************************/
void
HandleWebSocketCANAddressSend
(
 struct mg_connection*                  InConnection,
 HTTPServerSendType*                    InSend
)
{
  char                                  outBuffer[256];
  SystemTime                            t;

  SystemGetTime(&t);
  sprintf(outBuffer,
          "{ "
          "\"command\" : \"set\", "
          "\"value\" : \"nodeaddress\", "
          "\"time\" : \"%02d:%02d:%02d.%03d\", "
          "\"nodeType\" : %d, "
          "\"bayIndex\" : %d, "
          "\"unitNumber\" : %d, "
          "\"CANAddress\" : %d"
          "}",
          t.hour, t.minute, t.second, t.millisecond,
          InSend->NodeAddress.nodeType,
          InSend->NodeAddress.bayIndex,
          InSend->NodeAddress.unitNumber,
          InSend->NodeAddress.canAddress);
  HTTPSendWebsocketFrame(InConnection,
                          outBuffer, strlen(outBuffer));
}

/******************************************************************************!
 * Function : HTTPSendRectifierAdd
 ******************************************************************************/
void
HTTPSendRectifierAdd
(
 uint8_t                        InUnit,
 uint8_t                        InBay
)
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_ADD_RECTIFIER;
  SendQueue[SendQueueCurrent].RectifierAdd.unit = InUnit;
  SendQueue[SendQueueCurrent].RectifierAdd.bay = InBay;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}

/******************************************************************************!
 * Function : HTTPSendRectifierInfo
 ******************************************************************************/
void
HTTPSendRectifierInfo
(RectifierType* InRectifier)
{
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_INFO_RECTIFIER;
  SendQueue[SendQueueCurrent].RectifierInfo.rectifier = InRectifier;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
}


/******************************************************************************!
 * Function : HTTPSendBayAdd
 ******************************************************************************/
void
HTTPSendBayAdd
(
 Bay*                           InBay
)
{
  RectifierType*                        rectifier;
  MutexGet(SendQueueMutex);

  if ( SendQueueCurrentFull() ) {
      MutexRelease(SendQueueMutex);
      return;
  }
  SendQueue[SendQueueCurrent].type = HTTP_SEND_ADD_BAY;
  SendQueue[SendQueueCurrent].BayAdd.bay = InBay;
  SendQueueCurrentAdvance();
  MutexRelease(SendQueueMutex);
  for ( rectifier = BayGetFirstRectifier(InBay); rectifier; rectifier = BayGetNextRectifier(InBay, rectifier) ) {
    HTTPSendRectifierAdd(rectifier->parentNode->location.unitNumber,
                         InBay->index);
    
  }
}

/******************************************************************************!
 * Function : SendQueueCurrentAdvance
 ******************************************************************************/
void
SendQueueCurrentAdvance()
{
  SendQueueCurrent++;
  if ( SendQueueCurrent == HTTP_SERVER_SEND_QUEUE_SIZE ) {
    SendQueueCurrent = 0;
  }
}

/******************************************************************************!
 * Function : SendQueueHeadAdvance
 ******************************************************************************/
void
SendQueueHeadAdvance()
{
  SendQueueHead++;
  if ( SendQueueHead == HTTP_SERVER_SEND_QUEUE_SIZE ) {
    SendQueueHead = 0;
  }
}

/******************************************************************************!
 * Function : SendQueueCurrentFull
 ******************************************************************************/
bool
SendQueueCurrentFull()
{
  if ( SendQueueHead == SendQueueCurrent ) {
    return false;
  }
  
  if ( SendQueueCurrent == HTTP_SERVER_SEND_QUEUE_SIZE - 1 && SendQueueHead == 0 ) {
    return true;
  }
  if ( SendQueueCurrent + 1 == SendQueueHead ) {
    return true;
  }
  return false;
}

/*****************************************************************************!
 * Function : FindJSONEntryInt
 *****************************************************************************/
uint32_t
FindJSONEntryInt
(json_value* InJSONDoc, string InTag)
{
  int                                   i;
  for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
    json_object_entry*                j;
    
    j = &(InJSONDoc->u.object.values[i]);
    if ( StringEqual(j->name, InTag) ) {
      return j->value->u.integer;
    }
  }
  return 0;
}

/*****************************************************************************!
 * Function : FindJSONEntryString
 *****************************************************************************/
string
FindJSONEntryString
(json_value* InJSONDoc, string InTag)
{
  int                                   i;
  for ( i = 0 ; i < InJSONDoc->u.object.length; i++) {
    json_object_entry*                j;
    
    j = &(InJSONDoc->u.object.values[i]);
    if ( StringEqual(j->name, InTag) ) {
      return j->value->u.string.ptr;
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : HTTPSendWebsocketFrame
 *****************************************************************************/
void
HTTPSendWebsocketFrame
(struct mg_connection* InConnection, string InBuffer, uint16_t InBufferLen)
{
  mg_send_websocket_frame(InConnection, WEBSOCKET_OP_TEXT,
                          InBuffer, InBufferLen);
}

