/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
 ============================================================================================================================================
 Author      : samlu(sam.lu@vciash.com)
 Description : CAN socket handler.
 Status      : DEBUG
 ============================================================================================================================================
 */
#include "can0.h"
#include <errno.h>
#include <linux/can.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Arm.h"

INT32 s;
struct sockaddr_can addr;
struct ifreq ifr;
struct timeval tv;

void Can0_init(void) {

    tv.tv_sec = 5; // 30 Secs Timeout 
    tv.tv_usec = 0; // Not init'ing this can cause strange errors

    // Initialize CAN socket.
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
        //        log_err( "CAN INIT FAIL %s", strerror( errno ) );
        return;
    }
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof (struct timeval));
    strcpy(ifr.ifr_name, "can0");

    if (ioctl(s, SIOCGIFINDEX, &ifr) == -1) {
        //      log_err( "CAN ioctl() FAIL %s", strerror( errno ) );
        return;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr*) &addr, sizeof ( addr)) == -1) {
        //        log_err( "CAN BIND FAIL %s", strerror( errno ) );
        return;
    }
}

INT32 Can0Read(struct can_frame *frame) {
    INT32 nbytes = read(s, frame, sizeof ( struct can_frame));
    if (nbytes < 0) {
        printf("CAN is No Data\n");
    }
    else if (nbytes < sizeof ( struct can_frame)) {
        printf("INVALID CAN PACKET\n");
    }
    return nbytes;
}

void Can0Write(struct can_frame *frame) {
    INT32 nbytes;
    nbytes = write(s, frame, sizeof ( struct can_frame));
    if (nbytes != sizeof ( struct can_frame))
        printf("CAN0 WRITE ERROR!!!\n");
}

