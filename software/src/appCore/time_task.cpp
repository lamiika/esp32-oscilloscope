////////////////////////////////////////////////////////////////////////
// @file time_task.cpp
// Time task
///////////////////////////// 1.Libraries //////////////////////////////

#include <Arduino.h>

/////////////////////////////// 2.Macros ///////////////////////////////

#define REFRESH_RATE_MS 1000

/////////////////////////////// 3.Types ////////////////////////////////

static uint8_t conv2d(const char* p);

/**
 * @struct Time
 * @brief A structure representing time variables.
 *
 * This struct is used to store time variables.
 */
typedef struct {
  uint8_t hh=conv2d(__TIME__)
    , mm=conv2d(__TIME__+3)
    , ss=conv2d(__TIME__+6)
    ; // Get H, M, S from compile time
} Time ;

//////////////////////////// 4.Declarations ////////////////////////////

Time current_time;

//////////////////////////// 4.1.Variables /////////////////////////////
//////////////////////////// 4.2.Functions /////////////////////////////
//////////////////////////// 5.Definitions /////////////////////////////
//////////////////////////// 5.1.Variables /////////////////////////////
//////////////////////////// 5.2.Functions /////////////////////////////

/**
 * @brief Converts a double digit string to byte value.
 *
 * @param p Constant string value.
 *
 * @return Returns uint8_t value, from 0-99.
 */
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void time_task(void* pvParameter) {
  while (true) {
    current_time.ss++; // increment second
    if ( current_time.ss > 59 ) {
      current_time.ss = 0;
      current_time.mm++; // increment minute
      if( current_time.mm > 59 ) {
        current_time.mm = 0;
        current_time.hh++; // increment hour
        if( current_time.hh > 23 ) {
          current_time.hh = 0;
        }
      }
    }
    vTaskDelay(REFRESH_RATE_MS / portTICK_PERIOD_MS); // wait
  }
}
