/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AutoPilotData.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2017年2月14日, 上午11:07
 */

#ifndef AUTOPILOTDATA_H
#define AUTOPILOTDATA_H

#include "Arm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    PathMode = 1,
    SampleMode = 2,
    MonitorMode = 3,
    SampleMonitorMode = 4,
}TASKTYPE;

typedef struct autopilotdata
{
    TASKTYPE TaskType;
    DOUBLE Latitude;
    DOUBLE Longitude;
    
    UINT8 SampleBottleNumber;
    UINT8 SampleDepth;
    UINT8 SampleTask;
    UINT8 SampleVolume;
    UINT8 SampleWashFactor;
    
    UINT8 MonitorDepth;
    UINT8 MonitorPeriod;
    UINT8 MonitorPoint;    
}AUTOPILOTDATA;

extern AUTOPILOTDATA *AutoP;
extern UINT8 ReciveAutoPointCount;

void CreatAutoPointBuf(AUTOPILOTDATA **p, UINT8 size);

void USVPathPointSave(AUTOPILOTDATA **p, UINT8 DATA[], UINT32 lat, UINT32 lng, UINT8 *Number, AUTOPILOTDATA *temp);

void USVSamplePointSave(AUTOPILOTDATA **p, UINT8 DATA[], UINT32 lat, UINT32 lng, UINT32 Taskdata, UINT8 *Number, AUTOPILOTDATA *temp);

void USVMonitorPointSave(AUTOPILOTDATA **p, UINT8 DATA[], UINT32 lat, UINT32 lng, UINT32 Taskdata, UINT8 *Number, AUTOPILOTDATA *temp);
#endif /* AUTOPILOTDATA_H */

