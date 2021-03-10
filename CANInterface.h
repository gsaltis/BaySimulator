/******************************************************************************!
 * FILE		: CANInterface.h
 * PROJECT	: Bay Simulator
 * DATE		: June 20 2019
 ******************************************************************************/
#ifndef _caninterface_h_
#define _caninterface_h_

/******************************************************************************!
 * Global Headers
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <StringUtils.h>

/******************************************************************************!
 * Local Headers
 ******************************************************************************/

/******************************************************************************!
 * Exported Macros
 ******************************************************************************/
#define CAN_READ_OK                     1
#define CAN_READ_ERROR                  2
#define CAN_READ_TIMEOUT                3
#define CAN_READ_UNKNOWN                4

/******************************************************************************!
 * Exported Data
 ******************************************************************************/

/******************************************************************************!
 * Exported Type : CANInterface
 ******************************************************************************/
struct _CANInterface
{
  string                                portName;
  int                                   socket;
  bool                                  opened;
};
typedef struct _CANInterface CANInterface;

/******************************************************************************!
 * Exported Functions
 ******************************************************************************/
CANInterface*
CANInterfaceInit
(string InInterfaceName);

void
CANInterfaceClose
(CANInterface* InInterface);

void
CANInterfaceWrite
(CANInterface* InInterface, uint32_t InID, uint64_t InData, uint8_t InDataLength);

uint8_t
CANInterfaceRead
(CANInterface* InInterface, uint32_t* InID, uint64_t* InData, uint8_t* InDataLength);

uint8_t
CANInterfaceTransaction
(CANInterface* InInterface,
 uint32_t InID, uint64_t InData, uint8_t InDataLength,
 uint32_t* InReceiveID, uint64_t* InReceiveData, uint8_t* InReceiveDataLength,
 uint8_t InRetries, uint8_t InTimeout);

void
CANInterfaceReset
();

#endif // _caninterface_h_
