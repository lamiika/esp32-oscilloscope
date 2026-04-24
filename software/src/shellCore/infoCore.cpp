////////////////////////////////////////////////////////////////////////
// SPDX-FileCopyrightText: Copyright © 2026, wawa2024. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
/// @file infoCore.cpp
/// @date 2026-04-24
/// @author wawa2024
/// @copyright Copyright © 2026, wawa2024. All rights reserved.
/// @brief FreeRTOS CPU info functions.
///////////////////////////// 1.Libraries //////////////////////////////

// Requires: CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS=y
#include <Arduino.h>
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_heap_caps.h"

#include "esp_err.h"
#include "esp_clk_tree.h"   // public clock-tree API
#include "soc/soc.h"        // for SOC_MOD_CLK_CPU

/////////////////////////////// 2.Macros ///////////////////////////////
/////////////////////////////// 3.Types ////////////////////////////////
//////////////////////////// 4.Declarations ////////////////////////////
//////////////////////////// 4.1.Variables /////////////////////////////
//////////////////////////// 4.2.Functions /////////////////////////////
//////////////////////////// 5.Definitions /////////////////////////////
//////////////////////////// 5.1.Variables /////////////////////////////
//////////////////////////// 5.2.Functions /////////////////////////////

static uint64_t get_cpu_freq_hz(void){
  uint32_t freq_hz = 0;
  esp_err_t err = esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_CPU,
                                               ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT,
                                               &freq_hz);
  if (err != ESP_OK) return 0;
  return freq_hz;
}

String get_cpu_info() {
  esp_chip_info_t chip;
  esp_chip_info(&chip);
  String s;
  s += "Architecture: Xtensa (ESP32)\r\n";
  s += "CPU(s): " + String(chip.cores) + "\r\n";
  s += "Features: ";
  if (chip.features & CHIP_FEATURE_WIFI_BGN) s += "WiFi ";
  if (chip.features & CHIP_FEATURE_BLE) s += "BLE ";
  if (chip.features & CHIP_FEATURE_BT) s += "BT ";
  if (chip.features & CHIP_FEATURE_EMB_FLASH) s += "EmbeddedFlash ";
  s += "\r\n";
  s += "Silicon revision: " + String(chip.revision) + "\r\n";
  s += "CPU Mhz: " + String(get_cpu_freq_hz()/1e6) + "\r\n";
  return s;
}

String get_heap_info() {
  String s;
  s += "Free heap: " + String(esp_get_free_heap_size()) + "\r\n";
  s += "Minimum free heap: " + String(esp_get_minimum_free_heap_size()) + "\r\n";
  s += "DRAM (internal) free: " + String(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)) + "\r\n";
  s += "SPIRAM free (if present): " + String(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)) + "\r\n";
  return s;
}
