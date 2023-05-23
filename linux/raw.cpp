// Copyright (c) 2023 Shifei Technologies. All rights reserved.
//
// This file is part of internal projects, unauthorized copying of this file
// via any medium is strictly prohibited.

//
// Created by zheng zhao on 2023/5/11.
//

#include "raw.h"
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <linux/bpf_common.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/filter.h>
#include <assert.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#define OP_AND  (BPF_ALU | BPF_AND | BPF_K)
#define OP_JEQ  (BPF_JMP | BPF_JEQ | BPF_K)
#define OP_JUN  (BPF_JMP | BPF_JA)
#define OP_LDB  (BPF_LD  | BPF_B   | BPF_ABS)
#define OP_LDH  (BPF_LD  | BPF_H   | BPF_ABS)
#define OP_RETK (BPF_RET | BPF_K)
#define RAW_FILTER 5
#define VSC_FILTER 7
#define UDP_FILTER 17
#define VSC_ETHERTYPE 0x8880

static struct sock_filter filter_raw[RAW_FILTER] = {
        { OP_LDH, 0, 0, 12 },
        { OP_JEQ, 1, 0, ETH_P_1588 },
        { OP_JEQ, 0, 1, ETH_P_SLOW },
        { OP_RETK, 0, 0, 1500 },
        { OP_RETK, 0, 0, 0 },
};

static struct sock_filter filter_vsc[VSC_FILTER] = {
        { OP_LDH, 0, 0, 12 },
        { OP_JEQ, 0, 4, VSC_ETHERTYPE },
        { OP_LDH, 0, 0, 44},
        { OP_JEQ, 1, 0, ETH_P_1588 },
        { OP_JEQ, 0, 1, ETH_P_SLOW },
        { OP_RETK, 0, 0, 1500 },
        { OP_RETK, 0, 0, 0 },
};

static struct sock_filter filter_udp[UDP_FILTER] = {
        { OP_LDH, 0, 0, 0x0000000c },
        { OP_JEQ, 0, 4, 0x000086dd },
        { 0x30, 0, 0, 0x00000014 },
        { OP_JEQ, 0, 12, 0x00000011 },
        { OP_LDH, 0, 0, 0x00000038 },
        { 0x35, 8, 10, 0x0000013f },
        { OP_JEQ, 0, 9, 0x00000800 },
        { 0x30, 0, 0, 0x00000017 },
        { OP_JEQ, 0, 7, 0x00000011 },
        { OP_LDH, 0, 0, 0x00000014 },
        { 0x45, 5, 0, 0x00001fff },
        { 0xb1, 0, 0, 0x0000000e },
        { 0x48, 0, 0, 0x00000010 },
        { 0x35, 0, 2, 319 },
        { 0x25, 1, 0, 320 },
        { OP_RETK, 0, 0, 1500 },
        { OP_RETK, 0, 0, 0 },
};
static char ptp_dst_mac[] = "01:1B:19:00:00:00";
//const int32_t socket_priority = 0;
int raw::Open(std::string name, int32_t *fd, filterType type) {
    assert(fd != nullptr);

    *fd = open_socket(name,type);
    if (*fd < 0) {
        return -1;
    }

    return 0;
}

int32_t raw::Close(int fd) {
    return 0;
}

int32_t raw::Send(int fd, uint8_t *buf, uint32_t len) {
    assert(nullptr != buf);

    return send(fd, buf, len, 0);
}

int32_t raw::Receive(int fd, uint8_t *buf, uint32_t buflen) {
    assert(nullptr != buf);

    return recv(fd, buf, buflen, MSG_DONTWAIT);
}

int raw::open_socket(const std::string name, filterType type) {
    struct sockaddr_ll addr;

    struct ifreq ifreq;
    int fd, index;

    if ((fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        printf("open socket fd error ,fd = %d, %d\n",fd, errno);
        return -1;
    }

    memset(&ifreq, 0, sizeof(ifreq));
    strncpy(ifreq.ifr_name, name.c_str(), name.size());
    if (ioctl(fd, SIOCGIFINDEX, &ifreq) < 0) {
        printf("ioctl SIOCGIFINDEX error \n");
        return -1;
    }

    index = ifreq.ifr_ifindex;

    memset(&addr, 0, sizeof(addr));
    addr.sll_ifindex = index;
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr))) {
        printf("bind error \n");
        Close(fd);
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, name.c_str(), strlen(name.c_str()))) {
        printf("set sockopt SO_BINDTODEVICE error \n");
        Close(fd);
        return -1;
    }
/*
    if (socket_priority > 0 &&
        setsockopt(fd, SOL_SOCKET, SO_PRIORITY,&socket_priority,sizeof(socket_priority))) {
        close(fd);
        spdlog::error("[{}][{}] set SO_PRIORITY error",__FUNCTION__ ,__LINE__);
        return -1;
    }
*/
    if (0 != raw_config(fd, type, index)) {
        printf(" raw config error\n");
        return -1;
    }

    return fd;
}

int32_t raw::raw_config(int fd, filterType type, int index) {
    struct packet_mreq mreq;
    struct sock_fprog prg {};
    int bcast = 1;

    if (filterType::PTP_AND_ESMC == type) {
        prg = {RAW_FILTER, filter_raw};
    } else if (filterType::VSC_LONG_PREFIX == type) {
        prg = {VSC_FILTER, filter_vsc};
    } else {
        prg = {UDP_FILTER, filter_udp};
    }

    if (setsockopt(fd, SOL_SOCKET, SO_ATTACH_FILTER, &prg, sizeof(prg))) {
        printf("set SO_ATTACH_FILTER error\n");
        return -1;
    }

    return 0;

}

int main() {
    raw RAW("socket raw");
    int fd = -1;
    if(RAW.Open("eth0", &fd, filterType::VSC_LONG_PREFIX)) {
        return -1;
    }
    struct pollfd Vsc {};
    Vsc.fd = fd;
    Vsc.events = POLLERR | POLLPRI | POLLIN;
    uint8_t buffer[1500];

    while(true) {
        int32_t ret = poll(&Vsc, 1, -1);
        if (ret >0) {
            memset(buffer, 0 ,1500);
            ret = read(fd, buffer, 1500);

            for(int32_t i = 0; i < ret; i++) {
                printf ("%02x ",buffer[i]);
                if ((i+1)%16 == 0) {
                    printf("\n");
                }
            }
            printf("\n");
        }
    }


}