#include "lv_tab_info.h"
#include <lvgl.h>
#include <stdio.h>

#if LV_USE_OS == LV_OS_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

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

  // Prepare sections for FreeRTOS and LVGL information
  updateFreeRTOSInfo();
  updateLVGLInfo();
}

void LvTabInfo::updateFreeRTOSInfo()
{
    if (mp_FreeRTOSInfoLabel)
    {
#if LV_USE_OS == LV_OS_FREERTOS        
        // Retrieve FreeRTOS information
        auto taskCount = uxTaskGetNumberOfTasks();
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "Tasks: %u\n", taskCount);      

        lv_label_set_text_fmt(mp_FreeRTOSInfoLabel, "%s", buffer);
#endif
    }
}

void LvTabInfo::updateLVGLInfo()
{
    if (mp_LVGLInfoLabel)
    {
        // Retrieve LVGL memory usage information
        lv_mem_monitor_t memMonitor;
        lv_mem_monitor(&memMonitor);

        char buffer[128];
        snprintf(buffer, sizeof(buffer), "Heap Memory:\n Used %u bytes,\n Free %u bytes\n", memMonitor.total_size - memMonitor.free_size, memMonitor.free_size);

        // Append LVGL information to the label
        lv_label_set_text_fmt(mp_LVGLInfoLabel, "%s", buffer);
    }
}


void LvTabInfo::updateMQTTInfo()
{
    // This function can be implemented to retrieve and display MQTT-related information
    // For example, you could display the connection status, broker address, etc.
}
