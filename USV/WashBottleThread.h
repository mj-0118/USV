/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WishBottleThread.h
 * Author: Administrator
 *
 * Created on 2017年4月11日, 下午3:10
 */

#ifndef WASHBOTTLETHREAD_H
#define WASHBOTTLETHREAD_H

#include "AllPthreadCreate.h"

void * WashBottleThread();

void CreateWashBottleThread();

void WashModeFinished(UINT8 BottleNumber);

void WashBottleHandle(UINT8 Volume, UINT8 BottleNumber);

struct can_frame WashClean;

extern UINT8 WashVolume;

extern BOOL WashModeRun;

#endif /* WISHBOTTLETHREAD_H */

