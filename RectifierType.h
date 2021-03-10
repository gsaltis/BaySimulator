/*****************************************************************************
 FILE NAME      : RectifierType.h
 DATE           : July 17 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2019 by Vertiv Company
******************************************************************************/
#ifndef _rectifiertype_h_
#define _rectifiertype_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <StringUtils.h>

#pragma pack(1)

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define RECT_MAJOR_ALARMS_MASK          0x00F77FBE
#define RECT_MAJOR_ALARMS_NO_LSI_MASK   0x00B77FBE

#define RECT_INIT_STATUS_NONE           0x00
#define RECT_INIT_STATUS_HAS_SERIAL     0x01
#define RECT_INIT_STATUS_HAS_PRODUCT1   0x02
#define RECT_INIT_STATUS_HAS_PRODUCT2   0x04
#define RECT_INIT_STATUS_HAS_APP_VERSION 0x08
#define RECT_INIT_STATUS_HAS_ALL        0x0F
#define RECT_INIT_STATUS_HAS_ANNOUNCED  0x10

#define RECT_INIT_TRY_COUNT             10
#define RECTIFIER_DEFAULT_PART_NUMBER   "486534001"

/******************************************************************************!
 * Imported Type
 ******************************************************************************/
struct NodeType;

/*****************************************************************************!
 * Exported Type : RectifierAlarmType
 *****************************************************************************/
typedef union {
    uint8_t      byte[4];
    uint16_t     word[2];
    uint32_t     dwrd;
    struct {
        uint8_t aaa:1, b06:1, b05:1, b04:1, b03:1, b02:1, b01:1, b00:1;
        uint8_t dup:1, lsi:1, dcu:1, phv:1, tcl:1, dts:1, pff:1, phl:1;
        uint8_t fmi:1, ach:1, pts:1, npd:1, rhv:1, fma:1, inh:1, cbo:1;
        uint8_t cnv:1, hat:1, ems:1, swo:1, dhv:1, acl:1, ndr:1, sns:1;
    } bit;
} RectifierAlarmType;

/*****************************************************************************!
 * Exported : RectifierAlarms
 *****************************************************************************/
typedef union  {
    struct {
        uint32_t                        sns:1;       // Sense Fail
        uint32_t                        ndr:1;       // No Data Received 
        uint32_t                        acl:1;       // AC Low Alarm 
        uint32_t                        dhv:1;       // DC/DC Conv HVS
                
        uint32_t                        swo:1;       // Switch Turned Off
        uint32_t                        ems:1;       // Emergency Stop 
        uint32_t                        hat:1;       // High Ambient Temperature 
        uint32_t                        cnv:1;       // DC/DC Conv Fail 

        uint32_t                        cbo:1;       // Circuit Breaker Off 
        uint32_t                        inh:1;       // Internal Inhibit                  
        uint32_t                        fma:1;       // Fan Fail Major                  
        uint32_t                        rhv:1;       // Remote HVS               

        uint32_t                        npd:1;       // No Primary Data Stream
        uint32_t                        pts:1;       // Power Factor Thermal Shutdown                  
        uint32_t                        ach:1;       // AC High Alarm 
        uint32_t                        fmi:1;       // Fan Fail Minor               

        uint32_t                        phl:1;       // Phase Lost                  
        uint32_t                        pff:1;       // Power Factor Fail
        uint32_t                        dts:1;       // DC/DC conv. Thermal Shutdown        
        uint32_t                        tcl:1;       // Thermal Current Limit

        uint32_t                        phv:1;       // Power Factor HVS                  
        uint32_t                        dcu:1;       // DC/DC current unbalanced
        uint32_t                        lsi:1;       // Load Share Imbalance 
        uint32_t                        dnp:1;       // Duplicate Node                       

        uint32_t                        skip1:7;
        uint32_t                        aaa:1;       // Any Alarm Active                  
   };
    uint32_t                            allAlarms;
} RectifierAlarms;

/*****************************************************************************!
 * Exported Type : RectifierStatus;
 *****************************************************************************/
typedef union {
    struct {
        uint8_t                         skip1:2;
        uint8_t                         vin:1; 
        uint8_t                         cpr:1; 
        uint8_t                         lin:1; 
        uint8_t                         rls:1; 
        uint8_t                         lsc:1;
        uint8_t                         skip2:1;
    } status;
    uint8_t                             allStatus;
} RectifierStatus;

/*****************************************************************************!
 * Exported Type : RectifierCommandType
 *****************************************************************************/
typedef union {
    uint8_t      byte;
    struct {
        // disable low speed has to be put is specially when sendingl the command
        uint8_t lps_aco:1, rdl:1, ctc:1, wal:1, cur:1, bay:1, new:1, tco:1;  
    } bit;
} RectifierCommandType;

/*****************************************************************************!
 * Exported Type : RectifierControlType
 *****************************************************************************/
typedef union {
    uint8_t      byte;
    struct {
        uint8_t b07:1, b06:1, b05:1, b04:1, identify:1, hvs_reset:1, dls:1, tco:1;      
    } bit;
} RectifierControlType;

/*****************************************************************************!
 * Exported Type : RectifierDualUseType
 *****************************************************************************/
typedef union {
    uint16_t                            RectifierLoadShare;
    uint16_t                            DummyGbbAddress;
} RectifierDualUseType;

/*****************************************************************************!
 * Exported Type : RectifierDescription
 *****************************************************************************/
typedef struct {
    uint16_t                            rez:1;
    uint16_t                            :3;
    uint16_t                            ratedCurrent:12;
} RectifierDescription;
    
/*****************************************************************************!
 * Exported Type : RectifierParameterType
 *****************************************************************************/
typedef struct
{
    uint16_t                            volts;
    uint16_t                            current;
    uint16_t                            hsvVolts;
    union {
        struct {
            uint8_t                     tco:1;
            uint8_t                     dls:1;
            uint8_t                     teq:1;
            uint8_t                     dcp:1;
            uint8_t                     wal:1;
            uint8_t                     ctc:1;
            uint8_t                     rdl:1;
            uint8_t                     aco:1;
        } flags;
        uint8_t                         allFlags;
    } flags;
} RectifierParameterType;

/*****************************************************************************!
 * Exported Type : RectifierRatedType
 *****************************************************************************/
typedef enum
{
    RatedTypeNone,
    RatedTypeAC,
    RatedTypeDC
} RectifierRatedType;

/*****************************************************************************!
 * Exported Type : RectifierType
 *****************************************************************************/
typedef struct {
    float                               outputCurrent;
    RectifierRatedType                  ratedInputVoltageType;
    RectifierRatedType                  ratedOutputVoltageType;
    RectifierRatedType                  ratedOutputCurrentType;
    uint16_t                            ratedInputVoltage;
    uint16_t                            ratedOutputVoltage;
    uint16_t                            ratedOutputCurrent;
    uint16_t                            maxCurrentCapacity;
    RectifierDualUseType                field;
    RectifierAlarms                     alarms;
    RectifierStatus                     status;
    RectifierCommandType                command;
    RectifierParameterType              parameters;
    RectifierControlType                control;
    RectifierDescription                description;
    uint8_t                             initStatus;
    uint8_t                             initTryCount;
    bool                                power;
    struct _NodeType*                   parentNode;
} RectifierType;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
RectifierInitialize
(RectifierType* InRectifier);

void
RectifierInitializeStartupStatus
(RectifierType* InRectifier);

bool
RectifierHasAlarms
(RectifierType* InRectifier, bool InIncludeLSI);

bool
RectifierIsOn
(RectifierType* InRectifier);

void
RectifierClearLoadShare
(RectifierType* InRectifier);

void
RectifierUpdateInitStatus
(RectifierType* InRectifier, uint8_t InNodeNumber, uint8_t InInitStatusFlag);

void
RectifierAnnounce
(RectifierType* InRectifier, uint8_t InNodeNumber);

void
RectifierTurnOff
(RectifierType* InRectifier);

void
RectifierTurnOn
(RectifierType* InRectifier);

void
RectifierDisplay
(RectifierType* InRectifier);

bool
RectifierSetAlarmByName
(RectifierType* InRectifier, string InAlarmName, bool InAlarmValue);

void
RectifierCheckAlarms
(RectifierType* InRectifier);

#endif /* _rectifiertype_h_*/
