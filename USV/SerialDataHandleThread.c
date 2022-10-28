/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
 * File:   SerialDataHandleThread.c
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016.11.28, 下午2:24
 */
#include "AllPthreadCreate.h"
#include "BoatData.h"
#include <pthread.h>
#include "can1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AutoPilotData.h"
#include "TakingSampleByRemoteThread.h"
#include "SerialPort.h"
#include "SamplingMonitorMode.h"
#include "MonitorData.h"
#include "WashBottleThread.h"

AUTOPILOTDATA *AutoP = NULL;
UINT8 ReciveAutoPointCount = 0;
AUTOPILOTDATA Temp;
REMOTEMANUAL RemoteManual;
UINT8 SendMessageCount = 0;
UINT16 CheckSum = 0;
extern UINT8 USVSpeed;
int err;

void * SerialDataHandleThread() {
    UINT32 i = 0,
            value = -1;
    UINT8 serialbuffer[ 256 * 59 ];
    void *tret;

    while (1) {
        printf("SerialDataHandleThread( )\n");

        pthread_mutex_lock(&mutex);
        value = serial_read(serialbuffer);
        pthread_mutex_unlock(&mutex);
        RFReviceDateHandlerInit(value, serialbuffer);

        for (i = 0; i < value; i++) {
            printf("serialbuffer = serialbuffer[%d] = %d\n", i, serialbuffer[i]);
        }

        switch (BoatData.RFReciverMsg) {
            case USVWaypointTask:
            {

                if (value < 5) {
                    BoatData.RobotInfo.Status = 65530;
                    USVNavigatorData.NoSend = TRUE;
                    break;
                }

                UINT16 actualsum = (UINT16) (serialbuffer[value - 4] + (serialbuffer[value - 5]) * 256);
                UINT16 checksum = Checksum(serialbuffer, 0, value - 1);

                printf("actualsum = %d, checksum = %d\n ", actualsum, checksum);
                if (actualsum == checksum) {
                    UINT8 AutoPointNumber = serialbuffer[4];
                    for (i = 0; i < value; i++) {
                        printf("USVWaypointTask[%d] = %d\n", i, serialbuffer[i]);
                    }

                    CreatAutoPointBuf(&AutoP, AutoPointNumber); //malloc buffer size;

                    for (i = 4; i < value - 6;) {
                        if (serialbuffer[ i + 1] == 119) //Path point 
                        {
                            USVPathPointSave(&AutoP, serialbuffer, i + 2, i + 10, &ReciveAutoPointCount, &Temp);
                            i += 17;
                        } else if (serialbuffer[ i + 1] == 115) //Sample point 
                        {
                            USVSamplePointSave(&AutoP, serialbuffer, i + 2, i + 10, i + 18, &ReciveAutoPointCount, &Temp);
                            i += 22;
                        } else if (serialbuffer[ i + 1] == 109) //Moniter point
                        {
                            USVMonitorPointSave(&AutoP, serialbuffer, i + 2, i + 10, i + 18, &ReciveAutoPointCount, &Temp);
                            i += 20;
                        }
                    }

                    USVNavigatorData.NavigatorPointNumber = ReciveAutoPointCount;

                    ReciveAutoPointCount = 0; //clear 0;

                    for (i = 0; i < AutoPointNumber; i++) {
                        printf(" autop_lat[%d] = %f\n autop_lng[%d] = %f\n autop_TaskType = %d\n autop_SampleBottleNumber = %d\n\
                         autop_SampleDepth = %d\n autop_SampleTask = %d\n autop_SampleVolume = %d\n\
                         autop_SampleWashFactor = %d\n autop_MonitorDepth = %d autop_MonitorPeriod = %d\n\
                         autop_MonitorPoint = %d\n", i, AutoP[i].Latitude, i, AutoP[i].Longitude, AutoP[i].TaskType, AutoP[i].SampleBottleNumber,\
                         AutoP[i].SampleDepth, AutoP[i].SampleTask, AutoP[i].SampleVolume, AutoP[i].SampleWashFactor, AutoP[i].MonitorDepth, \
                         AutoP[i].MonitorPeriod, AutoP[i].MonitorPoint);
                    }

                    CourseReversalSet.IsCourseReversal = TRUE; //开始和取消任务标志位,如果任务取消不能返航;
                    CourseReversalSet.IsGoHome = FALSE;
                    CourseReversalSet.GoHomePreventRepeatedlyMinus = FALSE;
                    CourseReversalSet.PointMaxToMin = FALSE;
                    CreateAutoPilotThread();

                    USVNavigatorData.NoSend = TRUE;
                } else {
                    BoatData.RobotInfo.Status = 65530; //Reciver Failer;

                    USVNavigatorData.NoSend = TRUE;
                }

            }
                break;

            case USVNoSendData:
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 193) {
                    USVNavigatorData.NoSend = FALSE;
                }
            }
                break;

            case USVRemoteControllerTask:
            {
                if (value < 12 || value > 20) //Debug
                {
                    break;
                }
                UINT16 actualsum = (UINT16) (serialbuffer[value - 3] + (serialbuffer[value - 4]) * 256);
                CheckSum = Checksum(serialbuffer, 4, value);
                if (actualsum == CheckSum) {
                    BoatData.RobotInfo.CurrentMode = ManualRF;

                    UINT8 templength = value - 8;
                    UINT8 loop = templength / 2;
                    UINT8 startindex = 4, Canindex = 0;

                    for (i = 0; i < loop; i++) {
                        switch (serialbuffer[startindex]) {
                            case 0: //servo;
                            {
                                //Can1WriteFrame.data[Canindex] = tempbuffer[startindex + 1];
                                RemoteManual.MotorServo = serialbuffer[startindex + 1 ];
                                startindex += 2;
                                Canindex += 1;
                            }
                                break;
                            case 1: //propeller
                            {
                                //Can1WriteFrame.data[Canindex] = tempbuffer[startindex + 1];
                                RemoteManual.MotorPropeller = serialbuffer[startindex + 1 ];
                                startindex += 2;
                                Canindex += 1;
                            }
                                break;
                            case 2: //Alarm 0X15 Send data to Monitor Device;
                            {
                                Can1WriteFrame.data[Canindex] = serialbuffer[startindex + 1 ];

                                Can1WriteFrame.can_id = 0x23;
                                Can1WriteFrame.can_dlc = 3;
                                Can1WriteFrame.data[0] = Can1WriteFrame.data[Canindex];
                                Can1WriteFrame.data[1] = 0;
                                Can1WriteFrame.data[2] = 0;
                                Can1Write(&Can1WriteFrame);

                                startindex += 2;
                                Canindex += 1;
                            }
                                break;
                            case 3: //Alarm lamp
                            {
                                Can1WriteFrame.data[Canindex] = serialbuffer[startindex + 1 ];
                                if (ManualSamplingControl.SamplingIsAck == TRUE) {
                                    SendManualSamplingFinished(ManualSamplingControl.BottleNumber);
                                }

                                startindex += 2;
                                Canindex += 1;
                            }
                                break;

                            default:
                                break;
                        }
                    }
                   
                    SendMessageCount++;
                    if (SendMessageCount >= 11) {
                        BoatDataManualRFMessage();
                        SendMessageCount = 0;
                    }

                    //RightLeftMotorCtl(RemoteManual.MotorServo, RemoteManual.MotorPropeller);
                    TrafficBoatMotorCtl(RemoteManual.MotorServo, RemoteManual.MotorPropeller);
                }

            }
                break;

            case USVAutoControllerTask:
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 194) {
                    StopBoat();
                    BoatData.RobotInfo.CurrentMode = Auto;
                    USVNavigatorData.NoSend = TRUE; // WAREING;
                }
            }
                break;

            case USVCourseReversalTask: //返航命令
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 194) {
                    if (CourseReversalSet.IsCourseReversal == TRUE) { //开始任务时,标志返航状态位,如果结束任务不能返航;
                        CourseReversalSet.IsCourseReversal = FALSE;
                        if (CourseReversalSet.NavigatorOver == TRUE) // 此两个状态表示能重新创建导航线程,每个任务只能返航一次;如果自主导航结束,需要重新创建导航线程;
                        {
                            CourseReversalSet.IsGoHome = TRUE;
                            CourseReversalSet.GoHomePreventRepeatedlyMinus = TRUE; //重新创建线程导航计数重最大导航点向最小导航点运行,不需要在减一个导航点;
                            CourseReversalSet.PointMaxToMin = TRUE;
                            CreateAutoPilotThread(); //重新创建线程;
                            printf("serialbuffer[3] == 124 && serialbuffer[4] == 194\n");
                        } else {
                            CourseReversalSet.IsGoHome = TRUE;
                            printf("IsGoHome = TRUE;\n");
                        }
                    }
                }
            }
                break;

            case USVSpeedTask:
            {
                if (serialbuffer[3] == 124) {
                    if (serialbuffer[4] >= 1 && serialbuffer[4] <= 5) {
                        switch (serialbuffer[4]) {
                            case 1:
                                USVSpeed = 187;
                                break;
                            case 2:
                                USVSpeed = 175;
                                break;
                            case 3:
                                USVSpeed = 170;
                                break;
                            case 4:
                                USVSpeed = 165;
                                break;
                            case 5:
                                USVSpeed = 160;
                                break;
                        }
                    } else {
                        USVSpeed = 172;
                    }
                }
            }
                break;

            case USVOpenAlarmTask:
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 0) {
                    ManualSamplingData.can_id = 0x18;
                    ManualSamplingData.can_dlc = 3;
                    ManualSamplingData.data[0] = 0;
                    ManualSamplingData.data[1] = 0; //sampling pump open
                    ManualSamplingData.data[2] = 17; //alarm open
                    Can1Write(&ManualSamplingData);
                }
            }
                break;

            case USVCloseAlarmTask:
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 32) {
                    ManualSamplingData.can_id = 0x18;
                    ManualSamplingData.can_dlc = 3;
                    ManualSamplingData.data[0] = 0;
                    ManualSamplingData.data[1] = 0; //sampling pump open
                    ManualSamplingData.data[2] = 33; //alarm open
                    Can1Write(&ManualSamplingData);
                }
            }
                break;

            case USVCannelTask:
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 198) {
                    StopBoat(); //Stop USV;
                    if (USVNavigatorData.Status == TRUE) {
                        USVNavigatorData.Status = FALSE;
                        //等待线程结束;
                        BoatData.RobotInfo.Status = 65527; //正在取消任务请等待;
                        err = pthread_join(AutoPilot, &tret);
                        if (err != 0) {
                            fprintf(stderr, "errno: %s", strerror(err));
                        }
                        printf("AutoPilotThread is exit tret = %d\n", (int) tret);

                        StopBoat(); //Stop USV;

                        free(AutoP);

                        BoatData.RobotInfo.Status = 65532;

                        CourseReversalSet.IsCourseReversal = FALSE; //如果任务取消不能返航;
                        CourseReversalSet.IsGoHome = FALSE;
                        CourseReversalSet.GoHomePreventRepeatedlyMinus = FALSE;
                        IsSaveDB = FALSE;
                        CourseReversalSet.PointMaxToMin = FALSE;
                        StraightLineNav.IsDeviNav = FALSE; //取消任务后自动取消等分导航任务;
                    }
                }
            }
                break;

            case USVPauseTask:
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 101) {
                    if (USVNavigatorData.Status == TRUE) {
                        USVNavigatorData.Paused = TRUE;
                        BoatData.RobotInfo.Status = 65531;
                    }
                }
            }
                break;

            case USVRecoverTask:
            {
                if (serialbuffer[3] == 124 && serialbuffer[4] == 100) {
                    if (USVNavigatorData.Status == TRUE) {
                        USVNavigatorData.Paused = FALSE;
                    }
                }
            }
                break;

            case USVReadSetting:
            {

            }
                break;

            case USVSettingPar:
            {

            }
                break;

            case USVManualSamping:
            {
                if (BoatData.RobotInfo.CurrentMode == ManualRF && WashModeRun == TRUE) //samlu 未写完
                {
                    WashClean.can_id = 0x18;
                    WashClean.can_dlc = 3;
                    WashClean.data[0] = 0;
                    WashClean.data[1] = 33; //sampling pump close
                    WashClean.data[2] = 33; //alarm close 
                    Can1Write(&WashClean);
                    sleep(3);

                    WashClean.can_id = 0x20; //阀回中位;
                    WashClean.can_dlc = 2;
                    WashClean.data[0] = 10;
                    WashClean.data[1] = 100;
                    Can1Write(&WashClean);
                    sleep(3);

                    //杀死清洗模式线程;
                    pthread_cancel(WashMode);
                    printf("Pthread_canncel is kill\n");
                    WashModeRun = FALSE;
                }

                if (BoatData.RobotInfo.CurrentMode == ManualRF && ManualSamplingControl.IsSampling == FALSE) {
                    printf("USVManualSamping( )\n");
                    ManualSamplingControl.BottleNumber = serialbuffer[4];
                    ManualSamplingControl.Depth = serialbuffer[5];
                    ManualSamplingControl.Volume = serialbuffer[7];
                    ManualSamplingControl.WashFactor = serialbuffer[8];
                    CreateTakingSampleByRemoteThread();
                }
            }
                break;
            case USVIsDB:
            {
                if (serialbuffer[3] == 0x15) {
                    //IsSaveDB = FALSE; //数据库保存完毕应答机制;
                }
            }
                break;
            case USVWashMode:
            {
                if (BoatData.RobotInfo.CurrentMode == ManualRF && ManualSamplingControl.IsSampling == FALSE) {
                    WashVolume = serialbuffer[4];
                    CreateWashBottleThread();
                }
            }
                break;
            case USVConstantSpeedSet:
            {
                if(serialbuffer[3] >= 1 && serialbuffer[3] <= 80)
                {
                    if(serialbuffer[3] >= 1 && serialbuffer[3] <= 7)
                    {
                        //响应速度 
                        //定速速度小响应时间慢;
                        ConstantSpeed.ResponseSpeed = 250;
                        //调低导航速度;
                        USVSpeed = 168;
                    }
                    else 
                    {
                        //响应速度
                        //定速时间快响应时间快;
                        ConstantSpeed.ResponseSpeed = 200;
                        //稍微调快导航速度;
                    }
                    ConstantSpeed.Speed = serialbuffer[3];  //定速巡航参数赋值
                    ConstantSpeed.Enable = TRUE;    
                }
            }
            break;
            case USVCannelConstantSpeedSet:                 //取消定速巡航功能
            {
                if (serialbuffer[3] == 0x84) {
                    ConstantSpeed.Enable = FALSE;
                }
            }
            break;

            default:
                break;
        }

/*
            for ( i = 0; i < value; i++ ){
                printf ( "value = %d, serial read buffer[%d] = 0x%x\n", value, i, serialbuffer[ i ] );
            }
*/
        usleep(20);
    }
    pthread_exit(NULL);
}

void CreateSerialDataHandleThread(void) {
    int ret;

    ret = pthread_create(&SerialDateHandle, NULL, (void *) SerialDataHandleThread, NULL);
    if (ret != 0) {
        printf("Create SerialDateHandle pthread error!\n");
        exit(1);
    } else {
        printf("Create SerialDateHandle pthread success!\n");
    }
}

void RFReviceDateHandlerInit(UINT32 ReciverData, UINT8 Rebuffer[]) {
    if (ReciverData != 0) {
        if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x01) {
            BoatData.RFReciverMsg = USVWaypointTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x00) {
            BoatData.RFReciverMsg = USVRemoteControllerTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x02) {
            BoatData.RFReciverMsg = USVNoSendData;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x03) {
            BoatData.RFReciverMsg = USVAutoControllerTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x63 && Rebuffer[2] == 0x63) {
            BoatData.RFReciverMsg = USVCourseReversalTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x73 && Rebuffer[2] == 0x73) {
            BoatData.RFReciverMsg = USVSpeedTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x41) {
            BoatData.RFReciverMsg = USVOpenAlarmTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x61) {
            BoatData.RFReciverMsg = USVCloseAlarmTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x07) {
            BoatData.RFReciverMsg = USVCannelTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x53) {
            BoatData.RFReciverMsg = USVPauseTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x43 && Rebuffer[2] == 0x52) {
            BoatData.RFReciverMsg = USVRecoverTask;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x93 && Rebuffer[2] == 0x6E) {
            BoatData.RFReciverMsg = USVReadSetting;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x93 && Rebuffer[2] == 0x4E) {
            BoatData.RFReciverMsg = USVSettingPar;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x53 && Rebuffer[2] == 0x53) {
            BoatData.RFReciverMsg = USVManualSamping;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x13 && Rebuffer[2] == 0x14) {
            BoatData.RFReciverMsg = USVIsDB;
        } else if (Rebuffer[0] == 0x00 && Rebuffer[1] == 0x59 && Rebuffer[2] == 0x5a) {
            BoatData.RFReciverMsg = USVWashMode;
        } else if(Rebuffer[0] == 0x00 && Rebuffer[1] == 0x83 && Rebuffer[2] == 0x83)
        {
            BoatData.RFReciverMsg = USVConstantSpeedSet;
        } else if(Rebuffer[0] == 0x00 && Rebuffer[1] == 0x84 && Rebuffer[2] == 0x84)
        {
            BoatData.RFReciverMsg = USVCannelConstantSpeedSet;
        }else {
            if (USVNavigatorData.NoSend == FALSE) {
                BoatData.RobotInfo.Status = 65530;
                USVNavigatorData.NoSend = TRUE;
                printf("USVNavigatorData.NoSend == FALSE \n");
            }
            BoatData.RFReciverMsg = USVNoData;
        }
    } else {
        BoatData.RFReciverMsg = USVNoData;
    }
}

UINT16 Checksum(UINT8 data[], UINT32 start, UINT32 length) {
    UINT16 sum = 0;
    UINT32 i;
    for (i = start; i < length - 4; i++) {
        sum += data[i];
    }
    return sum;
}

void BoatDataAutoMessage() {
    UINT8 TempBuffer[110], i, Datacount;

    if (BoatData.RobotInfo.CurrentMode == Auto) {
        //Message Header
        TempBuffer[0] = 0;
        TempBuffer[1] = 66;
        TempBuffer[2] = 1;
        TempBuffer[3] = 124;
        //1, GPS Latitude, 8Bytes
        TempBuffer[4] = 1;
        for (i = 5; i < 5 + 8; i++) {
            TempBuffer[i] = BoatData.Gps.ByteLatitude[i - 5];
        }
        //2, GPS Longitude, 8Bytes
        TempBuffer[13] = 2;
        for (i = 14; i < 14 + 8; i++) {
            TempBuffer[i] = BoatData.Gps.ByteLongitude[i - 14];
        }
        //3, Speed, 2Byte
        TempBuffer[22] = 3;
        TempBuffer[23] = BoatData.Gps.ByteSpeed[0];
        TempBuffer[24] = BoatData.Gps.ByteSpeed[1];

        //4, GPS TIME
        TempBuffer[25] = 4;
        TempBuffer[26] = BoatData.USVTime.Hour;
        TempBuffer[27] = BoatData.USVTime.Minute;
        TempBuffer[28] = BoatData.USVTime.Second;
        //5, 
        //GPS Date, 3Byte
        TempBuffer[29] = 5;
        TempBuffer[30] = BoatData.USVDate.Year;
        TempBuffer[31] = BoatData.USVDate.Month;
        TempBuffer[32] = BoatData.USVDate.Day;

        //6,电池电量, 1Byte
        TempBuffer[33] = 6;
        TempBuffer[34] = BoatData.RobotInfo.BatteryLife;

        //pitch, 2Byte
        TempBuffer[35] = 10;
        TempBuffer[36] = BoatData.Tilt.Bytepitch[0];
        TempBuffer[37] = BoatData.Tilt.Bytepitch[1];

        //roll, 2Byte
        TempBuffer[38] = 11;
        TempBuffer[39] = BoatData.Tilt.Byteroll[0];
        TempBuffer[40] = BoatData.Tilt.Byteroll[1];

        //yaw, 2Byte
        TempBuffer[41] = 12;
        TempBuffer[42] = BoatData.Tilt.ByteYaw[0];
        TempBuffer[43] = BoatData.Tilt.ByteYaw[1];

        //14, Mode, 1Byte
        TempBuffer[44] = 14;
        TempBuffer[45] = 77;

        //15, Current Status;  
        TempBuffer[46] = 15;
        TempBuffer[47] = (UINT8) (BoatData.RobotInfo.Status / 256);
        TempBuffer[48] = (UINT8) (BoatData.RobotInfo.Status % 256);

        //16, Server ID;
        //17, UltraSonic;
        TempBuffer[49] = 17;
        TempBuffer[50] = BoatData.UltraSonic.Sensor1Distance / 256;
        TempBuffer[51] = BoatData.UltraSonic.Sensor1Distance % 256;

        TempBuffer[52] = 18;
        TempBuffer[53] = BoatData.UltraSonic.Sensor2Distance / 256;
        TempBuffer[54] = BoatData.UltraSonic.Sensor2Distance % 256;

        TempBuffer[55] = 19;
        TempBuffer[56] = BoatData.RobotInfo.Temperature + 40;

        TempBuffer[57] = 20;
        TempBuffer[58] = BoatData.RobotInfo.ErrorMessage;

        TempBuffer[59] = 22;
        TempBuffer[60] = (UINT8) (BoatData.RobotInfo.MissionID / 256);
        TempBuffer[61] = (UINT8) (BoatData.RobotInfo.MissionID % 256);

        TempBuffer[62] = 43;
        TempBuffer[63] = BoatData.SamplingWorkStatus.CurrentBottleNumber;

        Datacount = 63;
        for (i = 0; i < 10; i++) {
            if (MonitorData[i].Ishavedata == TRUE) {
                TempBuffer[Datacount + 1] = 81 + i;
                TempBuffer[Datacount + 2] = MonitorData[i].data[0];
                TempBuffer[Datacount + 3] = MonitorData[i].data[1];
                TempBuffer[Datacount + 4] = MonitorData[i].data[2];
                TempBuffer[Datacount + 5] = MonitorData[i].data[3];
                Datacount += 5;
            }
        }
        if (IsSaveDB == TRUE) {
            //开始写数据库
            Datacount++;
            TempBuffer[Datacount] = 254;
            Datacount++;
            TempBuffer[Datacount] = 0;
        } else {
            //写数据库完成
            Datacount++;
            TempBuffer[Datacount] = 253;
            Datacount++;
            TempBuffer[Datacount] = 0;
        }

        Datacount++;

        TempBuffer[Datacount] = 35; //#

        Datacount++;
        serial_write(0, Datacount, TempBuffer);

        printf("BoatData.RobotInfo.CurrentMode == AUTO \n");
    }
}

void BoatDataManualRFMessage() {
    UINT8 TempBuffer[40], i;

    if (BoatData.RobotInfo.CurrentMode == ManualRF) {
        //Message Header
        TempBuffer[0] = 0;
        TempBuffer[1] = 66;
        TempBuffer[2] = 52;
        TempBuffer[3] = 124;
        //1, GPS Latitude, 8Bytes
        TempBuffer[4] = 1;
        for (i = 5; i < 5 + 8; i++) {
            TempBuffer[i] = BoatData.Gps.ByteLatitude[i - 5];
        }
        //2, GPS Longitude, 8Bytes
        TempBuffer[13] = 2;
        for (i = 14; i < 14 + 8; i++) {
            TempBuffer[i] = BoatData.Gps.ByteLongitude[i - 14];
        }
        //3, Speed, 2Byte
        TempBuffer[22] = 3;
        TempBuffer[23] = BoatData.Gps.ByteSpeed[0];
        TempBuffer[24] = BoatData.Gps.ByteSpeed[1];

        //6,电池电量, 1Byte
        TempBuffer[25] = 6;
        TempBuffer[26] = BoatData.RobotInfo.BatteryLife;

        //yaw, 2Byte 
        TempBuffer[27] = 12;
        TempBuffer[28] = BoatData.Tilt.ByteYaw[0];
        TempBuffer[29] = BoatData.Tilt.ByteYaw[1];

        //25,工作电流 20170912
        TempBuffer[30] = 25;
        TempBuffer[31] = BoatData.MaxCurrent[0];
        TempBuffer[32] = BoatData.MaxCurrent[1];
        TempBuffer[33] = BoatData.MaxCurrent[2];
        TempBuffer[34] = BoatData.MaxCurrent[3];
        
        //26，漏水检测
        TempBuffer[35] = 26;
        TempBuffer[36] = BoatData.LeakCheck;
        
        //Terminate this Message
        TempBuffer[37] = 35; //#

        serial_write(0, 38, TempBuffer);

        printf("BoatData.RobotInfo.CurrentMode == ManualRF\n");
    }
}




