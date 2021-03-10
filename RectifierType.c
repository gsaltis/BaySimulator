/*****************************************************************************
 FILE NAME      : RectifierType.c
 DATE           : December 13 2018
 PROJECT        : Node Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertic Company
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "RectifierType.h"
#include "NodeType.h"
#include "CLIThread.h"

/*****************************************************************************!
 * Function : RectifierInitialize
 *****************************************************************************/
void
RectifierInitialize
(
 RectifierType*                         InRectifier
)
{
    if ( NULL == InRectifier ) {
        return;
    }
    memset(InRectifier, 0x00, sizeof(RectifierType));
    InRectifier->parameters.volts = 54331;
    InRectifier->parameters.current = 32768;
    InRectifier->parameters.hsvVolts = 56876;
    InRectifier->parameters.flags.allFlags = 0;
    InRectifier->initStatus = RECT_INIT_STATUS_HAS_SERIAL;
    InRectifier->ratedInputVoltage = 220;
    InRectifier->ratedOutputVoltage = 48;
    InRectifier->ratedOutputCurrent = 200;
    InRectifier->outputCurrent = 32;
    InRectifier->ratedInputVoltageType = RatedTypeAC;
    InRectifier->ratedOutputCurrentType = RatedTypeDC;
    InRectifier->ratedOutputVoltageType = RatedTypeDC;

    InRectifier->initTryCount = 0;
}

/*****************************************************************************!
 * Function : RectifierInitializeStartupStatus
 *****************************************************************************/
void
RectifierInitializeStartupStatus
(
 RectifierType*                         InRectifier
)
{
    InRectifier->initStatus = RECT_INIT_STATUS_HAS_SERIAL;
    InRectifier->initTryCount = 0;
}

/*****************************************************************************!
 * Function : RectifierHasAlarms
 *****************************************************************************/
bool
RectifierHasAlarms
(
 RectifierType*                         InRectifier,
 bool                                   InIncludeLSI
)
{
    uint32_t                            mask;

    if ( InRectifier == NULL ) {
         return false;
    }
    mask = RECT_MAJOR_ALARMS_MASK;
    if ( InIncludeLSI ) {
        mask = RECT_MAJOR_ALARMS_NO_LSI_MASK;
    }
    return InRectifier->alarms.allAlarms & mask ? true : false;
}

/*****************************************************************************!
 * Function : RectifierIsOn
 *****************************************************************************/
bool
RectifierIsOn
(
 RectifierType*                         InRectifier
)
{
    if ( NULL == InRectifier ) {
        return false;
    }
    return !InRectifier->command.bit.tco;
}

/*****************************************************************************!
 * Function : RectifierClearLoadShare
 *****************************************************************************/
void
RectifierClearLoadShare
(
 RectifierType*                         InRectifier
)
{
    if ( NULL == InRectifier ) {
        return;
    }
    InRectifier->field.RectifierLoadShare = 0;
}

/*****************************************************************************!
 * Function : RectifierUpdateInitStatus
 *****************************************************************************/
void
RectifierUpdateInitStatus
(
 RectifierType*                         InRectifier,
 uint8_t                                InNodeNumber,
 uint8_t                                InInitStatusFlag
)
{
    if ( NULL == InRectifier ) {
        return;
    }

    InRectifier->initStatus |= InInitStatusFlag;
    if ( (InRectifier->initStatus & RECT_INIT_STATUS_HAS_ALL) == RECT_INIT_STATUS_HAS_ALL) {
        RectifierAnnounce(InRectifier, InNodeNumber);
    }
}

/*****************************************************************************!
 * Function : RectifierAnnounce
 *****************************************************************************/
void
RectifierAnnounce
(
 RectifierType*                         InRectifier,
 uint8_t                                InNodeNumber
)
{
    if ( InRectifier == NULL ) {
        return;
    }

    InRectifier->initStatus |= RECT_INIT_STATUS_HAS_ANNOUNCED;
}

/*****************************************************************************!
 * Function : RectifierTurnOff
 *****************************************************************************/
void
RectifierTurnOff
(
 RectifierType*                         InRectifier
)
{
    if ( NULL == InRectifier ) {
        return;
    }
    InRectifier->power = false;
}

/*****************************************************************************!
 * Function : RectifierTurnOn
 *****************************************************************************/
void
RectifierTurnOn
(
 RectifierType*                         InRectifier
)
{
    if ( NULL == InRectifier ) {
        return;
    }
    InRectifier->power = true;
}

/*****************************************************************************!
 * Function : RectifierDisplay
 *****************************************************************************/
void
RectifierDisplay
(RectifierType* InRectifier)
{
  uint32_t intPart, fracPart, outputCurrent, voltOut, HVS;

  if ( InRectifier == NULL ) {
    return;
  }
  printf("%s               Rectifier%s\r\n", HeadingStyle, ColorWhite);       
  printf("     Rated Input Voltage : %d %s\r\n",
         InRectifier->ratedInputVoltage,
         InRectifier->ratedInputVoltageType == RatedTypeAC ? "AC" :
         InRectifier->ratedInputVoltageType == RatedTypeDC ? "DC" : "Un");
  printf("                   Power : %s\r\n", InRectifier->power ? "On" : "Off");
  printf("    Rated Output Voltage : %d %s\r\n",
         InRectifier->ratedOutputVoltage,
         InRectifier->ratedOutputVoltageType == RatedTypeAC ? "AC" :
         InRectifier->ratedOutputVoltageType == RatedTypeDC ? "DC" : "Un");
  printf("    Rated Output Current : %d\r\n",
         InRectifier->ratedOutputCurrent);
  printf("    Max Current Capacity : %d\r\n", InRectifier->maxCurrentCapacity);
  printf("              Parameters :\r\n");
  voltOut = (uint32_t)InRectifier->parameters.volts;
  intPart = voltOut / 1000;
  fracPart = voltOut % 1000;
  printf("                   Volts : %d.%03d\r\n", intPart, fracPart);
  HVS = (uint32_t)InRectifier->parameters.hsvVolts;
  intPart = HVS / 1000;
  fracPart = HVS % 1000;
  printf("               HSV Volts : %d.%03d\r\n", intPart, fracPart);
  printf("                 Current : %d\r\n", InRectifier->parameters.current);
  printf("                   Flags : %02x\r\n", InRectifier->parameters.flags.allFlags);
  printf("           Alarm Cut off : %s\r\n", InRectifier->parameters.flags.flags.aco ? "Set" : "-");
  printf("            Req Download : %s\r\n", InRectifier->parameters.flags.flags.rdl ? "Set" : "-");
  printf("            Text Changed : %s\r\n", InRectifier->parameters.flags.flags.ctc ? "Set" : "-");
  printf("         Write Alarm Log : %s\r\n", InRectifier->parameters.flags.flags.wal ? "Set" : "-");
  printf("  Disable Constant Power : %s\r\n", InRectifier->parameters.flags.flags.dcp ? "Set" : "-");
  printf("            Test/Eq Mode : %s\r\n", InRectifier->parameters.flags.flags.teq ? "Set" : "-");
  printf("   Disable Low Speed Fan : %s\r\n", InRectifier->parameters.flags.flags.dls ? "Set" : "-");
  printf("      Turn Rectifier Off : %s\r\n", InRectifier->parameters.flags.flags.tco ? "Set" : "-");
       
  outputCurrent = (uint32_t)(InRectifier->outputCurrent * 1000);
  intPart = outputCurrent / 1000;
  fracPart = outputCurrent % 1000;
  printf("          Output Current : %d.%03d\r\n", intPart, fracPart);
  printf("                  Alarms : %08x\r\n", InRectifier->alarms.allAlarms);
  printf("                  Status : %04x\r\n", InRectifier->status.allStatus);
}

/*****************************************************************************!
 * Function : RectifierSetAlarmByName
 *****************************************************************************/
bool
RectifierSetAlarmByName
(RectifierType* InRectifier, string InAlarmName, bool InAlarmValue)
{
  if ( NULL == InRectifier ) {
    return false;
  }
  if ( StringEqualNoCase(InAlarmName, "ACH") ) {
    InRectifier->alarms.ach = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "ACL") ) {
    InRectifier->alarms.acl = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "CBO") ) {
    InRectifier->alarms.cbo = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "CNV") ) {
    InRectifier->alarms.cnv = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "DHV") ) {
    InRectifier->alarms.dhv = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "DCU") ) {
    InRectifier->alarms.dcu = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "DTS") ) {
    InRectifier->alarms.dts = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "EMS") ) {
    InRectifier->alarms.ems = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "FMA") ) {
    InRectifier->alarms.fma = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "INH") ) {
    InRectifier->alarms.inh = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "NPD") ) {
    InRectifier->alarms.npd = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "PHL") ) {
    InRectifier->alarms.phl = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "PFF") ) {
    InRectifier->alarms.pff = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "PHV") ) {
    InRectifier->alarms.phv = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "PTS") ) {
    InRectifier->alarms.pts = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "RHV") ) {
    InRectifier->alarms.rhv = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "SWO") ) {
    InRectifier->alarms.swo = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "FMI") ) {
    InRectifier->alarms.fmi = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "HAT") ) {
    InRectifier->alarms.hat = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "TCL") ) {
    InRectifier->alarms.tcl = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "SNS") ) {
    InRectifier->alarms.sns = InAlarmValue;
  } else if ( StringEqualNoCase(InAlarmName, "DNP") ) {
    InRectifier->alarms.dnp = InAlarmValue;
  } else {
    return false;
  }
  RectifierCheckAlarms(InRectifier);
  return true;
}

/*****************************************************************************!
 * Function : RectifierCheckAlarms
 *****************************************************************************/
void
RectifierCheckAlarms
(RectifierType* InRectifier)
{
  if ( InRectifier == NULL ) {
    return;
  }
  InRectifier->alarms.aaa =  
    InRectifier->alarms.ach |
    InRectifier->alarms.acl |
    InRectifier->alarms.cbo |
    InRectifier->alarms.cnv |
    InRectifier->alarms.dhv |
    InRectifier->alarms.dcu |
    InRectifier->alarms.dts |
    InRectifier->alarms.ems |
    InRectifier->alarms.fma |
    InRectifier->alarms.inh |
    InRectifier->alarms.npd |
    InRectifier->alarms.phl |
    InRectifier->alarms.pff |
    InRectifier->alarms.phv |
    InRectifier->alarms.pts |
    InRectifier->alarms.rhv |
    InRectifier->alarms.swo |
    InRectifier->alarms.fmi |
    InRectifier->alarms.hat |
    InRectifier->alarms.tcl |
    InRectifier->alarms.sns |
    InRectifier->alarms.dnp ? true : false;
}

