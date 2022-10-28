/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "AllPthreadCreate.h"

void mutex_init ( ) {
    pthread_mutex_init(&mutex, NULL);
}

void CreateSerialCanbusPthread ( ) {
    mutex_init();
    CreateCanbusDataHandleThread();
    CreateSerialDataHandleThread();
    CreateUltrasonicReciveThread();
    CreateReadCan2Thread();
}
//~ Formatted by Jindent --- http://www.jindent.com
