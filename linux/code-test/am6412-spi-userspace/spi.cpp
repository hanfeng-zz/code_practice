// Copyright (c) 2023 Shifei Technologies. All rights reserved.
//
// This file is part of internal projects, unauthorized copying of this file
// via any medium is strictly prohibited.

// Copyright (c) 2023 Shifei Technologies. All rights reserved.
//
// This file is part of internal projects, unauthorized copying of this file
// via any medium is strictly prohibited.

//
// Created by zheng zhao on 2023/9/13.
//
#pragma once

#include <koopa/config.h>
#include "log.h"
#include "shared_mutex.h"
#include "koopa/config.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <cassert>

#define AM6412_SPI_READ_CNT 100
#define VSC7512_SPI_NR_BYTES     7                 /* Number of bytes to transmit or receive */
#define VSC7512_SPI_PADDING_MAX 15                 /* Maximum number of optional padding bytes */
#define VSC_TO_SPI(_a_)     (_a_ & 0x007FFFFF) /* 23 bit SPI address */

static volatile unsigned char *spi_reg_base = nullptr;
static volatile unsigned char *spi3_reg_base = nullptr;

static int spi3_fd = -1;
static long page_size;
static shared_mutex_t am6412_mutex;
static unsigned int mcspi_read_reg(int idx) {
    unsigned int val = readl(spi3_reg_base + idx);
    return val;
}

static void mcspi_write_reg(int idx, int val) {
    writel(val, spi3_reg_base + idx);
    readl(spi3_reg_base + idx);
}

static void mcspi_write_chconf(int val) {
    writel(val, spi3_reg_base + MCSPI_CHCONF0);
    readl(spi3_reg_base + MCSPI_CHCONF0);
}

static void mcspi_set_enable(int enable) {
    writel(enable, spi3_reg_base + MCSPI_CHCTRL0);
    readl(spi3_reg_base + MCSPI_CHCTRL0);
}

static int mcspi_write(const void *txp, unsigned int len, unsigned long flags) {
    assert(nullptr != txp);

    int i, chconf, cnt;

    chconf = readl(spi3_reg_base + MCSPI_CHCONF0);
    /* Enable the channel */
    mcspi_set_enable(MCSPI_CHCTRL_EN);

    chconf &= ~(MCSPI_CHCONF_TRM_MASK | MCSPI_CHCONF_WL_MASK);
    chconf |= (8 - 1) << 7; // default 8 bit
    chconf |= MCSPI_CHCONF_TRM_TX_ONLY;
    chconf |= MCSPI_CHCONF_FORCE;

    mcspi_write_chconf(chconf);

    for (i = 0; i < len; i++) {
        cnt = 0;
        while (!(mcspi_read_reg(MCSPI_CHSTAT0) & MCSPI_CHSTAT_TXS)) {
            cnt++;
            if (cnt > AM6412_SPI_READ_CNT) {
                log_err("[%s][%d] spi tx error\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }
        /* Write the data */
        volatile unsigned int *tx = (volatile unsigned int *)(spi3_reg_base + MCSPI_TX0);
        writel(((unsigned char *)txp)[i], tx);
    }

    /* wait to finish of transfer */
    while ((mcspi_read_reg(MCSPI_CHSTAT0) & (MCSPI_CHSTAT_EOT | MCSPI_CHSTAT_TXS)) !=
           (MCSPI_CHSTAT_EOT | MCSPI_CHSTAT_TXS)) {}

    /* Disable the channel otherwise the next immediate RX will get affected */
    mcspi_set_enable(MCSPI_CHCTRL_DIS);

    if (flags & SPI_XFER_END) {
        chconf &= ~MCSPI_CHCONF_FORCE;
        mcspi_write_chconf(chconf);
    }
    
    return 0;
}

static int mcspi_read(void *rxp, unsigned int len, unsigned long flags)
{
    assert(nullptr != rxp);

    int i, chconf, cnt;

    chconf = readl(spi3_reg_base + MCSPI_CHCONF0);

    /* Enable the channel */
    mcspi_set_enable(MCSPI_CHCTRL_EN);

    chconf &= ~(MCSPI_CHCONF_TRM_MASK | MCSPI_CHCONF_WL_MASK);
    chconf |= (8 - 1) << 7;
    chconf |= MCSPI_CHCONF_TRM_RX_ONLY;
    chconf |= MCSPI_CHCONF_FORCE;
    mcspi_write_chconf(chconf);

    writel(0, spi3_reg_base + MCSPI_TX0);

    for (i = 0; i < len; i++) {
        /* Wait till RX register contains data (RXS == 1) */
        cnt = 0;
        while (!(mcspi_read_reg(MCSPI_CHSTAT0) & MCSPI_CHSTAT_RXS)) {
            cnt++;
            if (cnt > AM6412_SPI_READ_CNT) {
                log_err("[%s][%d] spi read error\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }

        /* Disable the channel to prevent further receiving */
        if (i == (len - 1)) {
            mcspi_set_enable(MCSPI_CHCTRL_DIS);
        }

        /* Read the data */
        volatile unsigned int *rx = (volatile unsigned int *)(spi3_reg_base + MCSPI_RX0);
        ((unsigned char *)rxp)[i] = (unsigned char)readl(rx);
    }

    if (flags & SPI_XFER_END) {
        chconf &= ~MCSPI_CHCONF_FORCE;
        mcspi_write_chconf(chconf);
    }

    return 0;
}

/*McSPI Transmit Receive Mode*/
static int mcspi_txrx(unsigned int len, const void *txp, void *rxp, unsigned long flags)
{
    assert(nullptr != txp);
    assert(nullptr != rxp);

    int chconf, i, cnt;

    chconf = readl(spi3_reg_base + MCSPI_CHCONF0);

    /*Enable SPI channel*/
    mcspi_set_enable(MCSPI_CHCTRL_EN);

    /*set TRANSMIT-RECEIVE Mode*/
    chconf &= ~(MCSPI_CHCONF_TRM_MASK | MCSPI_CHCONF_WL_MASK);
    chconf |= (8 - 1) << 7;
    chconf |= MCSPI_CHCONF_FORCE;
    mcspi_write_chconf(chconf);

    /*Shift in and out 1 byte at time*/
    for (i=0; i < len; i++){
        /* Write: wait for TX empty (TXS == 1)*/
        cnt = 0;
        while (!(mcspi_read_reg(MCSPI_CHSTAT0) & MCSPI_CHSTAT_TXS)) {
            cnt++;
            if (cnt > 100) {
                log_err("[%s][%d] spi tx error\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }
        /* Write the data */
        volatile unsigned int *tx = (volatile unsigned int *)(spi3_reg_base + MCSPI_TX0);
        writel(((unsigned char *)txp)[i], tx);

        /*Read: wait for RX containing data (RXS == 1)*/
        cnt=0;
        while (!(mcspi_read_reg(MCSPI_CHSTAT0) & MCSPI_CHSTAT_RXS)) {
            cnt++;
            if (cnt > 100) {
                log_err("[%s][%d] spi rx error\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }
        /* Read the data */
        volatile unsigned int *rx = (volatile unsigned int *)(spi3_reg_base + MCSPI_RX0);
        ((unsigned char *)rxp)[i] = (unsigned char)readl(rx);
    }
    /* Disable the channel */
    mcspi_set_enable(MCSPI_CHCTRL_DIS);

    /*if transfer must be terminated disable the channel*/
    if (flags & SPI_XFER_END) {
        chconf &= ~MCSPI_CHCONF_FORCE;
        mcspi_write_chconf(chconf);
    }

    return 0;
}

int am6412_mcspi3_init() {

    page_size = sysconf(_SC_PAGESIZE);

    spi3_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (spi3_fd < 0) {
        log_err("[%s][%d] open /dev/mem error\n", __FUNCTION__, __LINE__);
        return -1;
    }

    spi_reg_base = (volatile unsigned char *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, spi3_fd, SPI3_BASE_ADDR);
    if (NULL == spi_reg_base) {
        log_err("[%s][%d] get spi 3 physical address error\n", __FUNCTION__, __LINE__);
        return -1;
    }

    spi3_reg_base = spi_reg_base + MCSPI_REG_OFFSET;

    am6412_mutex = shared_mutex_init("am6412_SPI");
    return 0;
}

void am6412_mcspi3_exit() {

    if (spi3_fd >= 0) {
        close(spi3_fd);
    }

    if (nullptr != spi_reg_base) {
        munmap((void *) spi_reg_base, page_size);
        spi3_reg_base = nullptr;
        spi_reg_base = nullptr;
    }

}

int am6412_mcspi3_read(unsigned int addr, unsigned int *const value) {
    assert(nullptr != value);
    shared_mutex_lock(&am6412_mutex);
    unsigned char tx[VSC7512_SPI_NR_BYTES + VSC7512_SPI_PADDING_MAX] = {0};
    unsigned char rx[sizeof(tx)] = { 0 };
    unsigned int siaddr = VSC_TO_SPI(addr);

    memset(tx, 0xff, sizeof(tx));
    tx[0] = (unsigned char)(siaddr >> 16);
    tx[1] = (unsigned char)(siaddr >> 8);
    tx[2] = (unsigned char)(siaddr >> 0);

    if (mcspi_txrx(VSC7512_SPI_NR_BYTES + VSC7512_SPI_PAD, tx, rx, SPI_XFER_ONCE)) {
        return -1;
    }

    unsigned int rx_word = (rx[3 + VSC7512_SPI_PAD] << 24) |
                           (rx[4 + VSC7512_SPI_PAD] << 16) |
                           (rx[5 + VSC7512_SPI_PAD] << 8) |
                           (rx[6 + VSC7512_SPI_PAD] << 0);
    *value = rx_word;
    shared_mutex_unlock(&am6412_mutex);
    return 0;

}

int am6412_mcspi3_write(unsigned int addr, unsigned int value) {
    shared_mutex_lock(&am6412_mutex);
    unsigned char tx[VSC7512_SPI_NR_BYTES] = { 0 };
    unsigned int siaddr = VSC_TO_SPI(addr);

    tx[0] = (unsigned char)(0x80 | (siaddr >> 16));
    tx[1] = (unsigned char)(siaddr >> 8);
    tx[2] = (unsigned char)(siaddr >> 0);
    tx[3] = (unsigned char)(value >> 24);
    tx[4] = (unsigned char)(value >> 16);
    tx[5] = (unsigned char)(value >> 8);
    tx[6] = (unsigned char)(value >> 0);

    int ret = mcspi_write(tx, VSC7512_SPI_NR_BYTES, SPI_XFER_ONCE);
    shared_mutex_unlock(&am6412_mutex);
    return ret;
}