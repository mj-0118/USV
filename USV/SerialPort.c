/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "SerialPort.h"
#include "AutoPilotThread.h"
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
static INT32 device_fd = -1; // File descriptor of MCS driver.

/*
 * -------------------------------------------------------------------------------------------------------------------------------------------
 *
 *                                                            Public Functions
 *
 * -----------------------------------------------------------------------------------------------------------------------------------------
 */
INT32 serial_constructor(void) {
    struct termios newtio, oldtio;

    if ((device_fd = open(SERIAL_DEV_NAME, O_RDWR | O_NONBLOCK)) < 0) {
        return -1;
    }

    if (fcntl(device_fd, F_SETFL, 0) < 0) {
        printf("fcntl failed!\n");
    } else {
        printf("fcntl=%d\n", fcntl(device_fd, F_SETFL, 0));
    }
    if (isatty(STDIN_FILENO) == 0) {
        printf("standard input is not a terminal device\n");
    } else {
        printf("isatty success!\n");
    }
    if (tcgetattr(device_fd, &oldtio) != 0) {
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
    newtio.c_cc[ VTIME ] = 150;
    newtio.c_cc[ VMIN ] = 0;
    tcflush(device_fd, TCIFLUSH);
    if ((tcsetattr(device_fd, TCSANOW, &newtio)) != 0) {
        perror("com set error");
        return -2;
    }
    return 0;
}

void serial_destructor(void) {
    if (device_fd != -1) {
        close(device_fd);
        device_fd = -1;
    }
}

INT32 serial_write(INT32 startindex, INT32 len, UINT8 data[ ]) {
    INT32 value;
    UINT8 tempdata[ len - startindex + 5 ];

    memcpy(tempdata, data + startindex, sizeof (UINT8) * (len - startindex));

    UINT16 sum = checksum(tempdata, len - startindex);

    tempdata[ len - startindex + 0 ] = sum / 256;
    tempdata[ len - startindex + 1 ] = sum % 256;
    tempdata[ len - startindex + 2 ] = len - startindex + 2;
    tempdata[ len - startindex + 3 ] = 0x0D;
    tempdata[ len - startindex + 4 ] = 0x0A;

    if (USVNavigatorData.NoSend == TRUE) {
        if ((value = write(device_fd, tempdata, len - startindex + 5)) < 0) {
            printf("Vciash Value ERROR = %d\n", value);
            return -1;
        }
    } else {
        value = 0;
    }

    return ( value < 0) ? -1 : 0;
}

UINT32 serial_read(UINT8 data[ ]) {
    UINT32 value = 0;
    UINT32 cnt = 0;

    while (1) {
        if ((value = read(device_fd, data + cnt, 1)) > 0) {
            if (data[ cnt - 1 ] == 0x0D && data[ cnt ] == 0x0A) {
                cnt++;
                printf("serialport is ok\n");
                break;
            }
            cnt++;
        } else {
            printf("serialport is fail\n");
            break;
        }
    }
    return cnt;
}

UINT16 checksum(UINT8 data[ ], INT32 count) {
    UINT16 result = 0;
    UINT32 i;

    for (i = 0; i < count; i++) {
        result += data[ i ];
    }
    return result;
}

void serial_init() {
    USVNavigatorData.NoSend = TRUE;
    switch (serial_constructor()) {
        case -1:
            fprintf(stderr, "serial open faile !\n");
            break;
        case -2:
            fprintf(stderr, "Fail to open the file of serial device!\n");
            break;
    }
    fflush(stderr);
}

void SetRFChannel()
{
    UINT8 TempBuffer[20];

        //Message Header
        TempBuffer[0] = 0x5A;
        TempBuffer[1] = 0x5A;
        TempBuffer[2] = 0x00;
        TempBuffer[3] = 0x00;
        TempBuffer[4] = 0x5A;
        TempBuffer[5] = 0x80;
        TempBuffer[6] = 0x05;
        TempBuffer[7] = 0x02;
        TempBuffer[8] = 0x01;
        TempBuffer[9] = 0x00;
        TempBuffer[10] = 0x96;
        TempBuffer[11] = 0x0D;
        TempBuffer[12] = 0x0A;
        write(device_fd, TempBuffer, 13);

}


//~ Formatted by Jindent --- http://www.jindent.com
