//  Author:Frankie.Chu 
//  Date:March 9,2012
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/***************************************************************************/
#define ZDEBUG

#define ADJUST_ALL    0x0001
#define ADJUST_GREEN  0x0002
#define ADJUST_RED    0x0003
#define ADJUST_BLUE   0x0004

#include "RGBdriver.h"
#include "Zir.h"
#include "Zchaos.h"
#define CLK 2//pins definitions for the driver        
#define DIO 3
#define STEP 5

RGBdriver Driver(CLK,DIO);

Zir ir(4);
int g_r = 0;
int g_g = 0;
int g_b = 0;
int adjust_state = ADJUST_ALL;
unsigned long adjust_deadline = 0;

bool is_led_on() {
  if (0 == g_r && 0 == g_g && 0 == g_b) return false;
  return true;  
}

void setupLed(int r ,int g, int b) 
{
#ifdef ZDEBUG
  Serial.print("set led to ");
  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.println(b);
#endif
  Driver.begin(); // begin
  Driver.SetColor(b, r, g); //Red. first node data
  Driver.end();
  g_r = r;
  g_g = g;
  g_b = b;
}


void set_adjust_deadline() {
  adjust_deadline = millis() + 5000;
}


void setupLed() 
{
#ifdef ZDEBUG
  Serial.print("set led to ");
  Serial.print(g_r);
  Serial.print(" ");
  Serial.print(g_g);
  Serial.print(" ");
  Serial.println(g_b);
#endif
  Driver.begin(); // begin
  Driver.SetColor(g_b, g_r, g_g); //Red. first node data
  Driver.end();
}


void make_color_lamp_weaker(int *color)
{
  *color = *color - STEP;
  if (*color < 0) *color = 0;
  setupLed();
  set_adjust_deadline();
}


void make_all_lamp_weaker(bool tmp)
{
  g_r = g_r - STEP;
  g_g = g_g - STEP;
  g_b = g_b - STEP;
  if (g_r < 0) g_r = 0;
  if (g_g < 0) g_g = 0;
  if (g_b < 0) g_b = 0;
  setupLed();
}


void make_lamp_weaker(bool tmp)
{
  switch(adjust_state)
  {
    case ADJUST_ALL:
      make_all_lamp_weaker(false);
      break;
    case ADJUST_GREEN:
      make_color_lamp_weaker(&g_g);
      break;
    case ADJUST_RED:
      make_color_lamp_weaker(&g_r);
      break;
    case ADJUST_BLUE:
      make_color_lamp_weaker(&g_b);
     break;
  }  
}


void make_color_lamp_stronger(int *color)
{
  *color = *color + STEP;
  if (*color > 255) *color = 255;
  setupLed();
  set_adjust_deadline();
}


void make_all_lamp_stronger(bool tmp)
{
  g_r = g_r + STEP;
  g_g = g_g + STEP;
  g_b = g_b + STEP;
  if (g_r > 255) g_r = 255;
  if (g_g > 255) g_g = 255;
  if (g_b > 255) g_b = 255;
  setupLed();
}


void make_lamp_stronger(bool tmp)
{
  switch(adjust_state)
  {
    case ADJUST_ALL:
      make_all_lamp_stronger(false);
      break;
    case ADJUST_GREEN:
      make_color_lamp_stronger(&g_g);
      break;
    case ADJUST_RED:
      make_color_lamp_stronger(&g_r);
      break;
    case ADJUST_BLUE:
      make_color_lamp_stronger(&g_b);
     break;
  }  
}




void switch_lamp(bool x) {
  Serial.println(is_led_on());
  if (is_led_on()) setupLed(0,0,0);
  else setupLed(255,255,255);
}


void warn_with_led(int *color) 
{
  int r = g_r;
  int g = g_g;
  int b = g_b;
  g_r = 0;
  g_g = 0;
  g_b = 0;
  for (int i=0; i<4; ++i) {
    *color = 255;
    setupLed();
    delay(100);
    *color = 0;
    setupLed();
    delay(50);
  }
  g_r = r;
  g_g = g;
  g_b = b;
  setupLed();
}


void set_to_red(bool _tmp) 
{
  warn_with_led(&g_r);  
  adjust_state = ADJUST_RED;
  set_adjust_deadline();
}


void set_to_green(bool _tmp) 
{
  warn_with_led(&g_g);  
  adjust_state = ADJUST_GREEN;
  set_adjust_deadline();
}


void set_to_blue(bool _tmp) 
{
  warn_with_led(&g_b);  
  adjust_state = ADJUST_BLUE;
  set_adjust_deadline();
}


void setup()  
{ 
  Serial.begin(9600);
  ir.setup();
  setupLed(0,0,0);

  ir.registerHandler(IR_REMOTE_CODE_POWER, switch_lamp, false);
  ir.registerHandler(IR_REMOTE_CODE_VOL_DOWN, make_lamp_weaker, true);
  ir.registerHandler(IR_REMOTE_CODE_VOL_UP, make_lamp_stronger, true);
  ir.registerHandler(IR_REMOTE_CODE_1, set_to_red, false);
  ir.registerHandler(IR_REMOTE_CODE_2, set_to_green, false);
  ir.registerHandler(IR_REMOTE_CODE_3, set_to_blue, false);

  Serial.println("Start");
}  


void loop()  
{ 
  ir.loop();

  if (0 < adjust_deadline) {
    if (millis() > adjust_deadline) {
#ifdef ZDEBUG
      Serial.println("time is up");
#endif
      adjust_state = ADJUST_ALL;
      adjust_deadline = 0;
    }
    
  }

}
