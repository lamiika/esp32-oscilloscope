////////////////////////////////////////////////////////////////////////
// @file menu_task.cpp
// Menu task
///////////////////////////// 1.Libraries //////////////////////////////

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <device_macros.h>

/////////////////////////////// 2.Macros ///////////////////////////////

#define REFRESH_RATE_MS 1000

/////////////////////////////// 3.Types ////////////////////////////////
//////////////////////////// 4.Declarations ////////////////////////////
//////////////////////////// 4.1.Variables /////////////////////////////

extern TFT_eSPI tft;

//////////////////////////// 4.2.Functions /////////////////////////////
//////////////////////////// 5.Definitions /////////////////////////////
//////////////////////////// 5.1.Variables /////////////////////////////
//////////////////////////// 5.2.Functions /////////////////////////////

void menu_task(void* pvParameter) {
  int i = 0;
  const int color[]= { TFT_RED, TFT_GREEN, TFT_BLUE, };
  const char* msg[] = { "It is windy", "It is rainy", "It is cloudy" };

  while (true) {
    //tft.setCursor(8, 52); // setCursor(x,y)
    //tft.print(__DATE__); // This uses the standard ADAFruit small font
    tft.fillScreen(TFT_BLACK); // Fill screen black background
    tft.setTextColor(color[i], TFT_BLACK); // setTextColor(fg,bg)

    // Old code
    //String str = "Voltage: ";
    //str += String(global.adc.voltage).c_str();
    //Serial.println(str.c_str());

    tft.drawCentreString(__DATE__,120,24,2); // drawCentreString(string,x,y,font_px_size)
    tft.drawCentreString(msg[i],120,48,2); // drawCentreString(string,x,y,font_px_size)
    tft.drawCentreString(str.c_str(),120,72,2); // drawCentreString(string,x,y,font_px_size)

    vTaskDelay(REFRESH_RATE_MS / portTICK_PERIOD_MS); // wait

    i++;
    i = i == 3 ? 0 : i;
  }
}
