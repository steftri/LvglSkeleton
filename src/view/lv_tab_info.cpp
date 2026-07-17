#include <lvgl.h>
#include <stdio.h>

#include "lv_tab_info.h"



LvTabInfo::LvTabInfo()
{
}

void LvTabInfo::setup(lv_obj_t *p_ParentTab)
{
  lv_obj_set_flex_flow(p_ParentTab, LV_FLEX_FLOW_COLUMN);

  // Create a label to display information
  lv_obj_t *p_PorschePanel = lv_obj_create(p_ParentTab);
  {     
    lv_obj_remove_style_all(p_PorschePanel);
    lv_obj_set_size(p_PorschePanel, lv_pct(100), LV_SIZE_CONTENT);

    LV_IMAGE_DECLARE(porsche_logo_i4);
    lv_obj_t * img1 = lv_image_create(p_PorschePanel);
    lv_image_set_src(img1, &porsche_logo_i4);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
  }

  lv_obj_t *p_PorscheInfoPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_size(p_PorscheInfoPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_clear_flag(p_PorscheInfoPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(p_PorscheInfoPanel, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * p_InfoLabel = lv_label_create(p_PorscheInfoPanel);
    lv_label_set_text(p_InfoLabel, "The development project for the Porsche\n"
                                   "mobile charger was carried out by BPS GmbH\n"
                                   "in Schorndorf, which was later acquired by\n"
                                   "ERNI (Germany) GmbH.");
  }  


  // Create a label to display information
  lv_obj_t *p_Imx6InfoPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_size(p_Imx6InfoPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_clear_flag(p_Imx6InfoPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(p_Imx6InfoPanel, LV_FLEX_FLOW_COLUMN);

    LV_IMAGE_DECLARE(imx6_logo);
    lv_obj_t * img1 = lv_image_create(p_Imx6InfoPanel);
    lv_image_set_src(img1, &imx6_logo);

    lv_obj_t * p_InfoLabel = lv_label_create(p_Imx6InfoPanel);
    lv_label_set_text(p_InfoLabel, "Embedded Artists iMX6 SoloX COM\n"
                                   "Cortex-A9 @ 1 GHz and Cortex-M4 @ 227 MHz");
  }

  lv_obj_t *p_YoctoInfoPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_size(p_YoctoInfoPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_clear_flag(p_YoctoInfoPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(p_YoctoInfoPanel, LV_FLEX_FLOW_COLUMN);

    LV_IMAGE_DECLARE(yocto_logo);
    lv_obj_t * img1 = lv_image_create(p_YoctoInfoPanel);
    lv_image_set_src(img1, &yocto_logo);    

    lv_obj_t * p_InfoLabel = lv_label_create(p_YoctoInfoPanel);
    lv_label_set_text(p_InfoLabel, "The task was to port an embedded Linux\n"
                                   "system to the target device to control\n"
                                   "the touchscreen.");
  }
}
