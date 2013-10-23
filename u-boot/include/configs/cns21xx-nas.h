/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <gj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

//#define CONFIG_SKIP_LOWLEVEL_INIT

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM920T		1	/* This is an ARM920T Core */
#define CONFIG_CNS21XX          1
#define CONFIG_STAR_EQUULEUS	1	/* on a STAR Equuleus Board */


/* input clock of PLL */
#if 1
#define CONFIG_SYS_CLK_FREQ	100000000 /* the Equuleus has 100MHz input clock */
#else // on FPGA
#define CONFIG_SYS_CLK_FREQ	13000000 /* the Equuleus has 13MHz input clock */
#endif

#define CONFIG_CPU_CLOCK	250
//#define CONFIG_CPU_CLOCK	225
//#define CONFIG_CPU_CLOCK	200
//#define CONFIG_CPU_CLOCK	175

//#define USE_920T_MMU		1
#define CONFIG_USE_IRQ		1
//#undef CONFIG_USE_IRQ			/* we don't need IRQ/FIQ stuff */

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	(4 * 1024 * 1024)

/*
 * Hardware drivers
 */

/* usb */
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_CNS21XX
#define CONFIG_USB_STORAGE
#define CONFIG_FS_FAT
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_DOS_PARTITION
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET


#define CONFIG_CNS21XX_NIC                       /* enable cns21xx NIC driver */
#define CONFIG_CNS21XX_NIC_PHY_INTERNAL          /* internal phy support */
//#define CONFIG_CNS21XX_NIC_PHY_VSC8601           /* VSC8601 phy support */
//#define CONFIG_CNS21XX_NIC_PHY_IP101A
//#define CONFIG_CNS21XX_NIC_PHY_IP1001

//#define CONFIG_MII

#define CONFIG_CNS21XX_SERIAL
/*
 * select serial console configuration
 */
#define CONFIG_CONSOLE_UART_PORT	0	/* we use serial port on Equuleus */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		38400

/***********************************************************
 * Command definition
 ***********************************************************/
#define CONFIG_BOOTP_MASK	(CONFIG_BOOTP_DEFAULT)

#include <config_cmd_default.h>

#define CONFIG_CMD_USB
#define CONFIG_CMD_USB_STORAGE

#define CONFIG_BOOTDELAY	8
/*#define CONFIG_BOOTARGS    	"root=ramfs devfs=mount console=ttySA0,9600" */
#define CONFIG_ETHADDR		00:aa:bb:cc:dd:10
#define CONFIG_NETMASK          255.255.255.0
#define CONFIG_IPADDR		192.168.2.240
#define CONFIG_SERVERIP		192.168.2.5


#define CONFIG_BOOTCOMMAND	"usb start; load usb 0 0x1000000 cns21xx.bin; go 0x1000000"

/*#define CONFIG_BOOTFILE	"elinos-lart" */
/*#define CONFIG_BOOTCOMMAND	"tftp; bootm" */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"cpu_clock=250\0"						\
	""

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP				/* undef to save memory */
#define	CONFIG_SYS_PROMPT		"=> "	/* Monitor Command Prompt */
#define	CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size */
#define	CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS		16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size */

#define CONFIG_MEMTEST_START	0x00100000	/* memtest works on */
#define CONFIG_MEMTEST_END		PHYS_SDRAM_1_SIZE

#undef  CONFIG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define	CONFIG_SYS_LOAD_ADDR		0x00800000	/* default load address	*/

#define	CONFIG_SYS_HZ			1000

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 14400, 19200, 28800, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */

//#define CONFIG_USE_SDRAM			/* use sdram */
#define CONFIG_USE_DDR				/* use ddr ram */


#define RAM_SIZE_16MBIT		(0)
#define RAM_SIZE_64MBIT		(1)
#define RAM_SIZE_128MBIT	(2)
#define RAM_SIZE_256MBIT	(3)
#define RAM_SIZE_512MBIT	(4)


#define RAM_SIZE		RAM_SIZE_256MBIT
#define CONFIG_NR_DRAM_BANKS	1		/* we have 1 bank of DRAM */
#define CONFIG_SYS_SDRAM_BASE		0x00000000	/* SDRAM Bank #1 */
#if (RAM_SIZE == RAM_SIZE_64MBIT)
#define PHYS_SDRAM_1_SIZE	0x00800000	/* 08 MB */
#elif (RAM_SIZE == RAM_SIZE_128MBIT)
#define PHYS_SDRAM_1_SIZE	0x01000000	/* 16 MB */
#elif (RAM_SIZE == RAM_SIZE_256MBIT)
#define PHYS_SDRAM_1_SIZE	0x02000000	/* 32 MB */
#elif (RAM_SIZE == RAM_SIZE_512MBIT)
#define PHYS_SDRAM_1_SIZE	0x04000000	/* 64 MB */
#endif

#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + PHYS_SDRAM_1_SIZE - \
                                GENERATED_GBL_DATA_SIZE)


/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT    (5*CONFIG_HZ)      /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT    (5*CONFIG_HZ)      /* Timeout for Flash Write */



#define CONFIG_SPI_FLASH_SUPPORT
#define CONFIG_ENV_IS_IN_FLASH

#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_CNS21XX_SPI_FLASH

#define CONFIG_SYS_FLASH_BASE	0x30000000
#define CONFIG_SYS_FLASH_BANKS_LIST     { CONFIG_SYS_FLASH_BASE }

#define CONFIG_SYS_MAX_FLASH_SECT	(512)

#define CONFIG_ENV_OFFSET		0x30000
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
#define CONFIG_ENV_SIZE			0x10000

#define CONFIG_SYS_TEXT_BASE	0x0
/*
#define CONFIG_SYS_MONITOR_LEN	(448 * 1024)
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_FLASH_BASE
*/
#endif	/* __CONFIG_H */

