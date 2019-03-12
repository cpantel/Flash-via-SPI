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
#define BAUDRATE 115200

#define SPI_CS_GPIO0  GPIO0
#define SPI_CLK_GPIO2 GPIO2
#define SPI_DI_GPIO4  GPIO4
#define SPI_DO_GPIO6  GPIO6
int main(void){

   char dato[] = { 0x0,0x0 };
   char mem;
   char membuffer[]  = { 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

   int bitcounter;
   int bytecounter;

   boardConfig();

   uartConfig( UART_232, BAUDRATE );

   uartConfig( UART_USB, BAUDRATE );

   gpioConfig( SPI_CS_GPIO0,  GPIO_OUTPUT );
   gpioConfig( SPI_CLK_GPIO2, GPIO_OUTPUT );
   gpioConfig( SPI_DI_GPIO4,  GPIO_OUTPUT );
   gpioConfig( SPI_DO_GPIO6,  GPIO_INPUT );

   gpioWrite( SPI_CS_GPIO0, ON);

   while(1) {

      if ( uartReadByte( UART_USB, (uint8_t * )&dato[0] ) ){
         if ( '#' == dato[0] ) {


            // one clk cycle
            delay(1);
            gpioWrite( SPI_CLK_GPIO2, ON );
            delay(2);
            gpioWrite( SPI_CLK_GPIO2, OFF );
            delay(1);


            delay(1);
            gpioWrite( SPI_CLK_GPIO2, ON );
            delay(2);
            gpioWrite( SPI_CLK_GPIO2, OFF );
            delay(1);


            gpioWrite( SPI_CS_GPIO0, OFF);  // para iniciar la instruccion


            for (bitcounter = 0; bitcounter < 6; ++bitcounter) {
           
              gpioWrite( SPI_DI_GPIO4, OFF);
              delay(1);
              gpioWrite( SPI_CLK_GPIO2, ON );  // bit 7-2 del comando read(0x03)
              delay(2);
              gpioWrite( SPI_CLK_GPIO2, OFF );
              delay(1);

            }

            gpioWrite( SPI_DI_GPIO4, ON);
            delay(1);
            gpioWrite( SPI_CLK_GPIO2, ON );  // bit 1 de 0x03
            delay(2);
            gpioWrite( SPI_CLK_GPIO2, OFF );
            delay(1);

            gpioWrite( SPI_DI_GPIO4, ON);
            delay(1);
            gpioWrite( SPI_CLK_GPIO2, ON );  // bit 0 de 0x03
            delay(2);
            gpioWrite( SPI_CLK_GPIO2, OFF );
            delay(1);


            for (bitcounter = 0; bitcounter < 21; ++bitcounter) {
              gpioWrite( SPI_DI_GPIO4, OFF);
              delay(1);
              gpioWrite( SPI_CLK_GPIO2, ON );  // bit 23-3 de la direccion 0x000000000004
              delay(2);
              gpioWrite( SPI_CLK_GPIO2, OFF );
              delay(1);
            }

            gpioWrite( SPI_DI_GPIO4, ON);
            delay(1);
            gpioWrite( SPI_CLK_GPIO2, ON ); // bit 2 de la direccion 0x000000000004
            delay(2);
            gpioWrite( SPI_CLK_GPIO2, OFF );
            delay(1);

            gpioWrite( SPI_DI_GPIO4, OFF);
            delay(1);
            gpioWrite( SPI_CLK_GPIO2, ON ); // bit 2 de la direccion 0x000000000004
            delay(2);
            gpioWrite( SPI_CLK_GPIO2, OFF );
            delay(1);

            gpioWrite( SPI_DI_GPIO4, OFF);
            delay(1);
            gpioWrite( SPI_CLK_GPIO2, ON ); // bit 2 de la direccion 0x000000000004
            delay(2);
            gpioWrite( SPI_CLK_GPIO2, OFF );
//            delay(1);


            for (bytecounter = 0; bytecounter < 7; ++bytecounter) {
              for (bitcounter = 0; bitcounter < 8; ++bitcounter) {
//                mem = gpioRead(SPI_DO_GPIO6); // lectura bit 
//                membuffer[bytecounter] <<= 1;
//                membuffer[bytecounter] |= mem;
                delay(2);
                gpioWrite(SPI_CLK_GPIO2, ON );  
                delay(2);
                gpioWrite( SPI_CLK_GPIO2, OFF );
              }
//              uartWriteByte(UART_USB, membuffer[bytecounter]);
            }


            delay(1);
            gpioWrite( SPI_CS_GPIO0, ON);   // para finalizar la instruccion

         }
         uartWriteByte( UART_232, dato[0] );
      }

      if(  uartReadByte( UART_232, (uint8_t * )&dato[0] ) ){
         uartWriteByte( UART_USB, dato[0] );
      }
   }
   return 0 ;
}

