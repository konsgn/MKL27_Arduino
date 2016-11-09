/*
  Copyright (c) 2016 Konstantin.  All right reserved.

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

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//   /*
 //   * \brief CUT me
 //   */
//   typedef enum _eAnalogReference
//   {
  //   AR_DEFAULT,
  //   AR_INTERNAL,
  //   AR_EXTERNAL,
  //   AR_INTERNAL1V0,
  //   AR_INTERNAL1V65,
  //   AR_INTERNAL2V23
//   } eAnalogReference ;
#define _8BITRES 0U  
#define _10BITRES 2U   
#define _12BITRES 1U  
#define _16BITRES 3U   

//   /*
 //   * \brief Configures the reference voltage used for analog input (i.e. the value used as the top of the input range).
 //   * This function is kept only for compatibility with existing AVR based API.
 //   *
 //   * \param ulMmode Should be set to AR_DEFAULT.
 //   */
//   extern void analogReference( eAnalogReference ulMode ) ;

/*
 * \brief Reads the value from the specified analog pin.
 *
 * \param ulPin
 *
 * \return Read value from selected pin, if no error.
 */
extern uint32_t analogRead( uint32_t ulPin ) ;

/*
 *  \brief Reads value of speciefied channel without pin handling.
 * 
 *  \param channel
 * 
 *  \return Read value of selected channel
 * 
 */
extern uint16_t analogReadInternal(uint8_t channel) ;

/*
 * \brief Set the resolution of analogRead return values. Default is 10 bits (range from 0 to 1023).
 *
 * \param res
 */
extern void analogReadResolution(int res);

#ifdef __cplusplus
}
#endif
