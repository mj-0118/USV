/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReadCan2Thread.h
 * Author: Administrator
 *
 * Created on 2017年3月26日, 下午1:25
 */

#ifndef READCAN2THREAD_H
#define READCAN2THREAD_H

#include "AllPthreadCreate.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "can1.h"
#include "Arm.h"

void * ReadCan2Thread();

void CreateReadCan2Thread();

void send4gLongitude(char data[]);

void send4gLatitude(char data[]);

void send4gBatteryLife();

#endif /* READCAN2THREAD_H */

