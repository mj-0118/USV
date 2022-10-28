/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AllPthreadCreate.h"
#include "BoatData.h"

UINT16 UltrasonicData1[3], UltrasonicData2[3];
UINT16 Uart4_Value, Uart2_Value;

void * UltrasonicRecive ( ) {
    while (1) {
        //pthread_mutex_lock(&mutex);
        Uart4_Value = Uart4_Ultrasonic_read();
        if (Uart4_Value == 0) {
            Uart4_Value = 8000;
        }
        if (Uart4_Value != 0xFFFF) {
            UltrasonicData1[2] = UltrasonicData1[1];
            UltrasonicData1[1] = UltrasonicData1[0];
            UltrasonicData1[0] = Uart4_Value;
            BoatData.UltraSonic.Sensor1Distance = (UltrasonicData1[0] + UltrasonicData1[1] + UltrasonicData1[2]) / 30;
        } else {
            printf("Ultrasionc uart4 READ ERROR DATA\n");
        }
       // usleep(91000);
        Uart2_Value = Uart2_Ultrasonic_read();
        if (Uart2_Value == 0) {
            Uart2_Value = 8000;
        }
        if (Uart2_Value != 0xFFFF) {
            UltrasonicData2[2] = UltrasonicData2[1];
            UltrasonicData2[1] = UltrasonicData2[0];
            UltrasonicData2[0] = Uart2_Value; 
            BoatData.UltraSonic.Sensor2Distance = (UltrasonicData2[0] + UltrasonicData2[1] + UltrasonicData2[2]) / 30;
        } else {
            printf("Ultrasionc uart2 READ ERROR DATA\n");
        }
        //延时0.1ms
       // pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    pthread_exit(NULL);
}

void CreateUltrasonicReciveThread ( ) {
    int ret;

    ret = pthread_create(&UltrasonicReciveHandle, NULL, (void *) UltrasonicRecive, NULL);
    if (ret != 0) {
        printf("CreateUltrasonicReciveThread error!\n");
        exit(1);
    } else {
        printf("CreateUltrasonicReciveThread Success!\n");
    }
}