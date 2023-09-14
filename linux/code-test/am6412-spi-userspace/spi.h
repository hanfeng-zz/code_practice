// Copyright (c) 2023 Shifei Technologies. All rights reserved.
//
// This file is part of internal projects, unauthorized copying of this file
// via any medium is strictly prohibited.

//
// Created by zheng zhao on 2023/9/13.
//

#pragma once

#define SBIT(nr)			((1u) << (nr))

#define readl(addr) ({ unsigned int __v = (*(volatile unsigned int *)(addr)); __v; })

#define writel(b, addr) (void)((*(volatile unsigned int *)(addr)) = (b))

#define GENMASK(h, l) (((~0u) << (l)) & (~0u >> (31 - (h))))

#define SPI3_BASE_ADDR 0x20130000
#define MCSPI_SYSCONFIG_SMARTIDLE (2 << 3)
#define MCSPI_SYSCONFIG_ENAWAKEUP SBIT(2)
#define MCSPI_SYSCONFIG_AUTOIDLE	SBIT(0)
#define MCSPI_SYSCONFIG_SOFTRESET SBIT(1)

#define MCSPI_SYSSTATUS_RESETDONE SBIT(0)

#define MCSPI_MODULCTRL_SINGLE	SBIT(0)
#define MCSPI_MODULCTRL_MS	SBIT(2)
#define MCSPI_MODULCTRL_STEST	SBIT(3)

#define MCSPI_CHCONF_PHA		SBIT(0)
#define MCSPI_CHCONF_POL		SBIT(1)
#define MCSPI_CHCONF_CLKD_MASK	GENMASK(5, 2)
#define MCSPI_CHCONF_EPOL		SBIT(6)
#define MCSPI_CHCONF_WL_MASK	GENMASK(11, 7)
#define MCSPI_CHCONF_TRM_RX_ONLY	SBIT(12)
#define MCSPI_CHCONF_TRM_TX_ONLY	SBIT(13)
#define MCSPI_CHCONF_TRM_MASK	GENMASK(13, 12)
#define MCSPI_CHCONF_DMAW		SBIT(14)
#define MCSPI_CHCONF_DMAR		SBIT(15)
#define MCSPI_CHCONF_DPE0		SBIT(16)
#define MCSPI_CHCONF_DPE1		SBIT(17)
#define MCSPI_CHCONF_IS		SBIT(18)
#define MCSPI_CHCONF_TURBO	SBIT(19)
#define MCSPI_CHCONF_FORCE	SBIT(20)

#define MCSPI_CHSTAT_RXS		SBIT(0)
#define MCSPI_CHSTAT_TXS		SBIT(1)
#define MCSPI_CHSTAT_EOT		SBIT(2)

#define MCSPI_CHCTRL_EN		SBIT(0)
#define MCSPI_CHCTRL_DIS		(0 << 0)

#define MCSPI_WAKEUPENABLE_WKEN	SBIT(0)
#define MCSPI_PINDIR_D0_IN_D1_OUT	0
#define MCSPI_PINDIR_D0_OUT_D1_IN	1

#define MCSPI_MAX_FREQ		48000000
#define SPI_WAIT_TIMEOUT		10
#define MCSPI_REG_OFFSET	0x100
#define MCSPI_CHAN_NB	4


#define SPI_XFER_BEGIN		SBIT(0)	/* Assert CS before transfer */
#define SPI_XFER_END		SBIT(1)	/* Deassert CS after transfer */
#define SPI_XFER_ONCE		(SPI_XFER_BEGIN | SPI_XFER_END)

#define MCSPI_REVISION		0x00
#define MCSPI_SYSSTATUS		0x14
#define MCSPI_IRQSTATUS		0x18
#define MCSPI_IRQENABLE		0x1c
#define MCSPI_WAKEUPENABLE	0x20
#define MCSPI_SYST		0x24
#define MCSPI_MODULCTRL		0x28
#define MCSPI_XFERLEVEL		0x7c

/* per-channel banks, 0x14 bytes each, first is: */
#define MCSPI_CHCONF0		0x2c
#define MCSPI_CHSTAT0		0x30
#define MCSPI_CHCTRL0		0x34
#define MCSPI_TX0			0x38
#define MCSPI_RX0			0x3c


#ifdef __cplusplus
extern "C" {
#endif
int am6412_mcspi3_init();

void am6412_mcspi3_exit();

int am6412_mcspi3_read(unsigned int addr, unsigned int *const value);

int am6412_mcspi3_write(unsigned int addr, unsigned int value);

#ifdef __cplusplus
}
#endif