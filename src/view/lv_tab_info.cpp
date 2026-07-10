#include <lvgl.h>
#include <stdio.h>

#include "controller.h"
#include "lv_tab_info.h"


extern Controller g_controller;



LvTabInfo::LvTabInfo()
    : mp_FreeRTOSInfoLabel(nullptr)
    , mp_LVGLInfoLabel(nullptr)
{
}

void LvTabInfo::setup(lv_obj_t *p_ParentTab)
{
  lv_obj_set_flex_flow(p_ParentTab, LV_FLEX_FLOW_COLUMN);

  // Create a label to display information
  lv_obj_t *p_FreeRTOSPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_size(p_FreeRTOSPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(p_FreeRTOSPanel, LV_FLEX_FLOW_COLUMN);
    //lv_obj_set_style_pad_all(p_FreeRTOSPanel, 10, LV_PART_MAIN);

    //lv_obj_t *p_FreeRTOSHead = lv_label_create(p_FreeRTOSPanel);
    //lv_label_set_text(p_FreeRTOSHead, "FreeRTOS");
    LV_IMAGE_DECLARE(freertos);
    lv_obj_t * img1 = lv_image_create(p_FreeRTOSPanel);
    lv_image_set_src(img1, &freertos);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);

    mp_FreeRTOSInfoLabel = lv_label_create(p_FreeRTOSPanel);
    lv_label_set_text(mp_FreeRTOSInfoLabel, "FreeRTOS information will be displayed here.");
  }

  lv_obj_t *p_LVGLPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_size(p_LVGLPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(p_LVGLPanel, LV_FLEX_FLOW_COLUMN);
    //lv_obj_set_style_pad_all(p_LVGLPanel, 10, LV_PART

    //lv_obj_t *p_LVGLHead = lv_label_create(p_LVGLPanel);

    LV_IMAGE_DECLARE(lvgl);
    //lv_label_set_text(p_LVGLHead, "LVGL");
    lv_obj_t * img1 = lv_image_create(p_LVGLPanel);
    lv_image_set_src(img1, &lvgl);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    

    mp_LVGLInfoLabel = lv_label_create(p_LVGLPanel);
    lv_label_set_text(mp_LVGLInfoLabel, "LVGL information will be displayed here.");    
  }

  lv_obj_t *p_MQTTPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_size(p_MQTTPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(p_MQTTPanel, LV_FLEX_FLOW_COLUMN);

    LV_IMAGE_DECLARE(mqtt);
    lv_obj_t * img1 = lv_image_create(p_MQTTPanel);
    lv_image_set_src(img1, &mqtt);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);

    // Create a label to display MQTT information
    mp_MQTTInfoLabel = lv_label_create(p_MQTTPanel);
    lv_label_set_text(mp_MQTTInfoLabel, "MQTT information will be displayed here.");
  }

  // Prepare sections for FreeRTOS and LVGL information
  updateFreeRTOSInfo();
  updateLVGLInfo();
  updateMQTTInfo();
}

void LvTabInfo::updateFreeRTOSInfo()
{
  SurveillanceData &r_SurveillanceData = g_controller.getModel().getData().getSurveillanceData();

  char ac_Uptime[64];
  char ac_StringBuffer[384];
  struct tm *p_Timeinfo;
  char ac_TimeBuffer[64]= {0};

  
  time_t currentTime = g_controller.getModel().getData().getWifiData().getTime();
  p_Timeinfo = localtime(&currentTime);
  if(p_Timeinfo == nullptr)
  {
    snprintf(ac_TimeBuffer, sizeof(ac_TimeBuffer), "Error retrieving time (time_t: %ld)", currentTime);
  }
  else
  {
    snprintf(ac_TimeBuffer, sizeof(ac_TimeBuffer), "%04d-%02d-%02d %02d:%02d:%02d", 
             p_Timeinfo->tm_year + 1900, p_Timeinfo->tm_mon + 1, p_Timeinfo->tm_mday,
             p_Timeinfo->tm_hour, p_Timeinfo->tm_min, p_Timeinfo->tm_sec);
  }

  snprintf(ac_Uptime, sizeof(ac_Uptime), "%uh %02um %02us", 
     r_SurveillanceData.getUptime() / 3600, (r_SurveillanceData.getUptime() % 3600) / 60, r_SurveillanceData.getUptime() % 60);

  snprintf(ac_StringBuffer, sizeof(ac_StringBuffer), "%s\nUptime: %s\nTasks: %u\n"
                                                     "Free heap total: %u bytes\n"
                                                     "Minimum ever: %u bytes\n"
                                                     "Free internal heap: %u bytes\n"
                                                     "Total PSRAM memory: %u bytes\n"
                                                     "Free PSRAM memory: %u bytes", 
          ac_TimeBuffer, ac_Uptime, r_SurveillanceData.getTasks(), 
          r_SurveillanceData.getFreeHeapSizeTotal(), r_SurveillanceData.getMinimumEverFreeHeapSize(), 
          r_SurveillanceData.getFreeHeapSizeInternal(), 
#ifdef ESP32
          ESP.getPsramSize(),
#else          
          0,
#endif           
          r_SurveillanceData.getFreePsramSize());      

  lv_label_set_text_fmt(mp_FreeRTOSInfoLabel, "%s", ac_StringBuffer);
}



void LvTabInfo::updateLVGLInfo()
{
  // Retrieve LVGL memory usage information
  lv_mem_monitor_t memMonitor;
  lv_mem_monitor(&memMonitor);

  char ac_StringBuffer[128];
  snprintf(ac_StringBuffer, sizeof(ac_StringBuffer), "Total Heap Memory: %u bytes\n"
                                                     "Free: %u bytes\n"
                                                     "Used: %u%%, fragmentation: %u%%", 
    static_cast<unsigned int>(memMonitor.total_size), 
    static_cast<unsigned int>(memMonitor.free_size),
    static_cast<unsigned int>(memMonitor.used_pct),
    static_cast<unsigned int>(memMonitor.frag_pct));

  lv_label_set_text_fmt(mp_LVGLInfoLabel, "%s", ac_StringBuffer);
}


void LvTabInfo::updateMQTTInfo()
{
  MqttData &r_MqttData = g_controller.getModel().getData().getMqttData();

  char ac_StringBuffer[128];
  snprintf(ac_StringBuffer, sizeof(ac_StringBuffer), "Messages sent: %u\nReceived: %u", r_MqttData.getSentMessageCount(), r_MqttData.getReceivedMessageCount());

  lv_label_set_text_fmt(mp_MQTTInfoLabel, "%s", ac_StringBuffer);
}
