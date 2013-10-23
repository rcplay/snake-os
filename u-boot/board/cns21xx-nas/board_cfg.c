/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
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

#include <common.h>
/*
 * Miscellaneous platform dependent initialisations
 */

extern int cns21xx_nic_initialize();
extern void flash_disable_direct_access(void);
extern void str8100_cpu_clock_show(void);

int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	/* arch number of STR81XX */
	gd->bd->bi_arch_number = MACH_TYPE_CNS21XX;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x00000100;

	
	return 0;
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->ram_size = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE, PHYS_SDRAM_1_SIZE);
	return 0;
}

int board_eth_init(bd_t *bis)
{
	return cns21xx_nic_initialize();
}

unsigned long do_go_exec(ulong (*entry)(int, char * const []), int argc,
				 char * const argv[])
{
	/* disable direct flash access before launching app */
	flash_disable_direct_access();

	str8100_cpu_clock_scale_start();

	return entry (argc, argv);
}
