/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BoatData.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2017年2月9日, 下午2:16
 */

#ifndef BOATDATA_H
#define BOATDATA_H

#include "Arm.h"

typedef struct Gps
{
    DOUBLE Latitude;
    DOUBLE Longitude;
    FLOAT  Speed;   
    
    UINT8 ByteLatitude[8];
    UINT8 ByteLongitude[8];
    UINT8 ByteSpeed[2]; 
}GPS;

typedef struct Tilt
{
    FLOAT yaw;
    FLOAT pitch;
    FLOAT roll;
    
    UINT8 ByteYaw[2];
    UINT8 Bytepitch[2];
    UINT8 Byteroll[2];
}TILT;

typedef struct USVTime
{
    UINT8 Hour;
    UINT8 Minute;
    UINT8 Second;
}USVTIME;
 
typedef struct USVDate
{
    UINT8 Year;
    UINT8 Month;
    UINT8 Day;  
}USVDATE;

typedef enum
{
    Auto,
    ManualRF,
}MODE;

typedef enum 
{
    GPS_No_Signal = 1,
    Speed_Error = 2,
    Compass_Error = 4,
    Acc_Error = 8,
    Tilt_Error = 16,
    Ultrasonic_Left_Error = 32,
    Ultrasonic_Right_Error = 64,
    Leakage_Error = 128,
    Internet_Error = 256,
    LowSpeed_Warning = 512,
    LowPower_Warning = 1024,
    Sensor_Error = 2048,
    BoardTemp_Warning = 4096,
    ObstaclesDetected = 8192,
}BoatError;


typedef enum 
{
    USVWaypointTask = 1,
    USVRemoteControllerTask = 2,
    USVCannelTask = 3,
    USVAutoControllerTask = 4,
    USVCourseReversalTask = 5,
    USVSpeedTask = 6,
    USVOpenAlarmTask = 7,
    USVCloseAlarmTask = 8,
    USVPauseTask = 9,
    USVRecoverTask = 10,
    USVReadSetting = 11,
    USVSettingPar = 12,
    USVManualSamping = 13,
    USVNoData = 14,
    USVNoSendData = 15,
    USVIsDB = 16,
    USVWashMode = 17,
    USVConstantSpeedSet = 18,
    USVCannelConstantSpeedSet = 19,
    
}RFRECIVERFIG;

typedef struct RobotInfo
{
    //private static Mode currentMode = Mode.ManualRF;   //模式，手动或自动 
    int bat;
    int temperature;
    UINT16 waypoints_status;
    BoatError ErrorMessage;
    UINT16 MissionID;
    MODE CurrentMode;
    int BatteryLife;
    int Temperature; 
    UINT16 Status;
}ROBOTINFO;

typedef struct UltraSonic
{
    UINT16 Sensor1Distance;
    UINT16 Sensor2Distance;  
}ULTRASONIC;

typedef struct WaterPara
{
    float Temperature;  
    float pH; 
    float DissolvedOxygen; 
    float Turbidity; 
    float Conductivity; 
    float Chloride; 
    float Nitrate;
    float Ammonium; 
    float FlowDirection;
    float FlowSpeed;
    float Depth;
    float Chlorophyll;
    float Blue_green_algae; 
}WATERPARA;
  
typedef struct SamplingWorkStatus
{
    int cd; 
    int cb; 
    int sv; 
    int esti_time; 
    int CurrentSPointDepth;
    int CurrentBottleNumber;
    int SampledVolumn;
    int TimeLeft;  
}SAMPLINGWORKSTATUS;

typedef struct USVSettingData
{
    UINT8 RudderMiddle;
}USVSETTINGDATA;

typedef struct BoatData
{
    //For the first time, gps signal has been found. If found, send a message to indicate BaseStation and Remote Controller.
    BOOL  GPSSignalFound;
    BOOL  SendSampleOver;
    BOOL  StartSample;
    UINT16 PropellerCommand;
    UINT16 RudderCommand;
    UINT8 MaxCurrent[4];
    UINT8 LeakCheck;
    GPS Gps;
    TILT Tilt;
    USVTIME USVTime;
    USVDATE USVDate;
    ROBOTINFO RobotInfo;
    ULTRASONIC UltraSonic;
    WATERPARA WaterPara;
    SAMPLINGWORKSTATUS SamplingWorkStatus;  
    RFRECIVERFIG RFReciverMsg;
    USVSETTINGDATA USVSettingData;
}BOATDATA;

typedef enum
{
    Sampleing = 1,
    Monitroing = 2,
    SampleMonitoring = 3,
}SAVETYPE;

typedef struct Database
{
    SAVETYPE SaveType;
    UINT8 SampleBottleNumber;
    UINT8 SampleVolumn;
    UINT8 SamplePoint;
    UINT8 MonitroPoint;
    UINT8 MonitroTime;     
}DATABASE;

extern BOATDATA BoatData;
#endif /* BOATDATA_H */

