/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016.11.28, 下午2:24
 */
#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Arm.h"
#include "SerialPort.h"
#include <unistd.h>
#include "AllPthreadCreate.h"
#include "CanbusDataHandleThread.h"
#include "can0.h"
#include "BoatData.h"
#include "can1.h"
#include "TakingSampleByRemoteThread.h"
#include "SerialDataHandleThread.h"
#include "SerialPort4.h"
#include "SerialPort2.h"

/*
 *主函数,初始化CANBus, Serial, 返航初始化，线程
 */

int main(int argc, char ** argv) {

    Can0_init();

    Can1_init();

    serial_init();

    Uart2_init();

    Uart4_init();
    
    CourseReversalSetinit();

    BoatData.USVSettingData.RudderMiddle = 156;

    ManualSamplingControl.IsSampling = FALSE;
    ManualSamplingControl.IsMonitor = FALSE;
    ManualSamplingControl.SamplingIsAck = FALSE;
    ConstantSpeed.ResponseSpeed = 1000;    //定速续航响应时间
    BoatData.RobotInfo.CurrentMode = Auto; //自主导航还要解决，开机不受控制问题,可以增加;

    CreateSerialCanbusPthread();
    
    while (1) {
        BoatDataAutoMessage();
        usleep(800000);   
    }
    return ( EXIT_SUCCESS);
}


//~ Formatted by Jindent --- http://www.jindent.com
