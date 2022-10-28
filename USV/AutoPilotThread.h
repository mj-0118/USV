/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AutoPilotThread.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016年12月1日, 下午4:56
 */
#ifndef AUTOPILOTTHREAD_H
#define AUTOPILOTTHREAD_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "can1.h"

#define EARTH_RADIUS 6378137.0
#define SQE 0.00669438
#define PI  3.14159

typedef struct USVNavigatorData{
    double targetx;
    double targety;
    double currentx;
    double currenty;
    double originlat;
    double originlng;
    double OriginECEF_X;
    double OriginECEF_Y;
    double OriginECEF_Z;
    double SinOfOriginLat;
    double CosOfOriginLat;
    double SinOfOriginLng;
    double CosOfOriginLng;
    double normal;
    BOOL Paused;
    BOOL Status;
    BOOL NoSend;
    UINT8 NavigatorPointNumber;
    UINT8 currentwaypoint_index;
    long Bx;
    long By;
    double NavigatorA_x;
    double NavigatorA_y;
    double NavigatorB_x;
    double NavigatorB_y;
    UINT8 Angle;
}USVNAVIGATORDATA;

typedef struct CourseReversalSet
{
    BOOL  IsGoHome;
    BOOL  NavigatorOver;
    BOOL  IsCourseReversal;
    BOOL  GoHomePreventRepeatedlyMinus;
    BOOL  PointMaxToMin;
}COURSEREVERSALSET;

typedef struct straightLine
{
    double Savetargetx;
    double Savetargety;
    double targetAx;
    double targetAy;
    double targetBx;
    double targetBy;
    
    double SavetargetAx;
    double SavetargetAy;
    double SavetargetBx;
    double SavetargetBy;
    
    UINT32 PartPoint; //等分点
    UINT32 PartPointCount;
    double Distance;
    BOOL IsDeviNav;
}STRAIGHTLINENAVIGATION;

typedef struct constantspeed
{
    UINT16 ResponseTime;
    UINT16 ResponseSpeed;
    UINT8 Speed;
    BOOL Enable;
}CONSTANTSPEED;

extern CONSTANTSPEED ConstantSpeed;

extern STRAIGHTLINENAVIGATION StraightLineNav;

extern COURSEREVERSALSET CourseReversalSet;

extern USVNAVIGATORDATA USVNavigatorData;

extern UINT8 USVSpeed;

void * AutoPilotThread( void );

void CreateAutoPilotThread( void );

double rad(double d);

void set_ENU_xy(double lat, double lng, double *x, double *y);

double NeedTurnAngle();

double MathRound(double dVal, short iPlaces);

double DesiredAngle();

double Distance();

void AutoNavigatorInit();

void TrafficBoatMotorCtl(UINT8 Turn, UINT8 Power);

UINT8 USV_servo_command(double need, double heading, double distance);

UINT8 USV_propeller_command(double need, double heading, double distance);

void GotToNaviPoint();

void GotToLastNaviPoint();

void GohomeNavPoint();

void StopBoat();

void USVRetreat();

UINT8 UltrasonicObstacleAvoidance();

void LeftTurn();

void RightTurn();

void CourseReversalSetinit();

double GrooveDistance();

double getDistanceBtwP(double startx, double starty, double endx, double endy);

double ENUGetNearestDistance(double startx, double starty, double endx, double endy, double usvcurrentx, double usvcurrenty);

UINT8 USVLocation(double startx, double starty, double endx, double endy, double currentx, double currenty);

void DivideNavtigationPoint ( UINT8 NavtigationPoint );

void DivideNavtigationPointCount ( );

void USVConstantSpeed(void);

#endif /* AUTOPILOTTHREAD_H */


//~ Formatted by Jindent --- http://www.jindent.com
