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


//hsv to rgb using floats, is more accurate than using just ints
RGB hsv_to_rgb(float H, unsigned char S, unsigned char V)  //input values are from 0 to 255, hue can be a float
{
 
  float s=(float)S/255.0; 
  float v=(float)V/255.0;
  RGB result; 		
  int i;
  float f, p, q, t;
  if(s == 0 ) //zero saturation, return gray level
  {
    result.r = round(255*v);
    result.g = round(255*v);
    result.b = round(255*v);
    return result;
  }


  i = (int)((float)H/42.5) %6;
  f = (float)H/42.5 - (int)((float)H/42.5);		
  p = v * ( 1.0 - s );
  q = v * ( 1.0 - (s * f));
  t = v * ( 1.0 - (s * ( 1.0 - f )));


  switch( i )
  {
  case 0:
    result.r = round(255*v); 
    result.g = round(255*t); 
    result.b = round(255*p);
    break;

  case 1:
    result.r = round(255*q); 
    result.g = round(255*v); 
    result.b = round(255*p); 
    break;

  case 2:
    result.r = round(255*p); 
    result.g = round(255*v); 
    result.b = round(255*t); 

    break;
    
  case 3:
    result.r = round(255*p); 
    result.g = round(255*q); 
    result.b = round(255*v); 
    break;			
    
  case 4:
    result.r = round(255*t); 
    result.g = round(255*p); 
    result.b = round(255*v); 
    break;
    
  default:								
    result.r = round(255*v); 
    result.g = round(255*p); 
    result.b = round(255*q); 
    break;								

  }

return result;

}





