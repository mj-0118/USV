/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SerialPort2.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2017年3月21日, 下午1:59
 */

#ifndef SERIALPORT2_H
#define SERIALPORT2_H

#include "Arm.h"

#define UART2_DEV_NAME  "/dev/ttymxc2"

INT32 Uart2_constructor( void );

void Uart2_destructor( void );

UINT16 Uart2_Ultrasonic_read();

void Uart2_init(void);
#endif /* SERIALPORT2_H */

