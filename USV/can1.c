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
#include "can1.h"
#include <errno.h>
#include <linux/can.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Arm.h"

INT32 s1;
struct sockaddr_can addr1;
struct ifreq ifr1;
struct timeval tv1;

void Can1_init(void) {

    tv1.tv_sec = 5; // 30 Secs Timeout 
    tv1.tv_usec = 0; // Not init'ing this can cause strange errors

    // Initialize CAN socket.
    if ((s1 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
        //        log_err( "CAN INIT FAIL %s", strerror( errno ) );
        return;
    }
    setsockopt(s1, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv1, sizeof (struct timeval));
    strcpy(ifr1.ifr_name, "can1");

    if (ioctl(s1, SIOCGIFINDEX, &ifr1) == -1) {
        //      log_err( "CAN ioctl() FAIL %s", strerror( errno ) );
        return;
    }

    addr1.can_family = AF_CAN;
    addr1.can_ifindex = ifr1.ifr_ifindex;
    if (bind(s1, (struct sockaddr*) &addr1, sizeof ( addr1)) == -1) {
        //        log_err( "CAN BIND FAIL %s", strerror( errno ) );
        return;
    }
}

INT32 Can1Read(struct can_frame *frame) {
    INT32 nbytes = read(s1, frame, sizeof ( struct can_frame));
    if (nbytes < 0) {
        printf("CAN1 is No Data\n");
    }
    else if (nbytes < sizeof ( struct can_frame)) {
        printf("INVALID CAN PACKET\n");
    }
    return nbytes;
}

void Can1Write(struct can_frame *frame) {
    INT32 nbytes;
    nbytes = write(s1, frame, sizeof ( struct can_frame));
    if (nbytes != sizeof ( struct can_frame))
        printf("CAN1 WRITE ERROR!!!\n");
}

