/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "SerialPort2.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>

/*
 * -------------------------------------------------------------------------------------------------------------------------------------------
 *
 *                                                            Private Variables
 *
 * -----------------------------------------------------------------------------------------------------------------------------------------
 */
static INT32 Uart2_fd = -1; // File descriptor of MCS driver.

/*
 * -------------------------------------------------------------------------------------------------------------------------------------------
 *
 *                                                            Public Functions
 *
 * -----------------------------------------------------------------------------------------------------------------------------------------
 */
INT32 Uart2_constructor() {
    struct termios newtio, oldtio;

    if ((Uart2_fd = open(UART2_DEV_NAME, O_RDWR | O_NONBLOCK)) < 0) {
        return -1;
    }
    if (fcntl(Uart2_fd, F_SETFL, 0) < 0) {
        printf("fcntl failed!\n");
    } else {
        printf("fcntl=%d\n", fcntl(Uart2_fd, F_SETFL, 0));
    }
    if (isatty(STDIN_FILENO) == 0) {
        printf("standard input is not a terminal device\n");
    } else {
        printf("isatty success!\n");
    }
    if (tcgetattr(Uart2_fd, &oldtio) != 0) {
        perror("SetupSerial 1");
        return -2;
    }
    bzero(&newtio, sizeof (newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_cflag &= ~PARENB;
    cfsetispeed(&newtio, B9600);
    cfsetospeed(&newtio, B9600);
    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cc[ VTIME ] = 10; //150
    newtio.c_cc[ VMIN ] = 0;
    tcflush(Uart2_fd, TCIFLUSH);
    if ((tcsetattr(Uart2_fd, TCSANOW, &newtio)) != 0) {
        perror("com set error");
        return -2;
    }
    return 0;
}

void Uart2_destructor() {
    if (Uart2_fd != -1) {
        close(Uart2_fd);
        Uart2_fd = -1;
    }
}

UINT16 Uart2_Ultrasonic_read() {
    UINT8 value = 0;
    UINT8 cnt = 0, i;
    UINT16 Sum = 0;
    UINT8 data[20], Uart2Buffer[1];
    BOOL Stauts = FALSE;
    UINT16 UltrasonicData = 0;
    UINT8 WriteData[4] = {0xFF, 0x01, 0x01, 0x01};

    write(Uart2_fd, WriteData, 4);
    //串口读数据,若没有数据会阻塞,
    while (1) {
        if ((value = read(Uart2_fd, Uart2Buffer, 1)) > 0) {
            if (Uart2Buffer[0] == 0xFF) {
                Stauts = TRUE;
            }
            if (Stauts == TRUE) {
                data[cnt] = Uart2Buffer[0];
                cnt++;

                if (cnt == 7) {
                    cnt = 0;
                    Stauts = FALSE;
                    for (i = 0; i < 6; i++) {
                        Sum += data[i];
                        //printf("Uart2 = 0X%X\n", data[i]);
                    }
                    if ((Sum & 0x00ff) == data[6]) {
                        UltrasonicData = data[2] * 256 + data[3];
                        break;
                    } else {
                        value = 0;
                        break;
                    }
                }
            }
        } else {
            break;
        }
    }
    /*
           while (1) {
            if ((value = read(Uart2_fd, Uart2Buffer , 1)) > 0) {
                if (Uart2Buffer[0] == 0x52) {
                    cnt = 0;
                    Stauts = TRUE;
                }
                if(Stauts == TRUE)
                {
                     if(Uart2Buffer[0] == 13)
                    {
                        UltrasonicData = data[1] * 100 + data[2] * 10 + data[3];
                        Stauts = FALSE;
                        cnt = 0;
                        break;
                    }
                    else 
                    {
                        data[cnt] = Uart2Buffer[0] - 48;
                        cnt++;
                        if(cnt > 6)
                        {
                            value = 0;
                            break;
                        }
                    }  
                }
            }
        }
     */

    if (value > 0) {
        return UltrasonicData;
    } else {
        return 0xffff;
    }
}

void Uart2_init() {
    switch (Uart2_constructor()) {
        case -1:
            fprintf(stderr, "Uart2 open faile !\n");
            break;
        case -2:
            fprintf(stderr, "Fail to open the file of Uart2 device!\n");
            break;
    }
    fflush(stderr);
}