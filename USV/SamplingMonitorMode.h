/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SamplingMonitorMode.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2017年3月20日, 上午11:09
 */

#ifndef SAMPLINGMONITORMODE_H
#define SAMPLINGMONITORMODE_H
#include "Arm.h"
#include "AutoPilotData.h"
#include "AutoPilotThread.h"
#include "TakingSampleByRemoteThread.h"
#include "can1.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct can_frame USVSamplingMonitorCmd;

void USVAutoSampling(AUTOPILOTDATA info);

void USVAutoMonitor(AUTOPILOTDATA info);

void USVAutoSamplingMonitor(AUTOPILOTDATA info);

BOOL extern IsSaveDB;

#endif /* SAMPLINGMONITORMODE_H */

