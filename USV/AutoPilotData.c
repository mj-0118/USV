/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Arm.h"
#include "AutoPilotData.h"
#include "CanbusDataHandleThread.h"

void CreatAutoPointBuf(AUTOPILOTDATA **p, UINT8 size) {
    *p = (AUTOPILOTDATA *) malloc(sizeof (AUTOPILOTDATA) * size);
    if (*p == NULL) {
        printf("*P MALLOC IS FAILER\n");
    }
}

void USVPathPointSave(AUTOPILOTDATA **p, UINT8 DATA[], UINT32 lat, UINT32 lng, UINT8 *Number, AUTOPILOTDATA *temp) {
    UINT8 Latitude[8], Longitude[8], i;

    i = *Number;
    memcpy(Latitude, DATA + lat, sizeof (UINT8) * 8);
    memcpy(Longitude, DATA + lng, sizeof (UINT8) * 8);

    memset(temp, 0, sizeof (AUTOPILOTDATA));    //clear 0;

    temp->TaskType = PathMode;
    temp->Latitude = BytesToDouble(Latitude);
    temp->Longitude = BytesToDouble(Longitude);

    memcpy((*p) + i, temp, sizeof (AUTOPILOTDATA));
    (*Number)++;
}

void USVSamplePointSave(AUTOPILOTDATA **p, UINT8 DATA[], UINT32 lat, UINT32 lng, UINT32 Taskdata, UINT8 *Number, AUTOPILOTDATA *temp) {
    UINT8 Latitude[8], Longitude[8], i, TaskData[5];

    i = *Number;
    memcpy(Latitude, DATA + lat, sizeof (UINT8) * 8);
    memcpy(Longitude, DATA + lng, sizeof (UINT8) * 8);
    memcpy(TaskData, DATA + Taskdata, sizeof (UINT8) * 5);

    memset(temp, 0, sizeof (AUTOPILOTDATA)); //clear 0;

    temp->TaskType = SampleMode;
    temp->Latitude = BytesToDouble(Latitude);
    temp->Longitude = BytesToDouble(Longitude);
    temp->SampleDepth = TaskData[0];
    temp->SampleTask = TaskData[1];
    temp->SampleVolume = TaskData[2];
    temp->SampleBottleNumber = TaskData[3];
    temp->SampleWashFactor = TaskData[4];

    memcpy((*p) + i, temp, sizeof (AUTOPILOTDATA));
    (*Number)++;
}

void USVMonitorPointSave(AUTOPILOTDATA **p, UINT8 DATA[], UINT32 lat, UINT32 lng, UINT32 Taskdata, UINT8 *Number, AUTOPILOTDATA *temp) {
    UINT8 Latitude[8], Longitude[8], i, TaskData[3];

    i = *Number;
    memcpy(Latitude, DATA + lat, sizeof (UINT8) * 8);
    memcpy(Longitude, DATA + lng, sizeof (UINT8) * 8);
    memcpy(TaskData, DATA + Taskdata, sizeof (UINT8) * 3);

    memset(temp, 0, sizeof (AUTOPILOTDATA));    //clear 0;

    temp->TaskType = MonitorMode;
    temp->Latitude = BytesToDouble(Latitude);
    temp->Longitude = BytesToDouble(Longitude);
    temp->MonitorDepth = TaskData[0];
    temp->MonitorPeriod = TaskData[1];
    temp->MonitorPoint = TaskData[2];

    memcpy((*p) + i, temp, sizeof (AUTOPILOTDATA));
    (*Number)++;
}