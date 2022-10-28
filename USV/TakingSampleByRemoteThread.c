/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TakingSampleByRemoteThread.c
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016.11.28, 下午2:24 
 */
#include "AllPthreadCreate.h"

MANUALSAMPLINGCONTROL ManualSamplingControl;
struct can_frame ManualSamplingData;

void * TakingSampleByRemoteThread ( ) {
    while (1) {
        //sleep ( 1 );
        if(BoatData.StartSample == FALSE)
        {
            BoatData.StartSample = TRUE;
            TakingManualSampling(ManualSamplingControl.Volume, ManualSamplingControl.BottleNumber, ManualSamplingControl.Depth);
        }
        sleep ( 1 );
        if(BoatData.SendSampleOver == TRUE)
        {
            SendManualSamplingFinished(ManualSamplingControl.BottleNumber);
            BoatData.SendSampleOver = FALSE;
            BoatData.StartSample = FALSE;
            break;
        }  
    }
    pthread_exit(NULL);
}

void CreateTakingSampleByRemoteThread ( ) {
    int ret;

    ret = pthread_create(&TakingSampleByRemote, NULL, (void *) TakingSampleByRemoteThread, NULL);
    if (ret != 0) {
        printf("CreateTakingSampleByRemoteThread error!\n");
        exit(1);
    } else {
        printf("CreateTakingSampleByRemoteThread Success!\n");
    }
}

//data[0]: SamplingRod    Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[1]: SamplingPump   Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[2]: Alarm          Open: 17     Close: 33  NULL: 0   
//data[3]: Value          BottleNumber            NULL: 0
//data[4]: MonitorPump    Open: 17     Close: 33  NULL: 0 

void TakingManualSampling ( UINT8 Volume, UINT8 bottle, UINT8 depth ) {
    ManualSamplingControl.IsSampling = TRUE;
    ManualSamplingControl.SamplingIsAck = FALSE;
    //step 1
    StopBoat(); //stop suv
    usleep(2000);
    
    ManualSamplingData.can_id = 0x26;
    ManualSamplingData.can_dlc = 3;
    ManualSamplingData.data[0] = bottle;   //sampling open
    ManualSamplingData.data[1] = depth;    //sampling open
    ManualSamplingData.data[2] = Volume;   //sampling open
    Can1Write(&ManualSamplingData);
/*
    if (depth == 0) {

    } else {
        ManualSamplingData.can_id = 0x18;
        ManualSamplingData.can_dlc = 1;
        ManualSamplingData.data[0] = 17;//sampling open
        Can1Write(&ManualSamplingData);
        //usleep(200);
        usleep(800000 * depth);
    }

    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 1;
    ManualSamplingData.data[0] = 33;   //sampling open
    Can1Write(&ManualSamplingData);
    usleep(200);

    //打开阀门
    ManualSamplingData.can_id = 0x20;
    ManualSamplingData.can_dlc = 2;
    ManualSamplingData.data[0] = 10;
    ManualSamplingData.data[1] = 220 + bottle;
    Can1Write(&ManualSamplingData);

    sleep(7);

    //step 2
    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 3;
    ManualSamplingData.data[0] = 0;
    ManualSamplingData.data[1] = 17;  //sampling pump open
    ManualSamplingData.data[2] = 17;  //alarm open
    Can1Write(&ManualSamplingData);
    usleep((Volume * 3500 + 3000) * 1000);

    //step 3
    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 3;
    ManualSamplingData.data[0] = 65;  //收采样杆
    ManualSamplingData.data[1] = 33;  //sampling Pump close 
    ManualSamplingData.data[2] = 33;  //alarm close 
    Can1Write(&ManualSamplingData);
    sleep(7);

    //step 4  反吹瓶子
    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 3;
    ManualSamplingData.data[0] = 0;
    ManualSamplingData.data[1] = 65;  //抽水泵反转
    ManualSamplingData.data[2] = 17;
    Can1Write(&ManualSamplingData);
    sleep(7);

    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 3;
    ManualSamplingData.data[0] = 33;
    ManualSamplingData.data[1] = 33;
    ManualSamplingData.data[2] = 33;
    Can1Write(&ManualSamplingData);
    sleep(1);

    //关闭阀门
    ManualSamplingData.can_id = 0x20;
    ManualSamplingData.can_dlc = 2;
    ManualSamplingData.data[0] = 10;
    ManualSamplingData.data[1] = 100;
    Can1Write(&ManualSamplingData);
*/
    ManualSamplingControl.IsSampling = FALSE;
}

void SendManualSamplingFinished ( UINT8 BottleNumber ) {
    UINT8 TempBuffer[5];
    TempBuffer[0] = 0;
    TempBuffer[1] = 83;
    TempBuffer[2] = 70;
    TempBuffer[3] = BottleNumber;
    TempBuffer[4] = 124;

    serial_write(0, 5, TempBuffer);
    usleep(36000);

    ManualSamplingControl.SamplingIsAck = TRUE;
}