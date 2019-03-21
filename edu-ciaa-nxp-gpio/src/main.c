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


#define ROWS          4
#define COLS          8
#define HALFWORDS     4
#define BITS          4 

void printAsHex(char hexbuffer[ROWS * COLS * HALFWORDS]) {
  int col;
  int row;
  int halfword;
  for (row = 0; row < ROWS; ++row) {
    for (col = 0; col < COLS; ++col) {
      for (halfword = 0; halfword < HALFWORDS; ++halfword) {
        uartWriteByte(UART_USB, hexbuffer[col * COLS + row * ROWS + halfword * HALFWORDS]);
      }
      uartWriteByte(UART_USB, ' ');
    }
    uartWriteByte(UART_USB,0x0a);
  }
  uartWriteByte(UART_USB,0x0a);
}

void printAsBin(char binbuffer[ROWS * COLS * HALFWORDS * BITS]) {
  int col;
  int row;
  int halfword;
  int bit;
  for (row = 0; row < ROWS; ++row) {
    for (col = 0; col < COLS; ++col) {
      for (halfword = 0; halfword < HALFWORDS; ++halfword) {
        for (bit = 0; bit < BITS; ++bit) {
          uartWriteByte(UART_USB, binbuffer[col * COLS + row * ROWS + halfword * HALFWORDS + bit * BITS]);
        }
      }
      uartWriteByte(UART_USB, ' ');
    }
    uartWriteByte(UART_USB,0x0a);
  }
  uartWriteByte(UART_USB,0x0a);

}

int main(void){

   char dato[] = { 0x0,0x0 };
   char mem;
   char hexbuffer[ROWS * COLS * HALFWORDS];
   char binbuffer[ROWS * COLS * HALFWORDS * BITS];
   char initMsg[] = "Init\n\0";
   int bits;

   boardConfig();

   uartConfig( UART_232, BAUDRATE );

   uartConfig( UART_USB, BAUDRATE );

   gpioConfig( SPI_CS_GPIO0,  GPIO_OUTPUT );
   gpioConfig( SPI_CLK_GPIO2, GPIO_OUTPUT );
   gpioConfig( SPI_DI_GPIO4,  GPIO_OUTPUT );
   gpioConfig( SPI_DO_GPIO6,  GPIO_INPUT );

   uartWriteString(UART_USB, initMsg);

   gpioWrite( SPI_CS_GPIO0, ON);

   while(1) {

      if ( uartReadByte( UART_USB, (uint8_t * )&dato[0] ) ){
         if ( '#' == dato[0]  || '$' == dato[0] ) {


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


            for (bits = 0; bits < 6; ++bits) {
           
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


            for (bits = 0; bits < 21; ++bits) {
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
if ('$' == dato[0] ) {
            gpioWrite( SPI_DI_GPIO4, ON);
} else {
            gpioWrite( SPI_DI_GPIO4, OFF);
}


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
            int col;
            int row;
            int halfword;
            int bits; 
            for (row = 0; row < ROWS; ++row) {
              for (col = 0; col < COLS; ++col) {
                for (halfword = 0; halfword < HALFWORDS; ++halfword) {
                  int idx = col * COLS + row * ROWS + halfword * HALFWORDS;
                  hexbuffer[idx] = 0;
                  for (bits = 0; bits < BITS; ++bits) {
                    mem = gpioRead(SPI_DO_GPIO6); // lectura bit 
                              
                    hexbuffer[idx] *= 2;
                    hexbuffer[idx] += mem != 0 ? 1 : 0;

                    binbuffer[idx + bits] = mem != 0 ? '1' : '0';


                    delay(2);
                    gpioWrite(SPI_CLK_GPIO2, ON );  
                    delay(2);
                    gpioWrite( SPI_CLK_GPIO2, OFF );
                  }
           
                  hexbuffer[idx] += ( (hexbuffer[idx] > 9) ? 55 : 48 );
              
//                  uartWriteByte(UART_USB,hexbuffer[idx]);
//              uartWriteByte(UART_USB, hexbuffer[halfword]);
                }
//              uartWriteByte(UART_USB, ' ');
              }
//              uartWriteByte(UART_USB,0x0a);
            }
            delay(1);
            gpioWrite( SPI_CS_GPIO0, ON);   // para finalizar la instruccion
        }
         //uartWriteByte( UART_232, dato[0] );
        printAsHex(hexbuffer);
//        printAsBin(binbuffer);
      }

      if(  uartReadByte( UART_232, (uint8_t * )&dato[0] ) ){
         uartWriteByte( UART_USB, dato[0] );
      }

  
   }
   return 0 ;
}


