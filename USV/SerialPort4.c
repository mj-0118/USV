/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "SerialPort4.h"
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
static INT32 Uart4_fd = -1; // File descriptor of MCS driver.

/*
 * -------------------------------------------------------------------------------------------------------------------------------------------
 *
 *                                                            Public Functions
 *
 * -----------------------------------------------------------------------------------------------------------------------------------------
 */
INT32 Uart4_constructor() {
    struct termios newtio, oldtio;

    if ((Uart4_fd = open(UART4_DEV_NAME, O_RDWR | O_NONBLOCK)) < 0) {
        return -1;
    }
    if (fcntl(Uart4_fd, F_SETFL, 0) < 0) {
        printf("fcntl failed!\n");
    } else {
        printf("fcntl = %d\n", fcntl(Uart4_fd, F_SETFL, 0));
    }
    if (isatty(STDIN_FILENO) == 0) {
        printf("standard input is not a terminal device\n");
    } else {
        printf("isatty success!\n");
    }
    if (tcgetattr(Uart4_fd, &oldtio) != 0) {
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
    tcflush(Uart4_fd, TCIFLUSH);
    if ((tcsetattr(Uart4_fd, TCSANOW, &newtio)) != 0) {
        perror("com set error");
        return -2;
    }
    return 0;
}

void Uart4_destructor() {
    if (Uart4_fd != -1) {
        close(Uart4_fd);
        Uart4_fd = -1;
    }
}

UINT16 Uart4_Ultrasonic_read() {
    UINT8 value = 0;
    UINT8 cnt = 0, i;
    UINT16 Sum = 0;
    UINT8 data[20], Uart4Buffer[1];
    BOOL Stauts = FALSE;
    UINT16 UltrasonicData = 0;
    UINT8 WriteData[4] = {0xFF, 0x01, 0x01, 0x01};

    write(Uart4_fd, WriteData, 4);
    while (1) {
        // printf("write(Uart4_fd, WriteData, 4); \n");
        if ((value = read(Uart4_fd, Uart4Buffer, 1)) > 0) {
            if (Uart4Buffer[0] == 0xFF) {
                Stauts = TRUE;
            }
            if (Stauts == TRUE) {
                data[cnt] = Uart4Buffer[0];
                cnt++;

                if (cnt == 7) {
                    cnt = 0;
                    Stauts = FALSE;
                    for (i = 0; i < 6; i++) {
                        Sum += data[i];
                        //printf("UART4 = 0X%X\n", data[i]);
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

    if (value > 0) {
        return UltrasonicData;
    } else {
        return 0xffff;
    }

    /*
        while (1) {
            if ((value = read(Uart4_fd, Uart4Buffer, 1)) > 0) {
                if (Uart4Buffer[0] == 0x52) {
                    cnt = 0;
                    Stauts = TRUE;
                }
                if (Stauts == TRUE) {
                    if(Uart4Buffer[0] == 13)
                    {
                        UltrasonicData = data[1] * 100 + data[2] * 10 + data[3];
                        Stauts = FALSE;
                        cnt = 0;
                        break;
                    }
                    else 
                    {
                        data[cnt] = Uart4Buffer[0] - 48;
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

        if (value > 0) {
            return UltrasonicData;
        } else {
            return 0xffff;
        }
     */

}

void Uart4_init() {
    switch (Uart4_constructor()) {
        case -1:
            fprintf(stderr, "Uart4 open faile !\n");
            break;
        case -2:
            fprintf(stderr, "Fail to open the file of Uart4 device!\n");
            break;
    }
    fflush(stderr);
}