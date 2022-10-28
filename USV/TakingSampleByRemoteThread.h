/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TakingSampleByRemoteThread.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016年12月1日, 下午4:57
 */
#ifndef TAKINGSAMPLEBYREMOTETHREAD_H
#define TAKINGSAMPLEBYREMOTETHREAD_H

#include "BoatData.h"
#include "Arm.h"

typedef struct ManualSampling {
    UINT8 BottleNumber;
    BOOL IsSampling;
    BOOL IsMonitor;
    UINT8 Volume;
    UINT8 WashFactor;
    UINT8 Depth;
    BOOL SamplingIsAck;
} MANUALSAMPLINGCONTROL;

extern MANUALSAMPLINGCONTROL ManualSamplingControl;

void * TakingSampleByRemoteThread();

void CreateTakingSampleByRemoteThread();

void TakingManualSampling(UINT8 Volume, UINT8 bottle, UINT8 depth);

void SendManualSamplingFinished(UINT8 BottleNumber);

struct can_frame ManualSamplingData;

#endif /* TAKINGSAMPLEBYREMOTETHREAD_H */


//~ Formatted by Jindent --- http://www.jindent.com
