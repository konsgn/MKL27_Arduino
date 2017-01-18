/*
  Copyright (c) 2016 Konstantin  All right reserved.

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

#include "variant.h"

//   const struct pin_and_config_table digital_pin_to_info_PGM[] = {
	//   {((volatile uint8_t)((PIN_0_PORT)<<5)  + (PIN_0_BIT))},
	//   {((volatile uint8_t)((PIN_1_PORT)<<5)  + (PIN_1_BIT))},
	//   {((volatile uint8_t)((PIN_2_PORT)<<5)  + (PIN_2_BIT))},
	//   {((volatile uint8_t)((PIN_3_PORT)<<5)  + (PIN_3_BIT))},
	//   {((volatile uint8_t)((PIN_4_PORT)<<5)  + (PIN_4_BIT))},
	//   {((volatile uint8_t)((PIN_5_PORT)<<5)  + (PIN_5_BIT))},
	//   {((volatile uint8_t)((PIN_6_PORT)<<5)  + (PIN_6_BIT))},
	//   {((volatile uint8_t)((PIN_7_PORT)<<5)  + (PIN_7_BIT))},
	//   {((volatile uint8_t)((PIN_8_PORT)<<5)  + (PIN_8_BIT))},
	//   {((volatile uint8_t)((PIN_9_PORT)<<5)  + (PIN_9_BIT))},
	//   {((volatile uint8_t)((PIN_10_PORT)<<5) + (PIN_10_BIT))},
	//   {((volatile uint8_t)((PIN_11_PORT)<<5) + (PIN_11_BIT))},
	//   {((volatile uint8_t)((PIN_12_PORT)<<5) + (PIN_12_BIT))},
	//   {((volatile uint8_t)((PIN_13_PORT)<<5) + (PIN_13_BIT))},
	//   {((volatile uint8_t)((PIN_14_PORT)<<5) + (PIN_14_BIT))},
	//   {((volatile uint8_t)((PIN_15_PORT)<<5) + (PIN_15_BIT))},
	//   {((volatile uint8_t)((PIN_16_PORT)<<5) + (PIN_16_BIT))},
	//   {((volatile uint8_t)((PIN_17_PORT)<<5) + (PIN_17_BIT))},
	//   {((volatile uint8_t)((PIN_18_PORT)<<5) + (PIN_18_BIT))},
	//   {((volatile uint8_t)((PIN_19_PORT)<<5) + (PIN_19_BIT))},
	//   {((volatile uint8_t)((PIN_20_PORT)<<5) + (PIN_20_BIT))},
	//   {((volatile uint8_t)((PIN_21_PORT)<<5) + (PIN_21_BIT))}
//   };

//analog channel structure is 0b00ax xxxx
// a = adc mux selection between a and b pins a=0 b=1
// x xxxx = the channel that is used for appropriate pin
// 
//Dont forget that these are referencing for An...up to A0
//   const uint8_t AnalogChannel[6] = {1u,23u,8u,9u,11u,15u}; 
const uint8_t AnalogChannel[6] = {15u,11u,9u,8u,23u,1u};

const struct pin_and_config_table digital_pin_to_info_PGM[] = {
	{(((PIN_0_PORT)<<5)  + (PIN_0_BIT)),  (volatile uint32_t*)PIN_0_CONFIG},
	{(((PIN_1_PORT)<<5)  + (PIN_1_BIT)),  (volatile uint32_t*)PIN_1_CONFIG},
	{(((PIN_2_PORT)<<5)  + (PIN_2_BIT)),  (volatile uint32_t*)PIN_2_CONFIG},
	{(((PIN_3_PORT)<<5)  + (PIN_3_BIT)),  (volatile uint32_t*)PIN_3_CONFIG},
	{(((PIN_4_PORT)<<5)  + (PIN_4_BIT)),  (volatile uint32_t*)PIN_4_CONFIG},
	{(((PIN_5_PORT)<<5)  + (PIN_5_BIT)),  (volatile uint32_t*)PIN_5_CONFIG},
	{(((PIN_6_PORT)<<5)  + (PIN_6_BIT)),  (volatile uint32_t*)PIN_6_CONFIG},
	{(((PIN_7_PORT)<<5)  + (PIN_7_BIT)),  (volatile uint32_t*)PIN_7_CONFIG},
	{(((PIN_8_PORT)<<5)  + (PIN_8_BIT)),  (volatile uint32_t*)PIN_8_CONFIG},
	{(((PIN_9_PORT)<<5)  + (PIN_9_BIT)),  (volatile uint32_t*)PIN_9_CONFIG},
	{(((PIN_10_PORT)<<5) + (PIN_10_BIT)), (volatile uint32_t*)PIN_10_CONFIG},
	{(((PIN_11_PORT)<<5) + (PIN_11_BIT)), (volatile uint32_t*)PIN_11_CONFIG},
	{(((PIN_12_PORT)<<5) + (PIN_12_BIT)), (volatile uint32_t*)PIN_12_CONFIG},
	{(((PIN_13_PORT)<<5) + (PIN_13_BIT)), (volatile uint32_t*)PIN_13_CONFIG},
	{(((PIN_14_PORT)<<5) + (PIN_14_BIT)), (volatile uint32_t*)PIN_14_CONFIG},
	{(((PIN_15_PORT)<<5) + (PIN_15_BIT)), (volatile uint32_t*)PIN_15_CONFIG},
	{(((PIN_16_PORT)<<5) + (PIN_16_BIT)), (volatile uint32_t*)PIN_16_CONFIG},
	{(((PIN_17_PORT)<<5) + (PIN_17_BIT)), (volatile uint32_t*)PIN_17_CONFIG},
	{(((PIN_18_PORT)<<5) + (PIN_18_BIT)), (volatile uint32_t*)PIN_18_CONFIG},
	{(((PIN_19_PORT)<<5) + (PIN_19_BIT)), (volatile uint32_t*)PIN_19_CONFIG},
	{(((PIN_20_PORT)<<5) + (PIN_20_BIT)), (volatile uint32_t*)PIN_20_CONFIG},
	{(((PIN_21_PORT)<<5) + (PIN_21_BIT)), (volatile uint32_t*)PIN_21_CONFIG}
};
