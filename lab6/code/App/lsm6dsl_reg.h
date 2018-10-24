/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - energy efficient sensors
 * -- Description : This modul handles the Register values of the Accelerometer

 * -- $Revision: 1607 $
 * -- $Id: lsm6dsl_reg.h 1607 2018-10-11 13:42:52Z angv $
 * --------------------------------------------------------------- */

#ifndef _LSM6DSL_REG
#define _LSM6DSL_REG

#include <stdint.h>


/*  Register adresses of the accelerometer LSM6DSL */

#define      FUNC_CFG_ACC_ADD      0x01
#define      FIFO_CTRL1_ADD        0x06
#define      FIFO_CTRL2_ADD        0x07
#define      FIFO_CTRL3_ADD        0x08
#define      FIFO_CTRL4_ADD        0x09
#define      FIFO_CTRL5_ADD        0x0A
#define      ORIENT_CFG_G_ADD      0x0B
#define      INT1_CTRL_ADD         0x0D
#define      INT2_CTRL_ADD         0x0E
#define      WHO_AM_I_ADD          0x0F
#define      CTRL1_XL_ADD          0x10
#define      CTRL2_G_ADD           0x11
#define      CTRL3_C_ADD           0x12
#define      CTRL4_C_ADD           0x13
#define      SIG_MOT_THRESHOLD_ADD 0x13   //Bank A
#define      CTRL5_C_ADD           0x14
#define      CTRL6_C_ADD           0x15
#define      CTRL7_G_ADD           0x16
#define      CTRL8_XL_ADD          0x17
#define      CTRL9_XL_ADD          0x18
#define      CTRL10_C_ADD          0x19
#define      MASTER_CONFIG_ADD     0x1A

/* end header ----------------------------------------------------- */
#endif
