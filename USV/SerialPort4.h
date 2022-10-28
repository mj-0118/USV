/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SerialPort4.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2017年3月21日, 下午1:59
 */

#ifndef SERIALPORT4_H
#define SERIALPORT4_H

#include "Arm.h"

#define UART4_DEV_NAME  "/dev/ttymxc4"

INT32 Uart4_constructor( void );

void Uart4_destructor( void );

UINT16 Uart4_Ultrasonic_read();

void Uart4_init(void);

#endif /* SERIALPORT4_H */

