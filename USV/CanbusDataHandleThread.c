/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   CanbusDataHandleThread.c
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016.11.28, 下午2:24
 */
#include "AllPthreadCreate.h"
#include "can0.h"
#include "BoatData.h"

BOATDATA BoatData;

void * CanbusDataHandleThread ( ) {
    UINT8 i;
    UINT32 RetData;
    float tempfloat = 0;
    while (1) {
        // pthread_mutex_lock ( &mutex );
        // printf("CanbusDataHandleThread\n");
        // pthread_mutex_unlock ( &mutex );
        RetData = Can0Read(&Can0ReadFrame);

        if (RetData != -1) {
            switch (Can0ReadFrame.can_id) {
                case 0x10:
                {
                    for (i = 0; i < Can0ReadFrame.can_dlc; i++) {
                        BoatData.Gps.ByteLongitude[i] = Can0ReadFrame.data[i];
                    }

                    BoatData.Gps.Longitude = BytesToDouble(BoatData.Gps.ByteLongitude);


                    if (BoatData.Gps.Longitude == 0) {
                        BoatData.RobotInfo.ErrorMessage |= GPS_No_Signal;
                    }

                    BoatData.RobotInfo.ErrorMessage &= ~GPS_No_Signal;
                }
                    break;

                case 0x11:
                {
                    for (i = 0; i < Can0ReadFrame.can_dlc; i++) {
                        BoatData.Gps.ByteLatitude[i] = Can0ReadFrame.data[i];
                    }

                    BoatData.Gps.Latitude = BytesToDouble(BoatData.Gps.ByteLatitude);

                    if (BoatData.Gps.Latitude == 0) {
                        BoatData.RobotInfo.ErrorMessage |= GPS_No_Signal;
                    }

                    BoatData.RobotInfo.ErrorMessage &= ~GPS_No_Signal;
                }
                    break;

                case 0x12:
                {
                    for (i = 0; i < Can0ReadFrame.can_dlc; i++) {
                        BoatData.Gps.ByteSpeed[i] = Can0ReadFrame.data[i];
                    }

                    BoatData.Gps.Speed = (FLOAT) (((BoatData.Gps.ByteSpeed[0] * 256 + BoatData.Gps.ByteSpeed[1]) * 1.0f)); //10.0F

                }
                    break;

                case 0x13:
                {
                    BoatData.USVTime.Hour = Can0ReadFrame.data[0];
                    BoatData.USVTime.Minute = Can0ReadFrame.data[1];
                    BoatData.USVTime.Second = Can0ReadFrame.data[2];
                    BoatData.USVDate.Year = Can0ReadFrame.data[3];
                    BoatData.USVDate.Month = Can0ReadFrame.data[4];
                    BoatData.USVDate.Day = Can0ReadFrame.data[5];
                }
                    break;

                case 0x14:
                {
                    for (i = 0; i < 2; i++) {
                        BoatData.Tilt.ByteYaw[i] = Can0ReadFrame.data[0 + i];
                        BoatData.Tilt.Bytepitch[i] = Can0ReadFrame.data[2 + i];
                        BoatData.Tilt.Byteroll[i] = Can0ReadFrame.data[4 + i];
                    }

                    //handler yaw data;
                    tempfloat = (FLOAT) (((BoatData.Tilt.ByteYaw[0] * 256 + BoatData.Tilt.ByteYaw[1]) / 10.0f));
                    if (tempfloat <= 180) {
                        BoatData.Tilt.yaw = tempfloat;

                    } else if (tempfloat < 360) {
                        BoatData.Tilt.yaw = tempfloat - 360.0f;
                    } else {
                        BoatData.RobotInfo.ErrorMessage |= Compass_Error;
                    }
                    BoatData.RobotInfo.ErrorMessage &= (~Compass_Error);

                    //handler pitch data;
                    tempfloat = (FLOAT) (((BoatData.Tilt.Bytepitch[0] * 256 + BoatData.Tilt.Bytepitch[1]) / 10.0f));
                    if ((tempfloat <= 180) && (tempfloat >= -180)) {
                        BoatData.Tilt.pitch = tempfloat;
                        BoatData.RobotInfo.ErrorMessage &= ~Tilt_Error;
                    } else {
                        BoatData.Tilt.pitch = 0;
                        BoatData.RobotInfo.ErrorMessage |= Tilt_Error;
                    }

                    //handler roll data;
                    tempfloat = (FLOAT) (((BoatData.Tilt.Byteroll[0] * 256 + BoatData.Tilt.Byteroll[1]) / 10.0f));
                    if ((tempfloat <= 180) && (tempfloat >= -180)) {
                        BoatData.Tilt.roll = tempfloat;
                        BoatData.RobotInfo.ErrorMessage &= ~Tilt_Error;
                    } else {
                        BoatData.Tilt.roll = 0;
                        BoatData.RobotInfo.ErrorMessage |= Tilt_Error;
                    }
                }
                    break;

                default:
                    break;
            }

            /*
            printf( "length = %d,  canid = %x\n", Can0ReadFrame.can_dlc, Can0ReadFrame.can_id );
            for ( i = 0 ; i < Can0ReadFrame.can_dlc ; i++ ) {
                printf( "candata[%d] = 0x%x\n", i, Can0ReadFrame.data[ i ] );
            }
             */
        } else {
            printf("GPSMODE CAN RECIVER FAILER\n");
        }

        usleep(100);

    }
    pthread_exit(NULL);
}

void CreateCanbusDataHandleThread ( ) {
    int ret;

    ret = pthread_create(&CanbusDataHandle, NULL, (void *) CanbusDataHandleThread, NULL);
    if (ret != 0) {
        printf("Create CanbusDataHandleThread error!\n");
        exit(1);
    } else {
        printf("Create CanbusDataHandleThread success!\n");
    }
}

/**
 * @brief  convert byte array to double
 *
 * @param  name of the byte array
 * @return a double data
 *
 */
double BytesToDouble ( UINT8 data[] ) {
    double a = 0;
    UINT8 ii;
    void *pf;
    BYTE* px = data;
    pf = &a;
    for (ii = 0; ii < 8; ii++) {
        *((BYTE *) pf + ii) = *(px + ii);
    }
    return a;
}

//~ Formatted by Jindent --- http://www.jindent.com
