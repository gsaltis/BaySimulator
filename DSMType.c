/*****************************************************************************
 FILE NAME      : DSMType.c
 DATE           : December 13 2018
 PROJECT        : Bay Simulator
 COPYRIGHT      : Copyright (C) 2018 by Vertiv Corp.
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "DSMType.h"
#include "CLIThread.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Function : GetShuntBits
 * Purpose  : Wrapper for getting the shunt bits from a DSM
 * Returns  : uint8_t where bit 0 is low bit and bit 1 is high bit
 *****************************************************************************/
uint8_t
DSMGetShuntBits
(
 DSMType*                               InDSM,
 int                                    InBitIndex
)
{
  int                                 lobit;
  int                                 hibit;
  int                                 offset, byte, bit;
  if ( NULL == InDSM ) {
    return 0;
  }

  if ( InBitIndex < 1 ) {
    return 0;
  }

  if ( InBitIndex > DSM_MAX_SHUNT_COUNT ) {
    return 0;
  }

  offset = InBitIndex - 1;
  byte = 2 - (offset / 8);
  bit =  offset % 8;
    
  hibit = (InDSM->shuntAlarmHiByte[byte] >> bit) & 1;
  lobit = (InDSM->shuntAlarmLoByte[byte] >> bit) & 1;

  return (hibit << 1) | lobit;
}

/*****************************************************************************!
 * Function : DSMGetIDBits
 * Purpose  : Wrapper for getting the shunt bits from a DSM
 * Returns  : uint8_t where bit 0 is low bit and bit 1 is high bit
 *****************************************************************************/
uint8_t
DSMGetIDBits
(
 DSMType*                               InDSM,
 int                                    InByteIndex,
 int                                    InBitIndex
)
{
  int                                 idbit;
  int                                 offset, byte, bit;
  if ( NULL == InDSM ) {
    return 0;
  }

  if ( InByteIndex > 2 ) {
    return 0;
  }
  if ( InBitIndex < 1 ) {
    return 0;
  }

  if ( InBitIndex > DSM_MAX_SHUNT_COUNT ) {
    return 0;
  }

  offset = InBitIndex - 1;
  byte = 2 - (offset / 8);
  bit =  offset % 8;
    
  idbit = (InDSM->IDBytes[InByteIndex][byte] >> bit) & 1;

  return (idbit);
}

/*****************************************************************************!
 * Function : DSMInsertShunt
 * Purpose  : Insert a shunt into a DSM
 *****************************************************************************/
void
DSMInsertShunt
(
 DSMType*                               InDSM,
 uint8_t                                InContactIndex,
 uint8_t                                InContactWidth,
 uint32_t                               InContactMaxCurrent,
 ShuntType                              InShuntType 
)
{
  uint8_t                               i, index;
  Shunt*                                shunt;
  
  if (NULL == InDSM ) {
    return;
  }
  
  if ( InContactWidth > DSM_MAX_SHUNTS ) {
    return;
  }
  index = InContactIndex-1;
  shunt = &(InDSM->shunts[index]);
  shunt->contactWidth = InContactWidth;
  shunt->maxCurrent = InContactMaxCurrent;
  shunt->type = InShuntType;
  shunt->current = 0;
  shunt->occupied = true;
  shunt->removed = false;
  shunt->power = SHUNT_POWER_CLOSED;
  for (i = index+1; i < index + InContactWidth; i++) {
    if ( i < DSM_MAX_SHUNTS ) {
      InDSM->shunts[i].occupied = false;
    }
  }
}

/******************************************************************************!
 * Function : DSMShuntFits
 ******************************************************************************/
bool
DSMShuntFits
(DSMType* InDSM, uint8_t InContactIndex, uint8_t InContactWidth)
{
  uint8_t                               index, i;
  uint8_t                               startIndex, endIndex, nextAvailable;

  if ( NULL == InDSM ) {
    return false;
  }

  if ( InContactIndex < 1 || InContactIndex > DSM_MAX_SHUNTS ) {
    return false;
  }
  index = InContactIndex - 1;

  if ( InDSM->shunts[index].occupied ) {
    return false;
  }
  if ( InContactIndex < 6 ) {
    startIndex = 0;
    endIndex = 6;
  } else {
    startIndex = 6;
    endIndex = 12;
  }

  nextAvailable = startIndex;
  for ( i = startIndex; i < endIndex; i++ ) {
    if ( InDSM->shunts[i].occupied ) {
      nextAvailable = nextAvailable + InDSM->shunts[i].contactWidth;
    }
  }
  return nextAvailable == index;
}

/******************************************************************************!
 * Function : DSMShuntRemove
 ******************************************************************************/
void
DSMShuntRemove
(DSMType* InDSM, uint8_t InContactIndex)
{
  Shunt*                                shunt;
  if ( InDSM == NULL ) {
    return;
  }

  if ( InContactIndex < 1 || InContactIndex > DSM_MAX_SHUNTS ) {
    return;
  }

  shunt = &(InDSM->shunts[InContactIndex-1]);
  shunt->contactWidth = 0;
  shunt->type = SHUNT_TYPE_NONE;
  shunt->maxCurrent = 0;
  shunt->current = 0;
  shunt->occupied = false;
  shunt->alarm = 0;
  shunt->status = SHUNT_STATUS_NONE;
  shunt->removed = true;
}

/******************************************************************************!
 * Function : DSMDisplay
 ******************************************************************************/
void
DSMDisplay
(DSMType* InDSM)
{
  int                                   i;
  if ( InDSM == NULL ) {
    return;
  }

  printf("%s                     DSM%s\r\n", HeadingStyle, ColorWhite);
  printf("  --DESCRIPTION---------  \r\n");
  printf("           RTR must == 0 : %s\r\n", InDSM->description.bit.rez ? "Set" : "-");
  printf("    2 Distribution buses : %s\r\n", InDSM->description.bit.tdb ? "Set" : "-");
  printf("              Data Bytes : %d\r\n", InDSM->description.bit.tdb ? 2 : 4);
  printf("       One Shunt Per Bus : %s\r\n", InDSM->description.bit.osp ? "Set" : "-");
  printf("         shunts are 50mV : %s\r\n", InDSM->description.bit.g50 ? "Set" : "-");
  printf("           Shunt Gain #1 : %s\r\n", InDSM->description.bit.gs1 ? "Set" : "-");
  printf("           Shunt Gain #2 : %s\r\n", InDSM->description.bit.gs2 ? "Set" : "-");
  printf("        Top Distribution : %s\r\n", InDSM->description.bit.top ? "Set" : "-");
  printf("   Battery LVD installed : %s\r\n", InDSM->description.bit.bli ? "Set" : "-");
  printf("      Load LVD installed : %s\r\n", InDSM->description.bit.lli ? "Set" : "-");
  printf("     Max # points on bus : %d\r\n", InDSM->description.bit.maxPointsOnBus);
  printf("  --COMMAND-------------  \r\n");
  printf("            Alarm Cutoff : %s\r\n", InDSM->command.bit.lps_aco ? "Set" : "-");
  printf("        Request Download : %s\r\n", InDSM->command.bit.rdl ? "Set" : "-");
  printf("   Customer Text Changed : %s\r\n", InDSM->command.bit.ctc ? "Set" : "-");
  printf(" Shunt Inventory Changed : %s\r\n", InDSM->command.bit.sic ? "Set" : "-");
  printf("         Write Alarm Log : %s\r\n", InDSM->command.bit.wal ? "Set" : "-");
  printf("         Force Reconnect : %s\r\n", InDSM->command.bit.frc ? "Set" : "-");
  printf("       Forced Disconnect : %s\r\n", InDSM->command.bit.fdc ? "Set" : "-");
  printf("  --BUS MILLIVOLTS------  \r\n");
  printf("        Bus 1 millivolts : %d\r\n", InDSM->busVoltages[0]);
  printf("        Bus 2 millivolts : %d\r\n", InDSM->busVoltages[1]);
  printf("  --SHUNTS----\n");
  for (i = 0; i < DSM_MAX_SHUNTS; i++) {
    Shunt* shunt = &(InDSM->shunts[i]);
    Shunt* rshunt;
    printf("      Shunt %2d", i + 1);
    printf("  %soccupied", shunt->occupied ? "    " : "not ");
          if ( shunt->occupied ) {
      if ( shunt->contactWidth > 0 || !shunt->occupied) {
        rshunt = shunt;
      } else if ( shunt->occupied ) {
        int k = i;
        while (InDSM->shunts[k].contactWidth == 0) {
          k--;
        }
        rshunt = &(InDSM->shunts[k]);
      }
      printf(" %s", shunt->alarm ? "alarm" : "     ");
      printf(" %02x ", GetShuntIDBits(rshunt));
      if ( shunt->current > shunt->maxCurrent ) {
        printf("%s", ColorRed);
      }
      printf(" %5d ", shunt->current);
      if ( shunt->current > shunt->maxCurrent ) {
        printf("%s", ColorWhite);
      }
      if ( shunt->occupied && shunt->contactWidth > 0 ) {
        printf("%d - %4d", shunt->contactWidth, shunt->maxCurrent);
        if ( shunt->type == SHUNT_TYPE_BREAKER ) {
          printf(" breaker");
        } else if ( shunt->type == SHUNT_TYPE_FUSE ) {
          printf(" fuse");
        } else if ( shunt->type == SHUNT_TYPE_BULLET ) {
          printf(" bullet");
        }
      }
            printf(" %s", shunt->power == SHUNT_POWER_OPENED ? "Open" : "Closed");
    }
    printf("\n");
  }
}

/*****************************************************************************!
 * Functio : DSMShuntOccupied
 *****************************************************************************/
bool
DSMShuntOccupied
(DSMType* InDSM, uint8_t InSlotIndex)
{
  if ( NULL == InDSM ) {
    return false;
  }
  if ( InSlotIndex < 1 || InSlotIndex > DSM_MAX_SHUNTS ) {
    return false;
  }

  return InDSM->shunts[InSlotIndex-1].occupied;
}


