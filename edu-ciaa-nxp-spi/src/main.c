/* Copyright 2019, Carlos Pantelides
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "sapi.h"

#define BAUDRATE     115200
#define LEN               4

int main(void){
   char initMsg[] = "Init\n\0";
   char dato[] = { 0x0,0x0 };

   int idx;
   uint8_t command[] = { 0x3, 0x0,0x0,0x0 };
   uint8_t response[LEN];

   boardConfig();

   spiConfig( SPI0 );

   gpioConfig( GPIO0,  GPIO_OUTPUT );



   uartConfig( UART_232, BAUDRATE );

   uartConfig( UART_USB, BAUDRATE );

   uartWriteString(UART_USB, initMsg);

   while(1) {

   gpioWrite( GPIO0, ON);
      if ( uartReadByte( UART_USB, (uint8_t * )&dato[0] ) ){
         if ( '#' == dato[0] ) {
            gpioWrite( GPIO0, OFF);
            spiWrite(SPI0, command,4);
            spiRead(SPI0, response,LEN);

            gpioWrite( GPIO0, ON);

            for (idx= 0; idx < LEN; ++idx) {
              uartWriteByte(UART_USB, response[idx]);

            }
            uartWriteByte(UART_USB,0xa);
            uartWriteByte(UART_USB,0xa);
        }

      }

      if(  uartReadByte( UART_232, (uint8_t * )&dato[0] ) ){
         uartWriteByte( UART_USB, dato[0] );
      }

  
   }
   return 0 ;
}


