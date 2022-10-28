/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ReadCan2Thread.h"
#include "MonitorData.h"
#include "can1.h"
#include "can0.h"
#include <linux/can.h>

MONITORDATA MonitorData[10];
char g4Longitude[20];
char g4Latitude[20];
struct can_frame CanLongitude, CanLatitude, CanBattery;

void * ReadCan2Thread() {
    UINT32 RetData;
    while (1) {
        //sleep ( 1 );
        RetData = Can1Read(&Can1ReadFrame);
        if (RetData != -1) {
            //接收大监测设备，电量及监测数据
            if (Can1ReadFrame.can_id == 0x19) {
                //发送电量
                if (Can1ReadFrame.data[0] == 80) {
                    BoatData.RobotInfo.BatteryLife = Can1ReadFrame.data[1];
                    //printf("BoatData.RobotInfo.BatteryLife = %d \n", BoatData.RobotInfo.BatteryLife);
                }
                if (Can1ReadFrame.data[0] > 80) {
                    MonitorData[Can1ReadFrame.data[0] - 81].Ishavedata = TRUE;
                    MonitorData[Can1ReadFrame.data[0] - 81].data[0] = Can1ReadFrame.data[1];
                    MonitorData[Can1ReadFrame.data[0] - 81].data[1] = Can1ReadFrame.data[2];
                    MonitorData[Can1ReadFrame.data[0] - 81].data[2] = Can1ReadFrame.data[3];
                    MonitorData[Can1ReadFrame.data[0] - 81].data[3] = Can1ReadFrame.data[4];
                }
            }
            //接收4G模块经纬度，船电量
            if (Can1ReadFrame.can_id == 0x21) {
                if (Can1ReadFrame.can_dlc == 1 && Can1ReadFrame.data[0] == 0xfe) {
                    if (BoatData.Gps.Longitude != 0 && BoatData.Gps.Latitude != 0) {
                        sprintf(g4Longitude, "%lf", BoatData.Gps.Longitude);
                        sprintf(g4Latitude, "%lf", BoatData.Gps.Latitude);
                        send4gLongitude(g4Longitude);
                        send4gLatitude(g4Latitude);
                        send4gBatteryLife();
                    }
                }
            }
            //接收小主控电量
            if (Can1ReadFrame.can_id == 0x23) {
                BoatData.RobotInfo.BatteryLife = Can1ReadFrame.data[0];
                BoatData.MaxCurrent[0] = Can1ReadFrame.data[1];
                BoatData.MaxCurrent[1] = Can1ReadFrame.data[2];
                BoatData.MaxCurrent[2] = Can1ReadFrame.data[3];
                BoatData.MaxCurrent[3] = Can1ReadFrame.data[4];
                BoatData.LeakCheck = Can1ReadFrame.data[5];
            }
            //采样结束
            if (Can1ReadFrame.can_id == 0x24 && Can1ReadFrame.can_dlc == 1) {
               BoatData.SendSampleOver = TRUE;
            }
        }
    }
    pthread_exit(NULL);
}

void CreateReadCan2Thread() {
    int ret;

    ret = pthread_create(&ReadCan2Handle, NULL, (void *) ReadCan2Thread, NULL);
    if (ret != 0) {
        printf("CreateReadCan2Thread error!\n");
        exit(1);
    } else {
        printf("CreateReadCan2Thread Success!\n");
    }
}

void send4gLongitude(char data[]) {
    UINT8 i;
    CanLongitude.can_id = 0x21;
    CanLongitude.can_dlc = 8;
    CanLongitude.data[0] = 1;
    for (i = 0; i < 7; i++) {
        CanLongitude.data[i + 1] = data[i];
    }
    Can1Write(&CanLongitude);
    usleep(200);

    CanLongitude.data[0] = 2;
    for (i = 0; i < 7; i++) {
        CanLongitude.data[i + 1] = data[i + 7];
    }
    Can1Write(&CanLongitude);
    usleep(200);

}

void send4gLatitude(char data[]) {
    UINT8 i;
    CanLatitude.can_id = 0x21;
    CanLatitude.can_dlc = 8;
    CanLatitude.data[0] = 3;
    for (i = 0; i < 7; i++) {
        CanLatitude.data[i + 1] = data[i];
    }
    Can1Write(&CanLatitude);
    usleep(500);

    CanLatitude.data[0] = 4;
    for (i = 0; i < 7; i++) {
        CanLatitude.data[i + 1] = data[i + 7];
    }
    Can1Write(&CanLatitude);
    usleep(500);
}

void send4gBatteryLife() {
    CanBattery.can_id = 0x21;
    CanBattery.can_dlc = 1;
    CanBattery.data[0] = BoatData.RobotInfo.BatteryLife;
    Can1Write(&CanBattery);
    usleep(500);
}