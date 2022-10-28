/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AutoPilotThread.c
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016.11.28, 下午2:24
 */
#include "AllPthreadCreate.h"
#include "AutoPilotData.h"
#include "AutoPilotThread.h"
#include "BoatData.h"
#include "can1.h"
#include "SamplingMonitorMode.h"

USVNAVIGATORDATA USVNavigatorData;
COURSEREVERSALSET CourseReversalSet;
STRAIGHTLINENAVIGATION StraightLineNav; //导航点等分;
CONSTANTSPEED ConstantSpeed; //定速巡航;

double _NeedTurnAngle;
double _Heading;
double _Distance;

UINT8 servo;
UINT8 propeller;
BOOL UltraSonicInfront = FALSE;
UINT8 USVSpeed = 174; //146
#define EQUALDISTANCE  10

struct can_frame MotorWriteFrame;

//自主导航线程

void * AutoPilotThread ( void ) {

    AutoNavigatorInit();

    while (USVNavigatorData.Status) {
        if (BoatData.RobotInfo.CurrentMode == Auto) {
            //导航没有结束;
            CourseReversalSet.NavigatorOver = FALSE;

            if (USVNavigatorData.currentwaypoint_index == 0 && CourseReversalSet.IsGoHome == FALSE) {
                //返航问题；
                BoatData.RobotInfo.Status = (UINT16) (USVNavigatorData.currentwaypoint_index) + 1;
            } else {
                //返航问题；
                BoatData.RobotInfo.Status = (UINT16) (USVNavigatorData.currentwaypoint_index);
            }

            if (USVNavigatorData.Paused) {
                StopBoat();
                usleep(2000);
                continue;
            }

            if (BoatData.Gps.Latitude == 0 || BoatData.Gps.Longitude == 0) {
                StopBoat();
                usleep(2000);
                continue;
            }

            if (BoatData.Gps.Speed < 0.8) {
                BoatData.RobotInfo.ErrorMessage |= Speed_Error;
            } else {
                BoatData.RobotInfo.ErrorMessage &= ~Speed_Error;
            }

/*
            switch (UltrasonicObstacleAvoidance()) {
                case 0:
                    UltraSonicInfront = FALSE;
                    break;
                case 1:
                    if (!UltraSonicInfront) {
                        StopBoat();
                        usleep(20000);
                    }
                    int a = (int) (BoatData.Gps.Speed * 10);
                    USVRetreat();
                    usleep(100 * a); //1600
                    usleep(80000); //80000
                    UltraSonicInfront = TRUE;
                    //绕过等分点,进入下一个等分点;
                    continue;
                case 2:
                    UltraSonicInfront = FALSE;
                    RightTurn();
                    usleep(800000); //400000
                    continue;
                case 3:
                    UltraSonicInfront = FALSE;
                    LeftTurn();
                    usleep(800000); //400000
                    continue;
                case 4:
                    UltraSonicInfront = FALSE;
                    break;
                default:
                    break;
            }
*/

            //经纬度坐标转换平面坐标;
            set_ENU_xy(BoatData.Gps.Latitude, BoatData.Gps.Longitude, &USVNavigatorData.currentx, &USVNavigatorData.currenty);

            _NeedTurnAngle = NeedTurnAngle();
            _Heading = BoatData.Tilt.yaw;
            _Distance = Distance();

            USVConstantSpeed(); //定速巡航;

            servo = USV_servo_command(_NeedTurnAngle, _Heading, _Distance);
            propeller = USV_propeller_command(_NeedTurnAngle, _Heading, _Distance);

            printf("_NeedTurnAngle = %f, _Heading = %f, servo = %d, propeller = %d\n", _NeedTurnAngle, _Heading, servo, propeller);

            //printf("currentspeed = %f, USVSPEED = %d\n", BoatData.Gps.Speed, USVSpeed);

            TrafficBoatMotorCtl(servo, propeller);

            //细分导航点,一个个点开始执行;
            //问题: 船速过快超过此点,船会掉头, 怎么办？ 难道将判断范围再扩大？
            if (CourseReversalSet.IsGoHome == FALSE) {
                if (StraightLineNav.PartPoint > 0) {
                    StraightLineNav.IsDeviNav = TRUE;
                    if (_Distance <= 5 && StraightLineNav.PartPointCount < StraightLineNav.PartPoint) {
                        StraightLineNav.PartPointCount++;
                        DivideNavtigationPointCount(); //重新计算导航目标点;
                    } else if (_Distance <= 5 && StraightLineNav.PartPointCount == StraightLineNav.PartPoint) {
                        //等分执行完成后回复原来坐标点;
                        USVNavigatorData.targetx = StraightLineNav.Savetargetx;
                        USVNavigatorData.targety = StraightLineNav.Savetargety;
                        _Distance = Distance(); //剩余导航路径重新计算,不计算,导航任务其实没有完成;
                        StraightLineNav.IsDeviNav = FALSE;
                        StraightLineNav.PartPoint = 0; //防止二次进入;
                    }
                }
            } else {
                StraightLineNav.IsDeviNav = FALSE;
            }

            //开始导航及细分后剩余路程导航
            if (StraightLineNav.IsDeviNav == FALSE) {
                //正常导航，不返航;
                if (CourseReversalSet.IsGoHome == FALSE) {
                    if (_Distance <= 5 && (USVNavigatorData.currentwaypoint_index == USVNavigatorData.NavigatorPointNumber - 1)) {
                        GotToLastNaviPoint();
                        break;
                    } else if (_Distance <= 5 && (USVNavigatorData.currentwaypoint_index < USVNavigatorData.NavigatorPointNumber - 1)) {
                        GotToNaviPoint();
                    }
                } else //返航中..
                {
                    if (USVNavigatorData.currentwaypoint_index >= 0 && CourseReversalSet.GoHomePreventRepeatedlyMinus == FALSE) { //自主导航没有结束,收到导航任务需要减一个导航点提高导航效率;
                        if (USVNavigatorData.currentwaypoint_index == 0) {
                            USVNavigatorData.currentwaypoint_index = 0;
                        } else {
                            USVNavigatorData.currentwaypoint_index--;
                        }
                        set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index].Latitude, AutoP[USVNavigatorData.currentwaypoint_index].Longitude, &USVNavigatorData.targetx, &USVNavigatorData.targety);
                        CourseReversalSet.GoHomePreventRepeatedlyMinus = TRUE;
                    }

                    if (_Distance <= 5 && USVNavigatorData.currentwaypoint_index == 0) {
                        StopBoat();
                        //返航完成;
                        BoatData.RobotInfo.Status = 65528;
                        break;
                    } else if (_Distance <= 5 && USVNavigatorData.currentwaypoint_index > 0) {
                        GohomeNavPoint();
                    }
                }
            }

            usleep(1000);
        }
    }
    //导航结束;
    CourseReversalSet.NavigatorOver = TRUE;
    //清除返航状态位;
    CourseReversalSet.IsGoHome = FALSE;
    //清除状态位;
    CourseReversalSet.GoHomePreventRepeatedlyMinus = FALSE;
    //数据库保存关闭;
    IsSaveDB = FALSE;
    CourseReversalSet.PointMaxToMin = FALSE;
    pthread_exit((void *) 100);
}

//创建导航线程

void CreateAutoPilotThread ( void ) {
    int ret;

    ret = pthread_create(&AutoPilot, NULL, (void *) AutoPilotThread, NULL);
    if (ret != 0) {
        printf("Create AotoPilot pthread error!\n");
        exit(1);
    } else {
        printf("Create AotoPilot pthread success!\n");
    }
}

//经纬度计算初始化

void AutoNavigatorInit ( ) {

    USVNavigatorData.originlat = BoatData.Gps.Latitude;
    USVNavigatorData.originlng = BoatData.Gps.Longitude;

    USVNavigatorData.SinOfOriginLat = sin(rad(USVNavigatorData.originlat));
    USVNavigatorData.CosOfOriginLat = cos(rad(USVNavigatorData.originlat));
    USVNavigatorData.SinOfOriginLng = sin(rad(USVNavigatorData.originlng));
    USVNavigatorData.CosOfOriginLng = cos(rad(USVNavigatorData.originlng));

    USVNavigatorData.normal = EARTH_RADIUS / sqrt(1.0 - SQE * USVNavigatorData.SinOfOriginLat * USVNavigatorData.SinOfOriginLat);

    USVNavigatorData.OriginECEF_X = USVNavigatorData.normal * USVNavigatorData.CosOfOriginLat * USVNavigatorData.CosOfOriginLng;
    USVNavigatorData.OriginECEF_Y = USVNavigatorData.normal * USVNavigatorData.CosOfOriginLat * USVNavigatorData.SinOfOriginLng;
    USVNavigatorData.OriginECEF_Z = USVNavigatorData.normal * (1.0 - SQE) * USVNavigatorData.SinOfOriginLat;

    USVNavigatorData.Bx = (long) (USVNavigatorData.SinOfOriginLng * USVNavigatorData.OriginECEF_X - USVNavigatorData.CosOfOriginLng * USVNavigatorData.OriginECEF_Y);

    USVNavigatorData.By = (long) (USVNavigatorData.SinOfOriginLat * USVNavigatorData.CosOfOriginLng * USVNavigatorData.OriginECEF_X + \
                                 USVNavigatorData.SinOfOriginLat * USVNavigatorData.SinOfOriginLng * USVNavigatorData.OriginECEF_Y - \
                                 USVNavigatorData.CosOfOriginLat * USVNavigatorData.OriginECEF_Z);

    if (CourseReversalSet.PointMaxToMin == FALSE) {
        //没有返航导航点从0开始加加;
        USVNavigatorData.currentwaypoint_index = 0;
    } else {
        //自动返航如果导航结束再返航需要赋值最高点,从最高点开始减减;
        USVNavigatorData.currentwaypoint_index = USVNavigatorData.NavigatorPointNumber - 1;
        CourseReversalSet.PointMaxToMin = FALSE;
    }

    //INIT 
    USVNavigatorData.Paused = FALSE;
    USVNavigatorData.Status = TRUE;
    StraightLineNav.PartPoint = 0;
    StraightLineNav.IsDeviNav = FALSE;
    ConstantSpeed.ResponseTime = 0;         //初始化响应时间;
    //ConstantSpeed.Enable = FALSE;
    
    set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index].Latitude, AutoP[USVNavigatorData.currentwaypoint_index].Longitude, &USVNavigatorData.targetx, &USVNavigatorData.targety);
}

double rad ( double d ) {
    return d * PI / 180.0;
}

//平面坐标系转换

void set_ENU_xy ( double lat, double lng, double *x, double *y ) {
    double coslat = cos(rad(lat));
    double coslng = cos(rad(lng));
    double sinlat = sin(rad(lat));
    double sinlng = sin(rad(lng));

    double normal = EARTH_RADIUS / sqrt(1.0 - SQE * pow(sinlat, 2));

    double X = normal * coslat * coslng;
    double Y = normal * coslat * sinlng;
    double Z = normal * (1.0 - SQE) * sinlat;

    *x = (X - USVNavigatorData.OriginECEF_X) * (-USVNavigatorData.SinOfOriginLng) + (Y - USVNavigatorData.OriginECEF_Y) * USVNavigatorData.CosOfOriginLng;
    *y = (X - USVNavigatorData.OriginECEF_X) * (-USVNavigatorData.SinOfOriginLat * USVNavigatorData.CosOfOriginLng) + (Y - USVNavigatorData.OriginECEF_Y) * \
         (-USVNavigatorData.SinOfOriginLat * USVNavigatorData.SinOfOriginLng) + (Z - USVNavigatorData.OriginECEF_Z) * USVNavigatorData.CosOfOriginLat;
}

//角度计算

double DesiredAngle ( ) {
    if ((abs(USVNavigatorData.currentx - USVNavigatorData.targetx)) <= 1) {
        if (USVNavigatorData.currenty > USVNavigatorData.targety)
            return -180.0;
        else return 0;
    }

    double atan1 = atan(MathRound((USVNavigatorData.currenty - USVNavigatorData.targety) /
            (USVNavigatorData.currentx - USVNavigatorData.targetx), 6));

    atan1 = MathRound(atan1, 6);

    // The case when Xc < Xd, that is, destination is on the right of current point.
    if (USVNavigatorData.currentx - USVNavigatorData.targetx < 0)
        return MathRound((90.0 - MathRound(atan1 / 3.141593 * 180.0, 2)), 2);

        // The case when Xc > Xd, that is, destination is on the left of current point.
    else
        return MathRound((-90.0 - MathRound(atan1 / 3.141593 * 180.0, 2)), 2);
}

//USV朝向计算

double NeedTurnAngle ( ) {
    double need = DesiredAngle() - BoatData.Tilt.yaw;

    if (need >= 180)
        need -= 360;
    else if (need < -180)
        need += 360;

    return need;
}

//取小数点

double MathRound ( double dVal, short iPlaces ) {
    double dRetval;
    double dMod = 0.0000001;
    if (dVal < 0.0)
        dMod = -0.0000001;
    dRetval = dVal;
    dRetval += (5.0 / pow(10.0, iPlaces + 1.0));
    dRetval *= pow(10.0, iPlaces);
    dRetval = floor(dRetval + dMod);
    dRetval /= pow(10.0, iPlaces);
    return ( dRetval);
}

//计算距离

double Distance ( ) {
    return MathRound(sqrt(pow((USVNavigatorData.currentx - USVNavigatorData.targetx), 2) + pow((USVNavigatorData.currenty - USVNavigatorData.targety), 2)), 2);
}

//转向

UINT8 USV_servo_command ( double need, double heading, double distance ) {
    //船朝向在-35和35之间才进行压线处理
    if (need < 35 && need > -35) {
        /*
                if (need <= 5 && need >= -5) {
                    return (UINT8) (BoatData.USVSettingData.RudderMiddle);
                } else {
         */
        //return (UINT8) (BoatData.USVSettingData.RudderMiddle - (UINT8) (int) (need / 3));               //attention
        return ((UINT8) BoatData.USVSettingData.RudderMiddle - (UINT8) (int) (need * 0.572f)); //attention 2018/3/4
        /*
                }
         */
    } else if (need >= 35) {
        return (UINT8) (BoatData.USVSettingData.RudderMiddle - 20);
        //return 132;
    } else //(need <= -35)
    {
        return (UINT8) (BoatData.USVSettingData.RudderMiddle + 20);
        //return 172; //192
    }
}

//Motor

UINT8 USV_propeller_command ( double need, double heading, double distance ) {
    if (distance <= 10 && StraightLineNav.IsDeviNav == FALSE) //Too close to the target 防止等分导航速度变慢;
    {
        //return (UINT8) 160;
        return USVSpeed;
    } else if (need >= 35 || need <= -35) //if a big angle it needs to turn, it lowers its speed
    {
        if(ConstantSpeed.Speed >= 1 && ConstantSpeed.Speed <= 7)
        {
            return USVSpeed;
        }
        else 
        {
            return (UINT8) 168;  
        }           
    } else {
        if (need <= 5 && need >= -5) {
            return USVSpeed;
        } else {
            return USVSpeed;              //samlu 170
        }
    }
}

//转向和动力输出

/*
void TrafficBoatMotorCtl ( UINT8 Turn, UINT8 Power ) {
    MotorWriteFrame.can_id = 0x23;
    MotorWriteFrame.can_dlc = 2;
    MotorWriteFrame.data[0] = Turn;
    MotorWriteFrame.data[1] = Power;
    Can1Write(&MotorWriteFrame);
}
 */
void TrafficBoatMotorCtl ( UINT8 Turn, UINT8 Power ) {
    MotorWriteFrame.can_id = 0x23;
    MotorWriteFrame.can_dlc = 2;
    //MotorWriteFrame.data[0] = Turn;
    if (Turn >= BoatData.USVSettingData.RudderMiddle && Turn <= 200) {
        MotorWriteFrame.data[0] = BoatData.USVSettingData.RudderMiddle - (Turn - BoatData.USVSettingData.RudderMiddle);
    } else if (Turn >= 100 && Turn < BoatData.USVSettingData.RudderMiddle) {
        MotorWriteFrame.data[0] = BoatData.USVSettingData.RudderMiddle + (BoatData.USVSettingData.RudderMiddle - Turn);

    } else {
        MotorWriteFrame.data[0] = BoatData.USVSettingData.RudderMiddle;
    }
    MotorWriteFrame.data[1] = Power;
    Can1Write(&MotorWriteFrame);
}

void GotToNaviPoint ( ) {
    if (AutoP[USVNavigatorData.currentwaypoint_index].TaskType == SampleMode) {
        BoatData.RobotInfo.Status = 65534;
        if (AutoP[USVNavigatorData.currentwaypoint_index].SampleWashFactor == 16) {
            BoatData.RobotInfo.Status = 65529;
            USVAutoSamplingMonitor(AutoP[USVNavigatorData.currentwaypoint_index]);
        } else {
            USVAutoSampling(AutoP[USVNavigatorData.currentwaypoint_index]);
        }

    } else if (AutoP[USVNavigatorData.currentwaypoint_index].TaskType == MonitorMode) {
        BoatData.RobotInfo.Status = (UINT16) (32768 + (USVNavigatorData.currentwaypoint_index));
        USVAutoMonitor(AutoP[USVNavigatorData.currentwaypoint_index]);
    }

    USVNavigatorData.currentwaypoint_index++;
    set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index].Latitude, AutoP[USVNavigatorData.currentwaypoint_index].Longitude, &USVNavigatorData.targetx, &USVNavigatorData.targety);

    if (CourseReversalSet.IsGoHome == FALSE) {
        //计算导航点等分距离个数;
        DivideNavtigationPoint(USVNavigatorData.currentwaypoint_index);
        //等分转换目标导航点
        DivideNavtigationPointCount();
    }
}

void GotToLastNaviPoint ( ) {
    if (AutoP[USVNavigatorData.currentwaypoint_index].TaskType == SampleMode) {
        BoatData.RobotInfo.Status = 65534;
        if (AutoP[USVNavigatorData.currentwaypoint_index].SampleWashFactor == 16) {
            BoatData.RobotInfo.Status = 65529;
            USVAutoSamplingMonitor(AutoP[USVNavigatorData.currentwaypoint_index]);
        } else {
            USVAutoSampling(AutoP[USVNavigatorData.currentwaypoint_index]);
        }
    } else if (AutoP[USVNavigatorData.currentwaypoint_index].TaskType == MonitorMode) {
        BoatData.RobotInfo.Status = (UINT16) (32768 + (USVNavigatorData.currentwaypoint_index));
        USVAutoMonitor(AutoP[USVNavigatorData.currentwaypoint_index]);
    } else {
        StopBoat();
    }

    BoatData.RobotInfo.Status = 65535;
}

//返航

void GohomeNavPoint ( ) {
    if (USVNavigatorData.currentwaypoint_index != 0) {
        USVNavigatorData.currentwaypoint_index--;
    }
    set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index].Latitude, AutoP[USVNavigatorData.currentwaypoint_index].Longitude, &USVNavigatorData.targetx, &USVNavigatorData.targety);
}

//停船

void StopBoat ( ) {
    TrafficBoatMotorCtl(BoatData.USVSettingData.RudderMiddle, 152);
}

//后退

void USVRetreat ( ) {
    TrafficBoatMotorCtl(BoatData.USVSettingData.RudderMiddle, 120);
}

//超声波避障

UINT8 UltrasonicObstacleAvoidance ( ) {
    if (UltrasonicData1[0] + UltrasonicData1[1] + UltrasonicData1[2] >= 22500 && UltrasonicData2[0] + UltrasonicData2[1] + UltrasonicData2[2] >= 22500) {
        return 0;
    } else if (UltrasonicData1[0] + UltrasonicData1[1] + UltrasonicData1[2] <= 12000 || UltrasonicData2[0] + UltrasonicData2[1] + UltrasonicData2[2] <= 12000) {
        return 1;
    } else if (UltrasonicData1[0] + UltrasonicData1[1] + UltrasonicData1[2] <= 18000 || UltrasonicData2[0] + UltrasonicData2[1] + UltrasonicData2[2] <= 18000) {
        if (UltrasonicData1[0] + UltrasonicData1[1] + UltrasonicData1[2] >= UltrasonicData2[0] + UltrasonicData2[1] + UltrasonicData2[2]) {
            return 3; //turn Left
        } else {
            return 2; //turn Right
        }
    } else {
        return 4;
    }
}

//压线距离计算
//此函数废弃,仅供参考

double GrooveDistance ( ) {
    //注意越界
    //返航不采取压线
    if (CourseReversalSet.IsGoHome == FALSE) {
        if (USVNavigatorData.currentwaypoint_index >= 2 && USVNavigatorData.currentwaypoint_index <= USVNavigatorData.NavigatorPointNumber - 1) {
            set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index - 1].Latitude, AutoP[USVNavigatorData.currentwaypoint_index - 1].Longitude, &USVNavigatorData.NavigatorA_x, &USVNavigatorData.NavigatorA_y);
            set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index].Latitude, AutoP[USVNavigatorData.currentwaypoint_index].Longitude, &USVNavigatorData.NavigatorB_x, &USVNavigatorData.NavigatorB_y);
            set_ENU_xy(BoatData.Gps.Latitude, BoatData.Gps.Longitude, &USVNavigatorData.currentx, &USVNavigatorData.currenty);
            USVNavigatorData.Angle = USVLocation(USVNavigatorData.NavigatorA_x, USVNavigatorData.NavigatorA_y, USVNavigatorData.NavigatorB_x, USVNavigatorData.NavigatorB_y, USVNavigatorData.currentx, USVNavigatorData.currenty);
            //printf("Angle = %d\n", USVNavigatorData.Angle);
            return ENUGetNearestDistance(USVNavigatorData.NavigatorA_x, USVNavigatorData.NavigatorA_y, USVNavigatorData.NavigatorB_x, USVNavigatorData.NavigatorB_y, USVNavigatorData.currentx, USVNavigatorData.currenty);
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

//判断USV当前位置在航线左侧还是右侧
//1 表示左侧 2 表示右侧 0 表示航线上
//此函数废弃,仅供参考

UINT8 USVLocation ( double startx, double starty, double endx, double endy, double currentx, double currenty ) {
    double S;
    S = (startx - currentx) * (endy - currenty) - (starty - currenty) * (endx - currentx);
    if (S > 0) {
        return 1;
    } else if (S < 0) {
        return 2;
    } else {
        return 0;
    }
}

//计算两坐标点直线距离

double getDistanceBtwP ( double startx, double starty, double endx, double endy ) {
    return MathRound(sqrt(pow((startx - endx), 2) + pow((starty - endy), 2)), 2);
}

//计算点到航向垂直距离,钝角除外;
//此函数废弃,仅供参考

double ENUGetNearestDistance ( double startx, double starty, double endx, double endy, double usvcurrentx, double usvcurrenty ) {
    double a, b, c;
    a = getDistanceBtwP(startx, starty, endx, endy);
    b = getDistanceBtwP(usvcurrentx, usvcurrenty, endx, endy);
    c = getDistanceBtwP(usvcurrentx, usvcurrenty, startx, starty);
    printf("\n a = %f, b = %f, c = %f\n", a, b, c);
    if (b * b >= c * c + a * a)
        return -1; //钝角                   
    if (c * c >= b * b + a * a)
        return -1; //钝角                
    double l = (a + b + c) / 2;
    double s = sqrt(l * (l - a) * (l - b) * (l - c));
    return 2 * s / a;
}

//左转向

void LeftTurn ( ) {
    TrafficBoatMotorCtl(BoatData.USVSettingData.RudderMiddle + 20, 170);
}

//右转向

void RightTurn ( ) {
    TrafficBoatMotorCtl(BoatData.USVSettingData.RudderMiddle - 20, 170);
}

//返航初始化

void CourseReversalSetinit ( ) {
    CourseReversalSet.IsGoHome = FALSE;
    CourseReversalSet.NavigatorOver = FALSE;
    CourseReversalSet.GoHomePreventRepeatedlyMinus = FALSE;
    CourseReversalSet.IsCourseReversal = FALSE;
    CourseReversalSet.PointMaxToMin = FALSE;
    BoatData.SendSampleOver = FALSE;
    BoatData.StartSample = FALSE;
}

//等分导航点个数计算

void DivideNavtigationPoint ( UINT8 NavtigationPoint ) {
    if (CourseReversalSet.IsGoHome == FALSE) {
        if (NavtigationPoint >= 2 && NavtigationPoint <= USVNavigatorData.NavigatorPointNumber - 1) {
            //将导航经纬度坐标转化成平面坐标
            set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index - 1].Latitude, AutoP[USVNavigatorData.currentwaypoint_index - 1].Longitude, &StraightLineNav.targetAx, &StraightLineNav.targetAy);
            set_ENU_xy(AutoP[USVNavigatorData.currentwaypoint_index].Latitude, AutoP[USVNavigatorData.currentwaypoint_index].Longitude, &StraightLineNav.targetBx, &StraightLineNav.targetBy);
            //保存两点经纬度平面坐标；
            StraightLineNav.SavetargetAx = StraightLineNav.targetAx;
            StraightLineNav.SavetargetBx = StraightLineNav.targetBx;
            StraightLineNav.SavetargetAy = StraightLineNav.targetAy;
            StraightLineNav.SavetargetBy = StraightLineNav.targetBy;
            //保存当前目标点
            StraightLineNav.Savetargetx = USVNavigatorData.targetx;
            StraightLineNav.Savetargety = USVNavigatorData.targety;
            //计算两点距离
            StraightLineNav.Distance = MathRound(sqrt(pow((StraightLineNav.targetAx - StraightLineNav.targetBx), 2) + pow((StraightLineNav.targetAy - StraightLineNav.targetBy), 2)), 2);
            //两点坐标垂直Y轴处理
            if (abs(StraightLineNav.targetAx - StraightLineNav.targetBx) <= 1) {
                //计算坐标距离
                StraightLineNav.Distance = fabs(StraightLineNav.targetBy - StraightLineNav.targetAy);
            } else {
                //计算坐标距离
                StraightLineNav.Distance = MathRound(sqrt(pow((StraightLineNav.targetAx - StraightLineNav.targetBx), 2) + pow((StraightLineNav.targetAy - StraightLineNav.targetBy), 2)), 2);
            }

            //等分距离个数
            StraightLineNav.PartPoint = (UINT32) (StraightLineNav.Distance / EQUALDISTANCE); //每10M进行等分

            if (StraightLineNav.PartPoint >= 2) {
                StraightLineNav.PartPoint -= 1;
            } else {
                StraightLineNav.PartPoint = 0;
            }

            if (StraightLineNav.PartPoint > 0) {
                StraightLineNav.PartPointCount = 1; //等分计数点赋予初值;
            }
        }
    }
}
//等分导航点后,平面坐标系目标点(targetx,targety)转化;

void DivideNavtigationPointCount ( ) {
    //计算两点经纬度度平面坐标x, y 坐标;
    if (StraightLineNav.PartPoint > 0) {
        if (abs(StraightLineNav.targetAx - StraightLineNav.targetBx) <= 1) {
            //如果两点坐标与Y轴平行;
            if (StraightLineNav.targetBy > StraightLineNav.targetAy) {
                USVNavigatorData.targetx = StraightLineNav.targetAx;
                StraightLineNav.targetAy += (fabs(StraightLineNav.targetAy - StraightLineNav.targetBy) / (StraightLineNav.PartPoint + 1)* 1.00f);
                USVNavigatorData.targety = StraightLineNav.targetAy;
            } else {
                USVNavigatorData.targetx = StraightLineNav.targetAx;
                StraightLineNav.targetAy -= (fabs(StraightLineNav.targetAy - StraightLineNav.targetBy) / (StraightLineNav.PartPoint + 1) * 1.00f);
                USVNavigatorData.targety = StraightLineNav.targetAy;
            }
        } else {
            //如果两点坐标与Y轴不平行,导航到达等分点坐标后,不断计算targetx, targety;
            if (StraightLineNav.targetBx > StraightLineNav.targetAx) {
                StraightLineNav.targetAx += (fabs(StraightLineNav.targetAx - StraightLineNav.targetBx) / (StraightLineNav.PartPoint + 1) * 1.00f);
                USVNavigatorData.targetx = StraightLineNav.targetAx;
                USVNavigatorData.targety = (StraightLineNav.SavetargetBy - StraightLineNav.SavetargetAy) / (StraightLineNav.SavetargetBx - StraightLineNav.SavetargetAx) * (USVNavigatorData.targetx - StraightLineNav.SavetargetAx) + StraightLineNav.SavetargetAy;
            } else if (StraightLineNav.targetBx < StraightLineNav.targetAx) {
                StraightLineNav.targetAx -= (fabs(StraightLineNav.targetAx - StraightLineNav.targetBx) / (StraightLineNav.PartPoint + 1) * 1.00f);
                USVNavigatorData.targetx = StraightLineNav.targetAx;
                USVNavigatorData.targety = (StraightLineNav.SavetargetBy - StraightLineNav.SavetargetAy) / (StraightLineNav.SavetargetBx - StraightLineNav.SavetargetAx) * (USVNavigatorData.targetx - StraightLineNav.SavetargetAx) + StraightLineNav.SavetargetAy;
            }
        }
        /*
                printf("USVNavigatorData.targetx = %f, USVNavigatorData.targety = %f\n", USVNavigatorData.targetx, USVNavigatorData.targety);
                printf("StraightLineNav.PartPoint = %d\n", StraightLineNav.PartPoint);
                printf("StraightLineNav.PartPointCount = %d\n", StraightLineNav.PartPointCount);
         */
    }
}

void USVConstantSpeed ( void ) {
    //响应时间
    if (ConstantSpeed.Enable == TRUE) {
        ConstantSpeed.ResponseTime++;

        if (ConstantSpeed.ResponseTime >= ConstantSpeed.ResponseSpeed) { //1000
            ConstantSpeed.ResponseTime = 0;
            
            if(ConstantSpeed.Speed >= 1 && ConstantSpeed.Speed <= 7)
            {
                if(abs(BoatData.Gps.Speed - ConstantSpeed.Speed) <= 2)  //迅速调速后，再增加响应时间；
                {
                    ConstantSpeed.ResponseSpeed = 700;
                }
            }

            if (BoatData.Gps.Speed > ConstantSpeed.Speed) {
                USVSpeed--;
                if (USVSpeed <= 154) {
                    USVSpeed = 154;
                }
            } else if (BoatData.Gps.Speed < ConstantSpeed.Speed) {
                USVSpeed++;
                if (USVSpeed >= 192) {
                    USVSpeed = 192;
                }
            }
        }
    }
}