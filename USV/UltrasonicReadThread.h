/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UltrasonicReadThread.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2017年3月21日, 下午5:37
 */

#ifndef ULTRASONICREADTHREAD_H
#define ULTRASONICREADTHREAD_H

#include "Arm.h"
#include "SerialPort2.h"
#include "SerialPort4.h"
void * UltrasonicRecive();

void CreateUltrasonicReciveThread();

extern UINT16 UltrasonicData1[3], UltrasonicData2[3];
#endif /* ULTRASONICREADTHREAD_H */

