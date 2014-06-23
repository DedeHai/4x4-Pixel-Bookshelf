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


#include <stdlib.h>
#define RANDOMIZEALL 0

void setrandomcolors(void)
{
  RGB colortoset;
  colortoset.r = random(256);
  colortoset.g = random(256);
  colortoset.b = random(256);

  for(uint8_t line = 0; line<HEIGHT; line++) //go through all lines (2)
  {
    for(uint8_t column = 0; column<WIDTH; column++) //go through all columns (3x3)
    {
      if(RANDOMIZEALL)
      {
        colortoset.r = random(256);
        colortoset.g = random(256);
        colortoset.b = random(256);
        pixelmatrix.setColor(column, line,colortoset); 
      }
      else
      {
        pixelmatrix.setColor(column, line,colortoset); 
      }
    }
  }
}


void fadecolors(void) 
{

  static byte totalfadesteps[NOOFPIXELS]; //maximum fade steps for each pixel
  static byte currentfadestep[NOOFPIXELS]; //current fadestep for each pixel

  byte j;

  if(modechangerequest)
  {
    modechangerequest = 0;
    for( j = 0; j<NOOFPIXELS; j++)
    {
      totalfadesteps[j] = 200;
      currentfadestep[j] = 0;
      byte col = j%WIDTH;
      byte line = j/WIDTH;
      //now set new endbuffer color
      colorbuffer[col][line][1] = hsv_to_rgb (random(256), 255, random(156)+100); //full color only, can be dark   
      //set startcolor to black
      colorbuffer[col][line][0].r = 0;
      colorbuffer[col][line][0].g = 0;
      colorbuffer[col][line][0].b =0;
    }
  }
  for( j = 0; j<(NOOFPIXELS); j++)
  {
    if( currentfadestep[j] >=  totalfadesteps[j]) //fade is done, set new end color, restet currentfadestep, set new totalfadestep
    {
      currentfadestep[j] = 0;
      totalfadesteps[j] = random(200)+50;
      byte col = j%WIDTH;
      byte line = j/WIDTH;

      copyendbuffertostartbuffer(col, line);
      //now set new endbuffer color
      colorbuffer[col][line][1] = hsv_to_rgb (random(256), 255, random(156)+100); //full color only, can be dark   
    }
  }
  //fade all colors towards endcolor
  for( j = 0; j<NOOFPIXELS; j++)
  {
    RGB result; //resulting color that will be sent to pixel
    currentfadestep[j]++;
    byte col = j%WIDTH;
    byte line = j/WIDTH;
    result = calculatefadetocolor(currentfadestep[j],  totalfadesteps[j],col, line);
    pixelmatrix.setColor(col,  line, result);


  }
}


void fadecolors_rows(void) 
{

  static uint8_t totalfadesteps; //maximum fade steps for each pixel
  static uint8_t currentfadestep; //current fadestep for each pixel

  byte col, line;
  RGB result;

  if(modechangerequest)
  {
    modechangerequest = 0;
    for( line = 0; line<HEIGHT; line++)
    {
      totalfadesteps  = 100;
      currentfadestep  = 0;
      result = hsv_to_rgb (random(256), 255, random(156)+100); //full color only, can be dark;

      for( col = 0; col<WIDTH; col++)
      {
        //now set new endbuffer color
        colorbuffer[col][line][1] = result; //set color
        //set startcolor to black
        colorbuffer[col][line][0].r = 0;
        colorbuffer[col][line][0].g = 0;
        colorbuffer[col][line][0].b =0;
      }
    }
  }
  col=0; 
  line=0;

  if(currentfadestep  >=  totalfadesteps) //fade is done, set new end color, restet currentfadestep, set new totalfadestep
  {
    currentfadestep  = 0;
    totalfadesteps  = 100;
    long timestamp = millis();
    if(slow)
    {
      while(timestamp+60000 > millis())
      {
        if(digitalRead(4) == 0) break;
      }
    }
    for( line = 0; line<HEIGHT; line++)
    {
      result = hsv_to_rgb(random(256), 255, random(156)+100); //full color only, can be dark;
      for( col = 0; col<WIDTH; col++)
      {
        copyendbuffertostartbuffer(col, line);
        //now set new endbuffer color
        colorbuffer[col][line][1] = result; //full color only, can be dark   
      }
    }
  }
  col=0; 
  line=0;
  for( line = 0; line<HEIGHT; line++)
  {
    currentfadestep ++;
    result = calculatefadetocolor(currentfadestep ,  totalfadesteps ,col, line);
    for( col = 0; col<WIDTH; col++)
    {
      pixelmatrix.setColor(col,  line, result);
    }
    col=0; 
  }



}

void fadecolors_columns(void) 
{

  static uint8_t totalfadesteps; //maximum fade steps for each pixel
  static uint8_t currentfadestep; //current fadestep for each pixel

  byte col, line;
  RGB result;

  col=0; 
  line=0;

  if(modechangerequest)
  {
    modechangerequest = 0;
    for( col = 0; col<WIDTH; col++)
    {
      totalfadesteps = 100;
      currentfadestep = 0;
      result = hsv_to_rgb (random(256), 255, random(156)+100); //full color only, can be dark;
      for( line = 0; line<HEIGHT; line++)
      {
        //now set new endbuffer color
        colorbuffer[col][line][1] = result; //set color
        //set startcolor to black
        colorbuffer[col][line][0].r = 0;
        colorbuffer[col][line][0].g = 0;
        colorbuffer[col][line][0].b =0;
      }
    }
  }
  col=0; 
  line=0;

  if( currentfadestep >=  totalfadesteps) //fade is done, set new end color, restet currentfadestep, set new totalfadestep
  {
    currentfadestep = 0;
    totalfadesteps = 250;
    long timestamp = millis();
    if(slow)
    {
      while(timestamp+60000 > millis())
      {
        if(digitalRead(4) == 0) break;
      }
    }
    for( col = 0; col<WIDTH; col++)
    {
      result = hsv_to_rgb(random(256), 255, random(156)+100); //full color only, can be dark;
      for( line = 0; line<HEIGHT; line++)
      {
        copyendbuffertostartbuffer(col, line);
        //now set new endbuffer color
        colorbuffer[col][line][1] = result; //full color only, can be dark   
      }
    }
  }
  col=0; 
  line=0;
  for( col = 0; col<WIDTH; col++)
  {
    currentfadestep++;
    result = calculatefadetocolor(currentfadestep,  totalfadesteps,col, line);
    for(line = 0; line<HEIGHT; line++)
    {
      pixelmatrix.setColor(col,  line, result);
    }
    line=0;
  }


}


void fadeall(void)
{

  static byte totalfadesteps; //maximum fade steps for each pixel
  static byte currentfadestep; //current fadestep for each pixel

  byte col, line,j;
  RGB result;
  result = hsv_to_rgb (random(256), 255, random(156)+100); //full color only, can be dark;
  if(modechangerequest)
  {
    modechangerequest = 0;
    totalfadesteps= 70;
    currentfadestep = 0;
    for( col = 0; col<HEIGHT; col++)
    {

      for( line = 0; line<HEIGHT; line++)
      {

        //now set new endbuffer color
        colorbuffer[col][line][1] = result; //set color
        //set startcolor to black
        colorbuffer[col][line][0].r = 0;
        colorbuffer[col][line][0].g = 0;
        colorbuffer[col][line][0].b =0;
      }
    }
  }
  if( currentfadestep >=  totalfadesteps) //fade is done, set new end color, restet currentfadestep, set new totalfadestep
  {  
    currentfadestep = 0;
    totalfadesteps = random(200)+56;
    result = hsv_to_rgb (random(256), 255, random(156)+100); //full color only, can be dark   
    for( j = 0; j<(NOOFPIXELS); j++)
    {


      byte col = j%WIDTH;
      byte line = j/WIDTH;

      copyendbuffertostartbuffer(col, line);
      //now set new endbuffer color
      colorbuffer[col][line][1] = result;
    }
  }

  currentfadestep++;

  //fade all colors towards endcolor
  for( j = 0; j<NOOFPIXELS; j++)
  {
    byte col = j%WIDTH;
    byte line = j/WIDTH;
    result = calculatefadetocolor(currentfadestep,  totalfadesteps,col, line);
    pixelmatrix.setColor(col,  line, result);
  }

}

void strobo(void)
{
  byte j;
  static byte toggle = 0;

  if(toggle)
  {
    for( j = 0; j<NOOFPIXELS*3; j++)
    {
      pixelmatrix.setByte(j,255); //set white
    } 
    toggle=0;
  }
  else
  {
    for( j = 0; j<NOOFPIXELS*3; j++)
    {
      pixelmatrix.setByte(j,0); //set black
    } 
    toggle = 1;
  }

}

void rainbow(void) 
{

  //initilization is only done once
  byte j;

  if(modechangerequest==1)
  {

    modechangerequest = 0;
    byte increment = 255/random(4,13); //divide hue into 4 to 12 parts parts (=x pixels for full rainbow)
    byte totalfadesteps = 150; //maximum fade steps for fade in
    byte currentfadestep = 0; //current fadestep 

    // initialize the secondary (fade to) buffer: 
    //initialize rainbow (first round)

    for( j = 0; j<NOOFPIXELS; j++)
    {
      colorbuffer[j%WIDTH][j/WIDTH][0].r = increment*(j%WIDTH)+j*4; //initialize hue to same color for same column
      colorbuffer[j%WIDTH][j/WIDTH][0].g = 0; //initialize substep to 0     
    }

    //generate first frame and put it to secondary buffer:
    for( j = 0; j<NOOFPIXELS; j++)
    {
      RGB result; 
      byte col = j%WIDTH;
      byte line = j/WIDTH;

      result = fadeHSV(col, line);
      colorbuffer[col][line][1].r = result.r;
      colorbuffer[col][line][1].g = result.g;
      colorbuffer[col][line][1].b = result.b;
    }

    //reset primary buffer to 0:
    for( j = 0; j<NOOFPIXELS; j++)
    {
      colorbuffer[j%WIDTH][j/WIDTH][0].r = 0;
      colorbuffer[j%WIDTH][j/WIDTH][0].g = 0; 
      colorbuffer[j%WIDTH][j/WIDTH][0].b = 0; 
    }

    //now fade 199 steps towards first frame:
    for( byte i  = 0; i<149; i++)
    {
      //fade all colors towards endcolor
      for(  j = 0; j<NOOFPIXELS; j++)
      {
        RGB result; //resulting color that will be sent to pixel
        byte col = j%WIDTH;
        byte line = j/WIDTH;
        result = calculatefadetocolor(currentfadestep,  totalfadesteps,col, line);
        pixelmatrix.setColor(col,  line, result);
      }
      currentfadestep++;
      pixelmatrix.sendColors();
      _delay_ms(40); 
      if(digitalRead(4) == 0) break;


    }

    //now reset primary rgb buffer for rainbow action so first frame can be created for good this time:
    for( j = 0; j<NOOFPIXELS; j++)
    {
      colorbuffer[j%WIDTH][j/WIDTH][0].r = increment*(j%WIDTH)+j*4; //initialize hue to same color for same column
      colorbuffer[j%WIDTH][j/WIDTH][0].g = 0; //initialize substep to 0     
    }

    //now go on and rainbow away!
  }


  //fade all colors to new hue
  for( j = 0; j<NOOFPIXELS; j++)
  {
    RGB result; //resulting color that will be sent to pixel
    byte col = j%WIDTH;
    byte line = j/WIDTH;

    result = fadeHSV(col, line);
    pixelmatrix.setColor(col,  line, result);

  }

}



























