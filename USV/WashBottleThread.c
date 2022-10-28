/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "WashBottleThread.h"

UINT8 WashVolume = 1;
BOOL  WashModeRun = FALSE;

void * WashBottleThread ( ) {
    UINT8 i;
    WashModeRun = TRUE;
    while (1) {
        for(i = 1; i < 17; i ++)
        {
           WashBottleHandle ( WashVolume, i); 
        }
        break;
    }
    WashModeRun = FALSE;
    pthread_exit(NULL);
}

void CreateWashBottleThread ( ) {
    int ret;

    ret = pthread_create(&WashMode, NULL, (void *) WashBottleThread, NULL);
    if (ret != 0) {
        printf("CreateWashBottleThread error!\n");
        exit(1);
    } else {
        printf("CreateWashBottleThread Success!\n");
    }
}

void WashBottleHandle ( UINT8 Volume, UINT8 BottleNumber) {
    UINT8 i;
    if (ManualSamplingControl.IsSampling == FALSE) {
        //1号瓶
        WashClean.can_id = 0x20;
        WashClean.can_dlc = 2;
        WashClean.data[0] = 10;
        WashClean.data[1] = 220 + BottleNumber;
        Can1Write(&WashClean);
        sleep(7);
        //step 2
        WashClean.can_id = 0x18;
        WashClean.can_dlc = 3;
        WashClean.data[0] = 0;
        WashClean.data[1] = 17; //sampling pump open
        WashClean.data[2] = 17; //alarm open
        Can1Write(&WashClean);
        usleep((Volume * 3500 + 3000) * 1000);
        //关闭泵 警灯;
        WashClean.can_id = 0x18;
        WashClean.can_dlc = 2;
        WashClean.data[0] = 0;
        WashClean.data[1] = 33; //sampling pump close
        Can1Write(&WashClean);
        sleep(8);
        
        //反吹
        WashClean.can_id = 0x18;
        WashClean.can_dlc = 2;
        WashClean.data[0] = 0;
        WashClean.data[1] = 65; //sampling pump close 
        Can1Write(&WashClean);
        sleep(10);
        
        //关泵
        WashClean.can_id = 0x18;
        WashClean.can_dlc = 3;
        WashClean.data[0] = 0;
        WashClean.data[1] = 33; //sampling pump close
        WashClean.data[2] = 33; //alarm close 
        Can1Write(&WashClean);
        sleep(5);
        
        WashClean.can_id = 0x20;
        WashClean.can_dlc = 2;
        WashClean.data[0] = 10;
        WashClean.data[1] = 100;
        Can1Write(&WashClean);
        sleep(7);
        
        for(i = 0; i < 8; i ++)
        {
            WashModeFinished ( BottleNumber );
        }   
    }
}

void WashModeFinished ( UINT8 BottleNumber ) {
    UINT8 TempBuffer[5];
    TempBuffer[0] = 0;
    TempBuffer[1] = 84;
    TempBuffer[2] = 71;
    TempBuffer[3] = BottleNumber;
    TempBuffer[4] = 124;

    serial_write(0, 5, TempBuffer);
    usleep(36000);
}