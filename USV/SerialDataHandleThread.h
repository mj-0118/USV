/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SerialDateHandlePthread.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016年12月1日, 下午4:57
 */
#ifndef SERIALDATAHANDLETHREAD_H
#define SERIALDATAHANDLETHREAD_H

#include "Arm.h"

typedef struct RemoteManual {
    UINT8 MotorServo;
    UINT8 MotorPropeller;
} REMOTEMANUAL;

void * SerialDateHandleThread();

void CreateSerialDataHandleThread(void);

void RFReviceDateHandlerInit(UINT32 ReciverData, UINT8 Rebuffer[]);

UINT16 Checksum(UINT8 data[], UINT32 start, UINT32 length);

void BoatDataAutoMessage();

void BoatDataManualRFMessage();

extern UINT8 SendMessageCount;

#endif  /* SERIALDATEHANDLEPTHREAD_H */


//~ Formatted by Jindent --- http://www.jindent.com
