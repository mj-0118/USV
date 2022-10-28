/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AllPthreadCreate.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016年11月30日, 下午8:01
 */
#ifndef ALLPTHREADCREATE_H
#define ALLPTHREADCREATE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "SerialDataHandleThread.h"
#include "AutoPilotThread.h"
#include "CanbusDataHandleThread.h"
#include "TakingSampleByRemoteThread.h"
#include "UltrasonicReadThread.h"
#include "Arm.h"
#include "SerialPort.h"
#include "ReadCan2Thread.h"

pthread_t AutoPilot;
pthread_t SerialDateHandle;
pthread_t TakingSampleByRemote;
pthread_t CanbusDataHandle;
pthread_t UltrasonicReciveHandle;
pthread_t ReadCan2Handle;
pthread_t WashMode;
pthread_mutex_t mutex;

void mutex_init();

void CreateSerialCanbusPthread();
#endif /* ALLPTHREADCREATE_H */


//~ Formatted by Jindent --- http://www.jindent.com
