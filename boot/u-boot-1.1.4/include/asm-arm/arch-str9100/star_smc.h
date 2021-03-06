/*******************************************************************************
 *
 *  Copyright(c) 2006 Star Semiconductor Corporation, All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along with
 *  this program; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  The full GNU General Public License is included in this distribution in the
 *  file called LICENSE.
 *
 *  Contact Information:
 *  Technology Support <tech@starsemi.com>
 *  Star Semiconductor 4F, No.1, Chin-Shan 8th St, Hsin-Chu,300 Taiwan, R.O.C
 *
 ******************************************************************************/

#ifndef	_STAR_SMC_H_
#define	_STAR_SMC_H_


#include <asm/arch/star_sys_memory_map.h>


#if defined(__UBOOT__)
#define	SMC_MEM_MAP_VALUE(reg_offset)		(*((u32	volatile *)(SYSPA_SMC_BASE_ADDR	+ reg_offset)))
#elif defined(__LINUX__)
#define	SMC_MEM_MAP_VALUE(reg_offset)		(*((u32	volatile *)(SYSVA_SMC_BASE_ADDR	+ reg_offset)))
#else
#error "NO SYSTEM DEFINED"
#endif


#define	SMC_MEMORY_BANK0_CONFIGURE_REG		SMC_MEM_MAP_VALUE(0x00)
#define	SMC_MEMORY_BANK0_TIMING_PARAM_REG	SMC_MEM_MAP_VALUE(0x04)
#define	SMC_MEMORY_BANK1_CONFIGURE_REG		SMC_MEM_MAP_VALUE(0x08)
#define	SMC_MEMORY_BANK1_TIMING_PARAM_REG	SMC_MEM_MAP_VALUE(0x0C)


/*
 * macros declarations
 */
#define	HAL_SMC_READ_MEMORY_BANK0_CONFIG(memory_bank0_config) \
    ((memory_bank0_config) = (SMC_MEM_BANK0_CONFIG_REG))


#define	HAL_SMC_WRITE_MEMORY_BANK0_CONFIG(memory_bank0_config) \
    ((SMC_MEM_BANK0_CONFIG_REG) = (memory_bank0_config))


#define	HAL_SMC_READ_MEMORY_BANK0_TIMING_PARAMETER(memory_bank0_timing)	\
    ((memory_bank0_timing) = (SMC_MEM_BANK0_TIMING_PARAM_REG))


#define	HAL_SMC_WRITE_MEMORY_BANK0_TIMING_PARAMETER(memory_bank0_timing) \
    ((SMC_MEM_BANK0_TIMING_PARAM_REG) = (memory_bank0_timing))


#define	HAL_SMC_READ_MEMORY_BANK1_CONFIG(memory_bank1_config) \
    ((memory_bank1_config) = (SMC_MEM_BANK1_CONFIG_REG))


#define	HAL_SMC_WRITE_MEMORY_BANK1_CONFIG(memory_bank1_config) \
    ((SMC_MEM_BANK1_CONFIG_REG) = (memory_bank1_config))


#define	HAL_SMC_READ_MEMORY_BANK1_TIMING_PARAMETER(memory_bank1_timing)	\
    ((memory_bank1_timing) = (SMC_MEM_BANK1_TIMING_PARAM_REG))


#define	HAL_SMC_WRITE_MEMORY_BANK1_TIMING_PARAMETER(memory_bank1_timing) \
    ((SMC_MEM_BANK1_TIMING_PARAM_REG) = (memory_bank1_timing))


#define	HAL_SMC_ENABLE_MEMORY_BANK0_WRITE_PROTECT() \
    ((SMC_MEM_BANK0_CONFIG_REG) |= (1 << 11))


#define	HAL_SMC_DISABLE_MEMORY_BANK0_WRITE_PROTECT() \
    ((SMC_MEM_BANK0_CONFIG_REG) &= ~(1 << 11))


#define	HAL_SMC_ENABLE_MEMORY_BANK1_WRITE_PROTECT() \
    ((SMC_MEM_BANK1_CONFIG_REG) |= (1 << 11))


#define	HAL_SMC_DISABLE_MEMORY_BANK1_WRITE_PROTECT() \
    ((SMC_MEM_BANK1_CONFIG_REG) &= ~(1 << 11))


#endif	// end of #ifndef _STAR_SMC_H_
