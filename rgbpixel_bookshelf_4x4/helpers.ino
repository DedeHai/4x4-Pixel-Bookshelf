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

void copyendbuffertostartbuffer(byte c, byte l)
{
  //copy current color to start-colorbuffer
  colorbuffer[c][l][0] =  colorbuffer[c][l][1];   //copy endcolor to new startcolor

} 


RGB calculatefadetocolor( uint8_t fadestep,  uint8_t totalfadesteps,  uint8_t c,  uint8_t l)
{
  RGB returncolor;
  float factor =  (float)(fadestep)/(float)totalfadesteps;
  returncolor.r = (uint8_t)(colorbuffer[c][l][0].r - (factor)*((int)colorbuffer[c][l][0].r - (int)colorbuffer[c][l][1].r));
  returncolor.g = (uint8_t)(colorbuffer[c][l][0].g - (factor)*((int)colorbuffer[c][l][0].g - (int)colorbuffer[c][l][1].g));
  returncolor.b =(uint8_t)(colorbuffer[c][l][0].b - (factor)*((int)colorbuffer[c][l][0].b - (int)colorbuffer[c][l][1].b));  
  return returncolor;
}


void settestpattern(void) //for debugging
{

  RGB color;
  color.r = 50;
  color.b=200;
  color.g = 0;

  pixelmatrix.setColor(0, 0,color);

  color.r = 0;
  color.b=20;
  color.g = 200;

  pixelmatrix.setColor(1, 0,color);

  color.r = 200;
  color.b=10;
  color.g = 20;
  pixelmatrix.setColor(2, 0,color);

}


//function returns new faded RGB value. initialize primary buffer red channel to desired start hue
//only full saturation and full value is returned, used in Rainbow function (this is more of a hack I threw in, but it works :)

RGB fadeHSV(byte column, byte line) //fades through colors in HSV, goes around hsv-circle, returns new rgb value of current pixel for next step
{
  //uses primary rgb buffer for hue fading!

  if(colorbuffer[column][line][0].g>9) //use 10 subdivisions for hue
  {
   colorbuffer[column][line][0].r++; //increase  hue, primary red buffer is abused for hue 
   colorbuffer[column][line][0].g=0; //reset hue substep, primary green buffer is abused for hue sub steps
  }
    
   RGB result = hsv_to_rgb (  (float)colorbuffer[column][line][0].r + (float)colorbuffer[column][line][0].g/10, 255, 255);    
 
  colorbuffer[column][line][0].g++;//increase sub step
  
  return result;
}



//sendByte is also defined in the library but it is a private function 
//so here is a copy, did not want to mess with the library.
inline void sendByte(uint8_t data) //sends one byte to the LEDs 
{
  uint8_t bitmask = 0b10000000;

  for(uint8_t cnt = 8; cnt; --cnt)
  { 
    if(data & bitmask)  //write a "1"
    {   
      PORTD |= 0b10000000; //pin7 high
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      PORTD &= 0b01111111; //pin7 low

    }
    else  //write a "0"
    {
      PORTD |= 0b10000000; //pin7 high
      asm volatile ("nop");
      PORTD &= 0b01111111; //pin7 low

    }
    bitmask = bitmask>>1;

  }
}


void  sendColors(void) //function is blocking until send is done, also blocks interrupts
{

  uint8_t index =0 ;
  uint8_t counter = 0;
  uint8_t databyte = 0;
  uint8_t localcolorarray[WIDTH*HEIGHT*3];
  RGB pixelcolor;


  cli();//globally disable interrupts, we do not want to be disturbed or timing will not work
  for(index = 0; index<WIDTH*HEIGHT*3; index+=3)
  { 
    for(counter = 0; counter < LEDPERPIXEL; counter++)
    {
      sendByte(pixelmatrix.getByte(index));
      sendByte(pixelmatrix.getByte(index+1));
      sendByte(pixelmatrix.getByte(index+2));
    }
  }


  sei();
  //_delay_us(50); //delay is not necessary, it can be assumed the function is not called in very fast succession

}



