////////////////////////////////////////////////////////////////////////
// @file main.cpp
// Main program
///////////////////////////// 1.Libraries //////////////////////////////

#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "graph_task.h"
#include "snake_task.h"
#include "serial_task.h"
#include "blink_task.h"
#include "time_task.h"
#include "menu_task.h"
#include <device_macros.h>

/////////////////////////////// 2.Macros ///////////////////////////////
/////////////////////////////// 3.Types ////////////////////////////////
//////////////////////////// 4.Declarations ////////////////////////////
//////////////////////////// 4.1.Variables /////////////////////////////
//////////////////////////// 4.2.Functions /////////////////////////////
//////////////////////////// 5.Definitions /////////////////////////////
//////////////////////////// 5.1.Variables /////////////////////////////

TFT_eSPI tft = TFT_eSPI(); // invoke lib, pins defined in User_Setup.h

//////////////////////////// 5.2.Functions /////////////////////////////

void freertos() {
  // Create FreeRTOS tasks

  // Example tasks
  //xTaskCreate(blink_task, "Blink Task", 1024, NULL, 1, NULL);
  //xTaskCreate(serial_task, "Serial Task", 1024, NULL, 1, NULL);
  //xTaskCreate(screen_task, "Screen Task", 1024, NULL, 1, NULL);
  //xTaskCreate(clock_task, "Clock Task", 1024, NULL, 1, NULL);
  //xTaskCreate(adc_task, "ADC Task", 1024, NULL, 1, NULL);
  xTaskCreate(snake_task,"Snake Task",16384,NULL,1,NULL);

  // Sensor tasks
  //xTaskCreate(ui_task, "UI", 4096, NULL, 1, NULL);
  //xTaskCreate(sensor_task, "Sensor", 4096, NULL, 1, NULL);
}

void setup() {
  Serial.begin(115200); // Start serial communication
  tft.init(); // Initialize screen
  tft.setRotation(1); // Set screen rotation
  tft.fillScreen(TFT_BLACK); // Fill screen black background
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // setTextColor(fg,bg)
  freertos();
}

void loop() {
  // Empty, as FreeRTOS is handling everything
}
