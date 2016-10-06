/*
  Copyright (c) 2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <MKL27Z644>
#include <fsl_device_registers.h>
#include <fsl_device_headers.h>

#include "watchdog.h"


void watchdogEnable ()
{
	//get default config
	cop_config_t config;
	COP_GetDefaultConfig (&config)
	/* This sets the timeout to longest possible*/
	COP_Init (SIM, &config);
}

void watchdogDisable(void)
{
	COP_Disable (SIM);
}

void watchdogReset(void)
{
	COP_Refresh (SIM);
}


extern "C"
void _watchdogDefaultSetup (void)
{
	COP_Disable (SIM);
}
void watchdogSetup (void) __attribute__ ((weak, alias("_watchdogDefaultSetup")));


