/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MonitorData.h
 * Author: Administrator
 *
 * Created on 2017年4月1日, 上午11:22
 */

#ifndef MONITORDATA_H
#define MONITORDATA_H

#include "Arm.h"

typedef struct MonitorData
{
    BOOL Ishavedata;
    UINT8 data[4];
}MONITORDATA;

extern MONITORDATA MonitorData[10];

#endif /* MONITORDATA_H */

