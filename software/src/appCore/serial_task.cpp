////////////////////////////////////////////////////////////////////////
// SPDX-FileCopyrightText: Copyright © 2026, wawa2024. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
/// @file serial_task.cpp
/// @date 2026-04-19
/// @author wawa2024
/// @brief Serial task for command line interaction with FreeRTOS
///////////////////////////// 1.Libraries //////////////////////////////

#include <esp32-oscilloscope.h>

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

/////////////////////////////// 2.Macros ///////////////////////////////

#define REFRESH_RATE_MS 100
#define WORD_SIZE 16

/////////////////////////////// 3.Types ////////////////////////////////

typedef void(*funcptr)(void);

typedef struct {
  char name[WORD_SIZE];
  funcptr f;
} builtin;

//////////////////////////// 4.Declarations ////////////////////////////
//////////////////////////// 4.1.Variables /////////////////////////////
//////////////////////////// 4.2.Functions /////////////////////////////

static void help();
static void ls();
static void pp();
static void lscpu();
static void kill();
static void reboot();
static void resume();
static void suspend();
static String getline();

//////////////////////////// 5.Definitions /////////////////////////////
//////////////////////////// 5.1.Variables /////////////////////////////

static builtin table[] = {
  { "help", help }
  , { "ls", ls }
  , { "pp" , pp }
  , { "lscpu", lscpu }
  , { "kill", kill }
  , { "reboot", reboot }
  , { "resume", resume }
  , { "suspend", suspend }
};

//////////////////////////// 5.2.Functions /////////////////////////////

static const char* task_state_name(eTaskState state){
  switch (state) {
  case eRunning:   return "Running";
  case eReady:     return "Ready";
  case eBlocked:   return "Blocked";
  case eSuspended: return "Suspended";
  case eDeleted:   return "Deleted";
  case eInvalid:   return "Invalid";
  default:         return "Unknown";
  }
}

static void print_all_tasks_info(void){

  UBaseType_t task_count = uxTaskGetNumberOfTasks();
  if (task_count == 0) {
    Serial.println("No tasks");
    return;
  }

  TaskStatus_t* tasks = (TaskStatus_t*)pvPortMalloc(task_count * sizeof(TaskStatus_t));
  if (tasks == NULL) {
#ifdef DEBUG
    Serial.println("[print_all_tasks_info]: Allocation failed");
#endif
    return;
  }

  // total_run_time is in the same units returned by the run-time counter
  uint32_t total_run_time = 0;
  UBaseType_t returned = uxTaskGetSystemState(tasks, task_count, &total_run_time);

  // Avoid divide-by-zero. If runtime not enabled, ulRunTimeCounter may be zero for all tasks.
  if (total_run_time == 0) total_run_time = 1;

  Serial.printf("%-16s%-16s%-16s%-16s%-16s%-16s\r\n"
                ,"Name"
                ,"State"
                ,"Prio"
                ,"StackHWM"
                ,"RunTime"
                ,"CPU%%"
                );

  for (UBaseType_t i = 0; i < returned; ++i) {
    TaskStatus_t *ts = &tasks[i];
    uint32_t rt = (uint32_t)ts->ulRunTimeCounter;
    float pct = (100.0f * rt) / (float)total_run_time;
    Serial.printf("%-16s%-16s%-16u%-16u%-16lu%-16f\r\n"
                  ,ts->pcTaskName
                  ,task_state_name(ts->eCurrentState)
                  ,(unsigned)ts->uxCurrentPriority
                  ,(unsigned)ts->usStackHighWaterMark
                  ,rt
                  ,pct
                  );
  }
  vPortFree(tasks);
}

TaskHandle_t get_task_by_name(const char* name){

  if (name == NULL) return NULL;

  UBaseType_t n = uxTaskGetNumberOfTasks();
  TaskStatus_t* tasks = (TaskStatus_t*)pvPortMalloc(n * sizeof(TaskStatus_t));
  if (!tasks) return NULL;

  UBaseType_t retrieved = uxTaskGetSystemState(tasks, n, NULL);
  TaskHandle_t handle = NULL;

  for (UBaseType_t i = 0; i < retrieved; ++i) {
    if (tasks[i].pcTaskName && strcmp(tasks[i].pcTaskName, name) == 0) {
      handle = tasks[i].xHandle;
      break;
    }
  }

  vPortFree(tasks);

  return handle;
}

static bool delete_task_by_name(const char* name){
  if (name == NULL) return false;
  TaskHandle_t handle = get_task_by_name(name);
  if (handle == NULL) return false;
  vTaskDelete(handle); // kill task
  return true;
}

static bool suspend_task_by_name(const char* name){
  if (name == NULL) return false;
  TaskHandle_t handle = get_task_by_name(name);
  if (handle == NULL) return false;
  vTaskSuspend(handle); // Suspend task
  return true;
}

static bool resume_task_by_name(const char* name){
  if (name == NULL) return false;
  TaskHandle_t handle = get_task_by_name(name);
  if (handle == NULL) return false;
  vTaskResume(handle); // Resume task
  return true;
}

static void pp(){
  const size_t buf_sz = 2048;
  char buf[buf_sz];
  Serial.printf("%-16s%-8s%-8s%-8s%-8s\r\n"
                ,"Name"
                ,"State"
                ,"Prio"
                ,"Stack"
                ,"#TCB"
                );
  vTaskList(buf);
  Serial.println(buf);
}

static void ls(){
  print_all_tasks_info();
}

static void lscpu(){
}

static void reboot(){
  esp_restart();
}

static void kill(){
  String s = getline();
  if( delete_task_by_name(s.c_str()) ){
    Serial.println("OK '" + s + "'");
  } else {
    Serial.println("? '" + s + "'");
  }
}

static void suspend(){
  String s = getline();
  if( suspend_task_by_name(s.c_str()) ){
    Serial.println("OK '" + s + "'");
  } else {
    Serial.println("? '" + s + "'");
  }
}

static void resume(){
  String s = getline();
  if( resume_task_by_name(s.c_str()) ){
    Serial.println("OK '" + s + "'");
  } else {
    Serial.println("? '" + s + "'");
  }
}

static void help(){
  for(int i = 0 ; i < ( sizeof(table)/sizeof(builtin) ) ; i++) {
    char* name = table[i].name;
    Serial.print(name); Serial.print(" ");
  }
  Serial.println();
}

static funcptr getcmd(String& word){

  for(int i = 0 ; i < ( sizeof(table)/sizeof(builtin) ) ; i++) {

    char* name = table[i].name;

    if( word.equals(name) ) {
      Serial.println("OK '" + word + "'");
      return table[i].f;
    }

  }
  Serial.println("? '" + word + "'");
  return nullptr;
}

static String getline(){

#ifdef DEBUG
  Serial.println("[func_call]: getline");
#endif

  bool flag = true;
  String s = "";
  while(flag){
    if(Serial.available()){
      char c = Serial.read();
      switch(c){
      case '\r':
        Serial.write(c);
        flag = false;
        break;
      case '\n':
        Serial.write(c);
        break;
      case '\b':
        if (!s.isEmpty()) s.remove(s.length() - 1);
        Serial.write(c);
        Serial.write(' ');
        Serial.write(c);
        break;
      default:
        s += c;
        Serial.write(c);
        break;
      }
    }
    DELAY(20);
  }

#ifdef DEBUG
  Serial.println("[func_return]: getline");
#endif

  return s;
}

static void serial_init(){
#ifdef DEBUG
  Serial.println("[serial_task]: launched");
#endif
}

static void serial_deinit(){
#ifdef DEBUG
  Serial.println("[serial_task]: self-deleting");
#endif
  vTaskDelete(NULL); // self-delete
}

void serial_task(void* pvParameter) {

  serial_init();

  if( not mutex_take(serial_mutex) ) {
    serial_deinit();
    return;
  }

  while (true) {

    String line = getline();

    if (line.length()) {

#ifdef DEBUG
      String msg = "[serial_task]: input=" + line;
      Serial.println(msg);
#endif

      funcptr f = getcmd(line);
      if( f != nullptr ) f();
    }

    DELAY(REFRESH_RATE_MS);
  }

  serial_deinit();

}
