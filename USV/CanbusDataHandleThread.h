/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   CanbusDateHandlePthread.h
 * Author: samlu(samlu@vciash.com)
 *
 * Created on 2016年12月1日, 下午4:56
 */
#ifndef CANBUSDATAHANDLETHREAD_H
#define CANBUSDATAHANDLETHREAD_H

#include "Arm.h"

void * CanbusDataHandleThread( );

void CreateCanbusDataHandleThread( );

double BytesToDouble(UINT8 data[]  );


#endif /* CANBUSDATEHANDLEPTHREAD_H */


//~ Formatted by Jindent --- http://www.jindent.com
