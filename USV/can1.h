/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 ============================================================================================================================================
 Author      : samlu (samlu@vciash.com)
 Description : CAN socket handler.
 Status      : DEBUG
 ============================================================================================================================================
 */
/** @defgroup CAN_H_ CAN socket
 *  @{
 */
#ifndef CAN1_H_
#define CAN1_H_

#include <linux/can.h>
#include "Arm.h"
/*-------------------------------------------------------------------------------------------------------------------------------------------
 *
 *                                                            Public Functions
 *
 *-----------------------------------------------------------------------------------------------------------------------------------------*/
/**
 * Initialize CAN socket.
 */
struct can_frame Can1ReadFrame;
struct can_frame Can1WriteFrame;


void Can1_init ( void );
INT32 Can1Read(struct can_frame *frame);
void Can1Write(struct can_frame *frame);

#endif
/** @}*/
