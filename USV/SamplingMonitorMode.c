/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "SamplingMonitorMode.h"


//data[0]: SamplingRod    Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[1]: SamplingPump   Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[2]: Alarm          Open: 17     Close: 33  NULL: 0   
//data[3]: Value          BottleNumber            NULL: 0
//data[4]: MonitorPump    Open: 17     Close: 33  NULL: 0 

BOOL IsSaveDB = FALSE;

void USVAutoSampling(AUTOPILOTDATA info) {
    ManualSamplingControl.IsSampling = TRUE;
    //step 1
    StopBoat(); //stop suv
    usleep(2000);
    
    ManualSamplingData.can_id = 0x26;
    ManualSamplingData.can_dlc = 3;
    ManualSamplingData.data[0] = info.SampleBottleNumber;   //sampling open
    ManualSamplingData.data[1] = 0;   //sampling open
    ManualSamplingData.data[2] = info.SampleVolume;   //sampling open
    Can1Write(&ManualSamplingData);
    BoatData.SamplingWorkStatus.CurrentBottleNumber = info.SampleBottleNumber;
    IsSaveDB = TRUE;                   //数据库保存命令开始
    while(1)
    {
        sleep(1);
        if(BoatData.SendSampleOver == TRUE)
        {
           BoatData.SendSampleOver = FALSE;
           IsSaveDB = FALSE;                   //数据库保存结束
           break;
        }
    }

/*
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 1;
    USVSamplingMonitorCmd.data[0] = 17; //sampling open
    Can1Write(&USVSamplingMonitorCmd);
    
    usleep(800000 * 2);
    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 1;
    ManualSamplingData.data[0] = 33;    //sampling open
    Can1Write(&ManualSamplingData);
    usleep(200);
    
    //打开阀门
    ManualSamplingData.can_id = 0x20;
    ManualSamplingData.can_dlc = 2;
    ManualSamplingData.data[0] = 10; 
    ManualSamplingData.data[1] = 220 + info.SampleBottleNumber;
    Can1Write(&ManualSamplingData);
    sleep(7);

    BoatData.SamplingWorkStatus.CurrentBottleNumber = info.SampleBottleNumber;
    //step 2
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 3;
    USVSamplingMonitorCmd.data[0] = 0;
    USVSamplingMonitorCmd.data[1] = 17; //sampling pump open
    USVSamplingMonitorCmd.data[2] = 17; //alarm open
    Can1Write(&USVSamplingMonitorCmd);
    usleep((info.SampleVolume * 3500 + 3000) * 1000);
    IsSaveDB = TRUE;                   //数据库保存命令开始
    
    //step 3
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 3;
    USVSamplingMonitorCmd.data[0] = 65;
    USVSamplingMonitorCmd.data[1] = 33;
    USVSamplingMonitorCmd.data[2] = 33;
    Can1Write(&USVSamplingMonitorCmd);
    sleep(7);

    //step 4
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 3;
    USVSamplingMonitorCmd.data[0] = 0;
    USVSamplingMonitorCmd.data[1] = 65;
    USVSamplingMonitorCmd.data[2] = 17;
    Can1Write(&USVSamplingMonitorCmd);
    sleep(7);
    IsSaveDB = FALSE;                   //数据库保存结束
    
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 3;
    USVSamplingMonitorCmd.data[0] = 33;
    USVSamplingMonitorCmd.data[1] = 33;
    USVSamplingMonitorCmd.data[2] = 33;
    Can1Write(&USVSamplingMonitorCmd);
    sleep(1); 
    
    //关闭阀门
    ManualSamplingData.can_id = 0x20;
    ManualSamplingData.can_dlc = 2;
    ManualSamplingData.data[0] = 10; 
    ManualSamplingData.data[1] = 100;
    Can1Write(&ManualSamplingData);
*/

    BoatData.SamplingWorkStatus.CurrentBottleNumber = 255;
    ManualSamplingControl.IsSampling = FALSE;
}



//data[0]: SamplingRod    Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[1]: SamplingPump   Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[2]: Alarm          Open: 17     Close: 33  NULL: 0   
//data[3]: Value          BottleNumber            NULL: 0
//data[4]: MonitorPump    Open: 17     Close: 33  NULL: 0 

void USVAutoMonitor(AUTOPILOTDATA info) {
    //step 1
    StopBoat(); //stop suv
    usleep(2000);
    ManualSamplingControl.IsMonitor = TRUE;
    
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 3;
    USVSamplingMonitorCmd.data[0] = 17; //SamplingRod down
    USVSamplingMonitorCmd.data[1] = 0;
    USVSamplingMonitorCmd.data[2] = 17; //Alarm open
    Can1Write(&USVSamplingMonitorCmd);
    //sleep(7);
    usleep(800000 * 2);
    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 1;
    ManualSamplingData.data[0] = 33;    //sampling open
    Can1Write(&ManualSamplingData);
    usleep(200);

    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 5;
    USVSamplingMonitorCmd.data[0] = 0;
    USVSamplingMonitorCmd.data[1] = 0;
    USVSamplingMonitorCmd.data[2] = 0;
    USVSamplingMonitorCmd.data[3] = 0;
    USVSamplingMonitorCmd.data[4] = 17; //MonitorPump open
    Can1Write(&USVSamplingMonitorCmd);
    usleep(info.MonitorPeriod * 1000000);

    IsSaveDB = TRUE;   //数据库保存命令
    //step 2
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 5;
    USVSamplingMonitorCmd.data[0] = 65; //SamplingRod up
    USVSamplingMonitorCmd.data[1] = 0;
    USVSamplingMonitorCmd.data[2] = 0; //
    USVSamplingMonitorCmd.data[3] = 0;
    USVSamplingMonitorCmd.data[4] = 33; //MonitorPump close
    Can1Write(&USVSamplingMonitorCmd);
    sleep(16);         //确保发送保存数据成功;
    IsSaveDB = FALSE;  //数据库保存结束;

    //step 3
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 3;
    USVSamplingMonitorCmd.data[0] = 33; //SamplingRod up
    USVSamplingMonitorCmd.data[1] = 0;
    USVSamplingMonitorCmd.data[2] = 33; //Alarm close
    Can1Write(&USVSamplingMonitorCmd);
    ManualSamplingControl.IsMonitor = FALSE;
}


//data[0]: SamplingRod    Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[1]: SamplingPump   Forward: 17  Close: 33  Reversal:65  NULL: 0
//data[2]: Alarm          Open: 17     Close: 33  NULL: 0   
//data[3]: Value          BottleNumber            NULL: 0
//data[4]: MonitorPump    Open: 17     Close: 33  NULL: 0 

void USVAutoSamplingMonitor(AUTOPILOTDATA info) {
    ManualSamplingControl.IsSampling = TRUE;
    //step 1
    StopBoat(); //stop suv
    usleep(2000);

    //step 2
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 4;
    USVSamplingMonitorCmd.data[0] = 17; //SamplingRod down
    USVSamplingMonitorCmd.data[1] = 0;
    USVSamplingMonitorCmd.data[2] = 17; //Alarm open
    USVSamplingMonitorCmd.data[3] = 0;
    Can1Write(&USVSamplingMonitorCmd);
    
    usleep(800000 * 2);
    ManualSamplingData.can_id = 0x18;
    ManualSamplingData.can_dlc = 1;
    ManualSamplingData.data[0] = 33;    //sampling open
    Can1Write(&ManualSamplingData);
    usleep(200);
    
    //打开阀门
    ManualSamplingData.can_id = 0x20;
    ManualSamplingData.can_dlc = 2;
    ManualSamplingData.data[0] = 10; 
    ManualSamplingData.data[1] = 220 + info.SampleBottleNumber;
    Can1Write(&ManualSamplingData);
    sleep(7);
    BoatData.SamplingWorkStatus.CurrentBottleNumber = info.SampleBottleNumber;

    //step 3
    USVSamplingMonitorCmd.can_id = 0x18;
    USVSamplingMonitorCmd.can_dlc = 5;
    USVSamplingMonitorCmd.data[0] = 0;
    USVSamplingMonitorCmd.data[1] = 17; //Sampling pump
    USVSamplingMonitorCmd.data[2] = 0;
    USVSamplingMonitorCmd.data[3] = 0;
    USVSamplingMonitorCmd.data[4] = 17; //Monitro pump
    Can1Write(&USVSamplingMonitorCmd);
    sleep(1);

    if ((info.SampleVolume * 3500 + 3000) * 1000 >= info.SampleDepth * 1000000) {
        usleep(info.SampleDepth * 1000000 - 1000000);
        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 5;
        USVSamplingMonitorCmd.data[0] = 0;  //SamplingRod down
        USVSamplingMonitorCmd.data[1] = 0;
        USVSamplingMonitorCmd.data[2] = 0;  //Alarm open
        USVSamplingMonitorCmd.data[3] = 0;
        USVSamplingMonitorCmd.data[4] = 33; //MonitorPump Close
        Can1Write(&USVSamplingMonitorCmd);
        IsSaveDB = TRUE;                    //数据库保存命令
        sleep(1);

        usleep(((info.SampleVolume * 3500 + 3000) * 1000) - info.SampleDepth * 1000000);
        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 2;
        USVSamplingMonitorCmd.data[0] = 65; //SamplingRod up
        USVSamplingMonitorCmd.data[1] = 33;
        Can1Write(&USVSamplingMonitorCmd);
        sleep(7);

        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 2;
        USVSamplingMonitorCmd.data[0] = 0; //SamplingRod up
        USVSamplingMonitorCmd.data[1] = 65;
        Can1Write(&USVSamplingMonitorCmd);
        sleep(7);

        IsSaveDB = FALSE;                    //数据库保存结束
        
        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 3;
        USVSamplingMonitorCmd.data[0] = 33;
        USVSamplingMonitorCmd.data[1] = 33;
        USVSamplingMonitorCmd.data[2] = 33;
        Can1Write(&USVSamplingMonitorCmd);
        sleep(1);
        
        //关闭阀门
        ManualSamplingData.can_id = 0x20;
        ManualSamplingData.can_dlc = 2;
        ManualSamplingData.data[0] = 10; 
        ManualSamplingData.data[1] = 100;
        Can1Write(&ManualSamplingData);        

        

    } else {
        usleep(((info.SampleVolume * 3500 + 3000) * 1000) - 1000000);
        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 2;
        USVSamplingMonitorCmd.data[0] = 0;
        USVSamplingMonitorCmd.data[1] = 33; // Sampling Close
        Can1Write(&USVSamplingMonitorCmd);
        sleep(1);

        usleep(info.SampleDepth * 1000000 - (info.SampleVolume * 3500 + 3000) * 1000);
        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 5;
        USVSamplingMonitorCmd.data[0] = 0;
        USVSamplingMonitorCmd.data[1] = 0;
        USVSamplingMonitorCmd.data[2] = 0;
        USVSamplingMonitorCmd.data[3] = 0;
        USVSamplingMonitorCmd.data[4] = 33; //MonitorPump Close
        Can1Write(&USVSamplingMonitorCmd);
        IsSaveDB = TRUE;                    //数据库保存命令
        sleep(1);

        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 1;
        USVSamplingMonitorCmd.data[0] = 65; //ROD down
        Can1Write(&USVSamplingMonitorCmd);
        sleep(7);

        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 2;
        USVSamplingMonitorCmd.data[0] = 0; //ROD UP
        USVSamplingMonitorCmd.data[1] = 65;
        Can1Write(&USVSamplingMonitorCmd);
        sleep(7);
        
        IsSaveDB = FALSE;                    //数据库保存结束 
        
        USVSamplingMonitorCmd.can_id = 0x18;
        USVSamplingMonitorCmd.can_dlc = 3;
        USVSamplingMonitorCmd.data[0] = 33;
        USVSamplingMonitorCmd.data[1] = 33;
        USVSamplingMonitorCmd.data[2] = 33;
        Can1Write(&USVSamplingMonitorCmd);
        sleep(1);
        //关闭阀门
        ManualSamplingData.can_id = 0x20;
        ManualSamplingData.can_dlc = 2;
        ManualSamplingData.data[0] = 10; 
        ManualSamplingData.data[1] = 100;
        Can1Write(&ManualSamplingData);      
    }

    BoatData.SamplingWorkStatus.CurrentBottleNumber = 255;
    ManualSamplingControl.IsSampling = FALSE;
}