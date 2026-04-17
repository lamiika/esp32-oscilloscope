////////////////////////////////////////////////////////////////////////
// @file main.cpp
// Main program
///////////////////////////// 1.Libraries //////////////////////////////

#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#include "appCore/snake_task.h"
#include "appCore/serial_task.h"
#include "appCore/time_task.h"
#include "appCore/menu_task.h"

#include "graph_task.h"

#define TFT_DISPLAY
#include <esp32-oscilloscope.h>

/////////////////////////////// 2.Macros ///////////////////////////////
/////////////////////////////// 3.Types ////////////////////////////////
//////////////////////////// 4.Declarations ////////////////////////////
//////////////////////////// 4.1.Variables /////////////////////////////
//////////////////////////// 4.2.Functions /////////////////////////////
//////////////////////////// 5.Definitions /////////////////////////////
//////////////////////////// 5.1.Variables /////////////////////////////

TFT_eSPI tft = TFT_eSPI();
SemaphoreHandle_t screen_mutex = xSemaphoreCreateMutex();

//////////////////////////// 5.2.Functions /////////////////////////////

void reset() {
  tft.setTextFont(1); // GLCD, original Adafruit 8x5 font
  tft.fillScreen(TFT_BLACK); // fill screen black background
  tft.setTextSize(TFT_MEDIUM);
  tft.setCursor(0,0); // setCursor(x,y)
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // setTextColor(fg,bg)
}

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(3);

  reset();

  for(int i = 0 ; i < 15 ; i++){
    tft.setCursor(0,i*HEIGHT_MEDIUM);
    tft.print(">");
    tft.setCursor(RESOLUTION_X-WIDTH_MEDIUM,i*HEIGHT_MEDIUM);
    tft.print("<");
  }
  tft.drawCentreString("SETUP BOOTED",RESOLUTION_X/2,RESOLUTION_Y/2,1);
  // ^drawCentreString(string,x,y,font_px_size)

  DELAY(2000);
  xTaskCreate(menu_task,"Menu",1024,NULL,1,NULL);
}

void loop() {
  // Empty, as FreeRTOS is handling everything
}
