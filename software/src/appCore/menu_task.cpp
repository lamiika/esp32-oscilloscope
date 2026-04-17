////////////////////////////////////////////////////////////////////////
// @file menu_task.cpp
// Menu task
///////////////////////////// 1.Libraries //////////////////////////////

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#define TFT_DISPLAY
#include <esp32-oscilloscope.h>

#include <hmiCore.h>
#include "snake_task.h"

/////////////////////////////// 2.Macros ///////////////////////////////

#define REFRESH_RATE_MS 250
#define BUFFER_SIZE 1
#define ITEM_SIZE sizeof(hmiEventData_t)
#define DELAY(X) vTaskDelay(X / portTICK_PERIOD_MS)

/////////////////////////////// 3.Types ////////////////////////////////

typedef struct {
  void (*task)(void*);
  char* title;
  uint32_t stacK_size;
} menu_t;

//////////////////////////// 4.Declarations ////////////////////////////
//////////////////////////// 4.1.Variables /////////////////////////////

extern TFT_eSPI tft;
extern SemaphoreHandle_t screen_mutex;

//////////////////////////// 4.2.Functions /////////////////////////////

extern void reset();
static void info(void*);

//////////////////////////// 5.Definitions /////////////////////////////
//////////////////////////// 5.1.Variables /////////////////////////////

static uint8_t queueStorageArea[BUFFER_SIZE*ITEM_SIZE];
static StaticQueue_t xStaticQueueBuffer;

static menu_t items[] = {
  {info,"About version", 1024}
  ,{info,"Oscilloscope", 1024}
  ,{snake_task,"Snake", 16384}
};
static const size_t items_num = sizeof(items)/sizeof(menu_t);

//////////////////////////// 5.2.Functions /////////////////////////////

static void info(void* pvParameter){
  QueueHandle_t* q = (QueueHandle_t*)pvParameter;
  // attempt to take mutex
  if (xSemaphoreTake(screen_mutex, portMAX_DELAY) == pdTRUE) {
    reset();
    tft.print("Compile time: ");
    tft.println(__DATE__);
    tft.print("Number of items: ");
    tft.println(items_num);
    DELAY(2500); // wait
    xSemaphoreGive(screen_mutex); // release mutex
  }
  xQueueReset(*q);
}

static void draw(uint16_t index) {

  // attempt to take mutex
  if (xSemaphoreTake(screen_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {

    reset();

    for( uint16_t i = 0 ; i < items_num ; i++ ){

      tft.setCursor(0,i*HEIGHT_MEDIUM);

      if( index == i )
        tft.print("> ");
      else
        tft.print("  ");

      tft.setCursor(2*WIDTH_MEDIUM,i*HEIGHT_MEDIUM);
      tft.print(items[i].title);

    }
    xSemaphoreGive(screen_mutex); // release mutex
  }

}

void menu_task(void* pvParameter) {

  QueueHandle_t key_q = hmiCore_init(100,250,100);

  QueueHandle_t q = xQueueCreateStatic(BUFFER_SIZE
                                       ,ITEM_SIZE
                                       ,queueStorageArea
                                       ,&xStaticQueueBuffer);

  hmiEventData_t data;

  uint16_t index = 0;

  TaskHandle_t xHandle = NULL;

  draw(index);

  while (true) {

    uint32_t& inputs = data.inputs;

    inputs = 0;
    while(xQueueReceive(key_q,&data,0) == pdTRUE);

    if ( inputs & BTN_UP ) {

      index = index <= 0 ? (items_num-1) : (index-1) ;

    } else if ( inputs & BTN_DOWN ) {

      index = index >= (items_num-1) ? 0 : (index+1) ;

    } else if ( inputs & BTN_ENTER ) {

      menu_t& item = items[index];
      xTaskCreate(item.task
                  ,item.title
                  ,item.stacK_size
                  ,&q
                  ,1
                  ,&xHandle
                  );

      draw(index);

    }

    if ( inputs & BTN_UP || inputs & BTN_DOWN ) {
      draw(index);
    }

    if(xHandle){
      eTaskState s = eTaskGetState(xHandle);
      if (s != eDeleted) {
        xQueueSend(q,&data,portMAX_DELAY);
      } else {
        xHandle = NULL;
      }
    }

    DELAY(REFRESH_RATE_MS);

  }

  hmiCore_deinit();

}
