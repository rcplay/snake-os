/******************************************************************************
 *
 *  Copyright (c) 2008 Cavium Networks 
 * 
 *  This file is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License, Version 2, as 
 *  published by the Free Software Foundation. 
 *
 *  This file is distributed in the hope that it will be useful, 
 *  but AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or 
 *  NONINFRINGEMENT.  See the GNU General Public License for more details. 
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this file; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA or 
 *  visit http://www.gnu.org/licenses/. 
 *
 *  This file may also be available under a different license from Cavium. 
 *  Contact Cavium Networks for more information
 *
 ******************************************************************************/


#include <common.h>
#include <usb.h>
#include "ehci.h"


#define SYSPA_USB20_CONFIG_BASE_ADDR            0xC8000000
#define SYSPA_USB20_OPERATION_BASE_ADDR         0xCC000000
#define SYSPA_USB20_DEVICE_BASE_ADDR            0xD0000000



int ehci_hcd_init(int index, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{

	ehci_writel(SYSPA_USB20_CONFIG_BASE_ADDR + 0x04, 0x106);
	ehci_writel(SYSPA_USB20_OPERATION_BASE_ADDR + 0x40, (3 << 5) | 0x20000);

	mdelay(100);

	*hccr = (struct ehci_hccr *)(SYSPA_USB20_OPERATION_BASE_ADDR);
	*hcor = (struct ehci_hcor *)((uint32_t) *hccr
			+ HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));


	debug("cns21xx-ehci: init hccr %x and hcor %x hc_length %d\n",
		(uint32_t)*hccr, (uint32_t)*hcor,
		(uint32_t)HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

int ehci_hcd_stop(int index)
{
	return 0;
}
