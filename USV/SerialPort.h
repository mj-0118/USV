/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SerialPort.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016年11月28日, 下午3:23
 */

#ifndef SERIALPORT_H

#define SERIALPORT_H

#include "Arm.h"

#define SERIAL_DEV_NAME  "/dev/ttymxc3"

INT32 serial_constructor(void);

void serial_destructor(void);

INT32 serial_write(INT32 startindex, INT32 len, UINT8 data[ ]);

UINT32 serial_read(UINT8 data[ ]);

UINT16 checksum(UINT8 data[ ], INT32 count);

void serial_init();

void SetRFChannel();

#endif /* SERIALPORT_H */


//~ Formatted by Jindent --- http://www.jindent.com
