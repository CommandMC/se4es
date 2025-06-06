/**************************************************************************************************
  Revised:        $Date: 2007-07-06 11:19:00 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Copyright 2007 Texas Instruments Incorporated.  All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights granted under
  the terms of a software license agreement between the user who downloaded the software,
  his/her employer (which must be your employer) and Texas Instruments Incorporated (the
  "License"). You may not use this Software unless you agree to abide by the terms of the
  License. The License limits your use, and you acknowledge, that the Software may not be
  modified, copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the foregoing purpose,
  you may not use, reproduce, copy, prepare derivative works of, modify, distribute,
  perform, display or sell this Software and/or its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS�
  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
  IN NO EVENT SHALL TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
  INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST
  DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY
  THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *   BSP (Board Support Package)
 *   Target : Texas Instruments CC430EM
 *   Board definition file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#ifndef BSP_BOARD_DEFS_H
#define BSP_BOARD_DEFS_H


/* ------------------------------------------------------------------------------------------------
 *                                     Board Unique Define
 * ------------------------------------------------------------------------------------------------
 */
#define BSP_BOARD_CC430EM


/* ------------------------------------------------------------------------------------------------
 *                                           Mcu
 * ------------------------------------------------------------------------------------------------
 */
#include "machine/simpliciti/bsp_msp430_defs.h"


/* ------------------------------------------------------------------------------------------------
 *                                          Clock
 * ------------------------------------------------------------------------------------------------
 */
#include "machine/simpliciti/bsp_config.h"
#define __bsp_CLOCK_MHZ__         BSP_CONFIG_CLOCK_MHZ


/* ------------------------------------------------------------------------------------------------
 *                                     Board Initialization
 * ------------------------------------------------------------------------------------------------
 */
void delay_us(unsigned int us); //from "machine/timer.h"

#define BSP_BOARD_C               "bsp_board.c"
#define BSP_INIT_BOARD()          BSP_InitBoard()
#define BSP_DELAY_USECS(x)        delay_us(x)

void BSP_InitBoard(void);
void BSP_Delay(uint16_t usec);

/* ************************************************************************************************
 *                                   Compile Time Integrity Checks
 * ************************************************************************************************
 */

/**************************************************************************************************
 */
#endif
