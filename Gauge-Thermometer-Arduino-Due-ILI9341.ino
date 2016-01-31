/******************************************************************************/
/*                                                                            */
/*  THERMOMETER GAUFE EXAMPLE FOR ARDUINO DUE                                 */
/*                                                                            */
/******************************************************************************/
/*  Copyright (c) 2014  Dr. NCX (mirracle.mxx@gmail.com)                      */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL              */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED              */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR    */
/* BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES      */
/* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,     */
/* WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,     */
/* ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS        */
/* SOFTWARE.                                                                  */
/*                                                                            */
/*  MIT license, all text above must be included in any redistribution.       */
/******************************************************************************/ 


/*----------------------------------------------------------------------------*/
/*  connection of thermistor:                                                 */
/*                                                                            */
/*  GND = --[ 10K ]--  A6 --[ NTC ]-- +3.3V                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*  ILI9341:                                                                  */
/*----------------------------------------------------------------------------*/
/*   8 = RST                                                                  */
/*   9 = D/C                                                                  */
/*  10 = CS                                                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*  VGA:                                                                      */
/*----------------------------------------------------------------------------*/
/*  41 = R --[470R]--     }                                                   */
/*  40 = R --[ 1K ]--     } = VGA 1 (RED)                                     */
/*  39 = R --[ 2K2]--     }                                                   */
/*                                                                            */
/*  38 = G --[470R]--     }                                                   */
/*  37 = G --[ 1K ]--     } = VGA 2 (GREEN)                                   */
/*  36 = G --[ 2K2]--     }                                                   */
/*                                                                            */
/*  35 = B --[390R]--     } = VGA 3 (BLUE)                                    */
/*  34 = B --[820R]--     }                                                   */
/*                                                                            */
/*  43 = Hsync --[ 82R]--  = VGA 13                                           */
/*  42 = Vsync --[ 82R]--  = VGA 14                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/ 

#define USE_VGA  // For VGA leave uncommented
#define USE_ILI  // For ILI9341 leave uncommented

/******************************************************************************/
/*   MAIN THERMISTOR VARIABLES                                                */
/******************************************************************************/

// Thermistor pin
#define THERMISTORPIN A6
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000  //should be 10000ohm but +-15% tolerance
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// how many samples to take and average
#define NUMSAMPLES 10
// The beta coefficient of the thermistor (usually 3000-4000) [B25/50 value]
#define BCOEFFICIENT 4050
// the value of the 'other' resistor //should be 10000ohm
#define SERIESRESISTOR 10000 

/******************************************************************************/
/*   LIBRARIES INCLUDES                                                       */
/******************************************************************************/

#include "sprites.h" // INCLUDEs bitmaps 

// SPI Library
#include <SPI.h>

#ifdef USE_VGA
// VGA library by stimmer, v0.404 (4/4/2013) http://stimmer.github.io/DueVGA/ 
#include "VGA.h"
#endif

#ifdef USE_ILI
// ILI9341_due NEW lib by Marek Buriak http://marekburiak.github.io/ILI9341_due/
#include "ILI9341_due_config.h"
#include "ILI9341_due.h"

// Connection konfiguration of ILI9341 LCD TFT
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10
ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);
#endif 

#define  BLACK   0x0000

/******************************************************************************/
/*   MEASURE TEMPERATURE                                                      */
/******************************************************************************/
 
// measure temperature
float getTemperature(void){
 int i;
 float average;
 int samples[NUMSAMPLES];
 float thermistorResistance;
 
 // acquire N samples
 for (i=0; i< NUMSAMPLES; i++) {
 samples[i] = analogRead(THERMISTORPIN);
 delay(10);
 }
 
 // average all the samples out
 average = 0;
 for (i=0; i< NUMSAMPLES; i++) {
 average += samples[i];
 }
 average /= NUMSAMPLES;
 
 // convert the value to resistance
 thermistorResistance = average * SERIESRESISTOR / (1023 - average);
 
 float steinhart;
 steinhart = thermistorResistance / THERMISTORNOMINAL; // (R/Ro)
 steinhart = log(steinhart); // ln(R/Ro)
 steinhart /= BCOEFFICIENT; // 1/B * ln(R/Ro)
 steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
 steinhart = 1.0 / steinhart; // Invert
 steinhart -= 273.15; // convert to C
 
 // float value
 return(steinhart);
}

/******************************************************************************/
/*   GRRAPHICS FUNCTIONS                                                      */
/******************************************************************************/

void VGAdrawBitmap2bpp (const unsigned char* bitmap, const unsigned char* pal, word x, word y, word w, word h) {
  uint32_t i=0;
  for (word tmpy=0;tmpy<h;tmpy++) {
    for (word tmpx=0;tmpx<w;tmpx++) {
      VGA.drawPixel(x+tmpx, y+tmpy, pal[(unsigned char)bitmap[i] & 0x03 ]);
      tmpx++;
      VGA.drawPixel(x+tmpx, y+tmpy, pal[((((unsigned char)bitmap[i]) & 0x0c) >> 2) ]);
      tmpx++;
      VGA.drawPixel(x+tmpx, y+tmpy, pal[((((unsigned char)bitmap[i]) & 0x30) >> 4) ]);
      tmpx++;
      VGA.drawPixel(x+tmpx, y+tmpy, pal[((((unsigned char)bitmap[i]) & 0xc0) >> 6) ]);
      i++;
    }
  }
}

void tftdrawBitmap2bpp (const unsigned char* bitmap, const word* pal, word x, word y, word w, word h) {
  uint32_t i=0;
  for (word tmpy=0;tmpy<h;tmpy++) {
    for (word tmpx=0;tmpx<w;tmpx++) {
      tft.drawPixel(x+tmpx, y+tmpy, pal[(unsigned char)bitmap[i] & 0x03 ]);
      tmpx++;
      tft.drawPixel(x+tmpx, y+tmpy, pal[((((unsigned char)bitmap[i]) & 0x0c) >> 2) ]);
      tmpx++;
      tft.drawPixel(x+tmpx, y+tmpy, pal[((((unsigned char)bitmap[i]) & 0x30) >> 4) ]);
      tmpx++;
      tft.drawPixel(x+tmpx, y+tmpy, pal[((((unsigned char)bitmap[i]) & 0xc0) >> 6) ]);
      i++;
    }
  }
}

void tftRedrawBitmapSector2bpp (int x0,int y0,int x1,int y1,const unsigned char* bitmap, const word* pal, int x,int y, word w, word h) {
  uint32_t i=0;
  if (x1<x0) { word _x_=x0; x1=x0; x0=_x_;  }
  if (y1<y0) { word _y_=y0; y1=y0; y0=_y_;  }

  if (x0<x) x0=x;
  if (y0<y) y0=x;
  if (x1<x) return;
  if (y1<y) return;
  
  for (word tmpy=y0-2;tmpy<y1+1;tmpy++) {
    for (word tmpx=x0/4-1;tmpx<x1/4+1;tmpx++) {
      tft.drawPixel(tmpx*4+0+x%4, y+tmpy, pal[(unsigned char)bitmap[(tmpx-x/4)+tmpy*(w/4)] & 0x03 ]);
      tft.drawPixel(tmpx*4+1+x%4, y+tmpy, pal[((((unsigned char)bitmap[(tmpx-x/4)+tmpy*(w/4)]) & 0x0c) >> 2) ]);
      tft.drawPixel(tmpx*4+2+x%4, y+tmpy, pal[((((unsigned char)bitmap[(tmpx-x/4)+tmpy*(w/4)]) & 0x30) >> 4) ]);
      tft.drawPixel(tmpx*4+3+x%4, y+tmpy, pal[((((unsigned char)bitmap[(tmpx-x/4)+tmpy*(w/4)]) & 0xc0) >> 6) ]);
    }
  }
}

/******************************************************************************/
/*   GAUGE DRAW FUNCTIONS                                                     */
/******************************************************************************/

void tftGaugeClearNeedle (word x0, word y0, word r, word r0, float ddeg, boolean line=true) {
  float drad = ddeg * 2.0f * 3.142f / 360.0f;
  float x1 = x0 + r * cosf(drad);
  float y1 = y0 + r * sinf(drad);

  float x2 = x0 + r0 * cosf(drad-90);
  float y2 = y0 + r0 * sinf(drad-90);
  float x3 = x0 + r0 * cosf(drad+90);
  float y3 = y0 + r0 * sinf(drad+90);


  word tminX=0;
  word tminY=0;
  word tmaxX=0;
  word tmaxY=0;

  if (x1<x2 && x1<x3) tminX=x1;
  else if (x2<x1 && x2<x3) tminX=x2;
  else if (x3<x1 && x3<x2) tminX=x3;
  if (x1>x2 && x1>x3) tmaxX=x1;
  else if (x2>x1 && x2>x3) tmaxX=x2;
  else if (x3>x1 && x3>x2) tmaxX=x3;

  if (y1<y2 && y1<y3) tminY=y1;
  else if (y2<y1 && y2<y3) tminY=y2;
  else if (y3<y1 && y3<y2) tminY=y3;
  if (y1>y2 && y1>y3) tmaxY=y1;
  else if (y2>y1 && y2>y3) tmaxY=y2;
  else if (y3>y1 && y3>y2) tmaxY=y3;

  tftRedrawBitmapSector2bpp (tminX,tminY-r0,tmaxX,tmaxY,gauge, gauge_pal0,50,10,220,220);
}

void tftGaugeDrawNeedle (word x0, word y0, word r, word r0, float ddeg, boolean line=true) {
  float drad = ddeg * 2.0f * 3.142f / 360.0f;
  float x1 = x0 + r * cosf(drad);
  float y1 = y0 + r * sinf(drad);

  float x2 = x0 + r0 * cosf(drad-90);
  float y2 = y0 + r0 * sinf(drad-90);
  float x3 = x0 + r0 * cosf(drad+90);
  float y3 = y0 + r0 * sinf(drad+90);

  if (line==true) {
    tft.drawLine(x0, y0, x1, y1, C16(255,0,0));
  } else {

  tft.fillTriangle( x1, y1, x2, y2, x3, y3,C16(255,0,0));
  
  tft.drawLine(x2, y2, x1, y1, C16(0,0,0));
  tft.drawLine(x3, y3, x1, y1, C16(0,0,0));

  tft.fillCircle(x0, y0, r0, C16(255,0,0)); 
  tft.drawCircle(x0, y0, r0, C16(0,0,0));   
  }
}

void VGAGaugeDrawNeedle (word x0, word y0, word r, word r0, float ddeg, boolean line=true) {
  float drad = ddeg * 2.0f * 3.142f / 360.0f;
  float x1 = x0 + r * cosf(drad);
  float y1 = y0 + r * sinf(drad);

  float x2 = x0 + r0 * cosf(drad-90);
  float y2 = y0 + r0 * sinf(drad-90);
  float x3 = x0 + r0 * cosf(drad+90);
  float y3 = y0 + r0 * sinf(drad+90);

  if (line==true) {
    VGA.drawLine(x0, y0, x1, y1, C8(255,0,0));
  } else {

  VGA.fillTri( x1, y1, x2, y2, x3, y3,C8(255,0,0));
  
  VGA.drawLine(x2, y2, x1, y1, C8(0,0,0));
  VGA.drawLine(x3, y3, x1, y1, C8(0,0,0));

  VGA.fillCircle(x0, y0, r0, C8(255,0,0)); 
  VGA.drawCircle(x0, y0, r0, C8(0,0,0));   
  }
}

/******************************************************************************/
/*   SETUP                                                                    */
/******************************************************************************/

void setup() {
  Serial.begin(9600);

//------------------------------------------------------------------------------
#ifdef USE_VGA
  // SETUP VGA OUTPUT
  VGA.begin(320,240,VGA_COLOUR);
#endif
//------------------------------------------------------------------------------
#ifdef USE_ILI
  // SETUP TFT LCD
  tft.begin();
  delay(100);
  tft.setRotation(iliRotation270);
  delay(100);
  tft.fillScreen(BLACK);
#endif
//------------------------------------------------------------------------------ 
   
   tftdrawBitmap2bpp(gauge, gauge_pal0,50,10,220,220);
   VGAdrawBitmap2bpp(gauge,gauge_pal1,50,10,220,220);
   
}

/******************************************************************************/
/*   LOOP                                                                     */
/******************************************************************************/

float actVal = 0;  //actual temperature
float stepVal = 0; //actual temperature step
float prevVal = 0; //previous value of step temperature

float points = 0;
float prevpoints = 0;

void AnimateGaugeNeedle(float tempVal) {
   
   prevVal=stepVal;

   if (stepVal>tempVal-2 && stepVal<tempVal+2 ){
      stepVal=tempVal;
   } else if (stepVal>tempVal) {
      for (byte i=3;i>0;i--) {
        if (stepVal>tempVal) stepVal--;
      }  
   } else if (stepVal<tempVal) {
      for (byte i=3;i>0;i--) {
        if (stepVal<tempVal) stepVal++;  
      }  
   } 
   
   prevpoints = (((prevVal + 10) /80) * 268) + 137;
   if (prevpoints>=360) prevpoints=prevpoints-360;

   points = (((stepVal + 10) /80) * 268) + 137;
   if (points>=360) points=points-360;
  
   tftGaugeClearNeedle(160, 120, 85, 8, prevpoints, false);
   VGAdrawBitmap2bpp(gauge,gauge_pal1,50,10,220,220);
   
   VGAGaugeDrawNeedle (160, 120, 85, 8, points, false);
   tftGaugeDrawNeedle (160, 120, 85, 8, points, false); 
}

void loop() {
  
  actVal = getTemperature();
  Serial.println(actVal);  

  AnimateGaugeNeedle(actVal);
  
  delay(1000);
}  
