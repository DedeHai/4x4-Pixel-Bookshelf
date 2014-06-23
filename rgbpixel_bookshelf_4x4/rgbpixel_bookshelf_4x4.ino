/**
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */


/*
4x4 matrix demo for 5LED per pixel bookshelf (partially) using the pixelmatrix library
 */

#include "RGBpixelmatrix.h"
#include <avr/delay.h>

#define WIDTH 4 //matrix has 4 columns
#define HEIGHT  4 //and 4 lines
#define LEDPERPIXEL 5 //number of leds per pixel
#define NOOFPIXELS WIDTH*HEIGHT
#define F_CPU 16000000UL 

RGBpixelmatrix pixelmatrix(WIDTH, HEIGHT); //create a RGBpixelmatrix, output on pin7 (defined in sendByte function and initialized in setup() )

RGB colorbuffer[WIDTH][HEIGHT][2]; //backup buffer for fading color manipulation, (startcolor, endcolor)
#define BAUDRATE 115200

//Global variables
byte mode =1;
byte initializerainbow = 1; 

void setup()
{
  Serial.begin(BAUDRATE);
  pinMode(7, OUTPUT);
  randomSeed(analogRead(A0));
  mode = 3;
  pinMode(4,INPUT);
  digitalWrite(4, HIGH); //pullup
  Serial.println("setup done");

}                    

byte modechangerequest = 1;
byte slow = 1;

void loop()
{

  switch (mode) {
  case 0:
    fadecolors(); //fade each to random color at random speed
    _delay_ms(60);
    break;
  case 1:
    rainbow(); //rainbow pattern
    _delay_ms(8);
    break;
  case 2:
    fadecolorsHSV(); //fade all colors to same random color
    _delay_ms(25);
    break;
  case 3:
    slow=1;
    fadecolors_rows(); //fade each row to random color at random speed
    _delay_ms(10);
    break;
  case 4:
    slow=1;
    fadecolors_columns(); //fade each column to random color at random speed
    _delay_ms(10);
    break;
  case 5:
    fadeall(); //fade all to same color
    _delay_ms(50);
    break;
  case 6:
    slow=0;
    fadecolors_rows(); //fade each row to random color at random speed
    _delay_ms(40);
    break;
  default:
    mode = 0;
    break;
  }

  sendColors();
  
  //Button is on pin4 and pulls it to GND

  static uint8_t oneshot = 0; //oneshot to do this only once on each button press

  if(digitalRead(4) == 0 && oneshot == 0)
  {
    oneshot = 1;
    initializerainbow = 1; 
    modechangerequest = 1;
    mode++;
  }
  else if(digitalRead(4) == 1 && oneshot == 1)
  {
    oneshot = 0;
  }



}




