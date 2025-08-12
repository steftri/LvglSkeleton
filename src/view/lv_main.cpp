#include "lv_main.h"
#include "controller.h"


static const uint8_t SCREEN_BAR_HEIGHT = 32;


extern Controller g_controller;


LvMain::LvMain(void)
{
}



void LvMain::setup(void)
{
  lv_theme_t *p_Theme = lv_theme_default_init(nullptr,  /*Use the DPI, size, etc from this display*/
                                        lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN),   /*Primary and secondary palette*/
                                        false,    /* false = Light or true = dark mode*/
                                        &lv_font_montserrat_18); 

  lv_disp_set_theme(nullptr, p_Theme); /*Assign the theme to the display*/

  lv_obj_t *p_Grid = lv_obj_create(lv_screen_active()); /*Create the screen object, this is needed to use the screen as a parent for other objects*/
  {
    lv_obj_remove_style_all(p_Grid);

    lv_coord_t a_ColumnDesc[] = {lv_pct(100), LV_GRID_TEMPLATE_LAST}; 

    lv_coord_t a_RowDesc[] = {SCREEN_BAR_HEIGHT, lv_pct(100)-SCREEN_BAR_HEIGHT, LV_GRID_TEMPLATE_LAST}; 
    lv_obj_set_grid_dsc_array(p_Grid, a_ColumnDesc, a_RowDesc);
    lv_obj_set_size(p_Grid, lv_pct(100), lv_pct(100)); // Set the grid to fill the screen
    lv_obj_set_layout(p_Grid, LV_LAYOUT_GRID);

    lv_obj_t *p_TitleBar = lv_obj_create(p_Grid);
    {
      lv_obj_set_grid_cell(p_TitleBar, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                                       LV_GRID_ALIGN_STRETCH, 0, 1); //row

      lv_obj_remove_style_all(p_TitleBar);
      lv_obj_set_size(p_TitleBar, lv_pct(100), SCREEN_BAR_HEIGHT);
      lv_obj_set_style_pad_left(p_TitleBar, 10, LV_PART_MAIN);
      lv_obj_set_style_pad_right(p_TitleBar, 10, LV_PART_MAIN);
      lv_obj_set_style_pad_top(p_TitleBar, 6, LV_PART_MAIN);
      lv_obj_set_style_pad_bottom(p_TitleBar, 6, LV_PART_MAIN);
      lv_obj_set_flex_flow(p_TitleBar, LV_FLEX_FLOW_ROW);

      lv_obj_t *p_Title = lv_label_create(p_TitleBar);
      lv_label_set_text(p_Title, APPLICATION_NAME);
      lv_obj_set_flex_grow(p_Title, 1); 

      mp_WifiSymbol = lv_label_create(p_TitleBar);
      lv_obj_align(mp_WifiSymbol, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_label_set_text(mp_WifiSymbol, LV_SYMBOL_WIFI);
      lv_obj_add_flag(mp_WifiSymbol, LV_OBJ_FLAG_HIDDEN);
    }


    mp_TabView = lv_tabview_create(p_Grid);
    {
      lv_obj_set_grid_cell(mp_TabView, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                                  LV_GRID_ALIGN_STRETCH, 1, 1);      //row

      lv_obj_set_size(mp_TabView, lv_pct(100), lv_pct(100));

      lv_obj_t *p_TabInfo = lv_tabview_add_tab(mp_TabView, "Info");
      lv_obj_t *p_TabHistory = lv_tabview_add_tab(mp_TabView, "History");
      lv_obj_t *p_TabSettings = lv_tabview_add_tab(mp_TabView, "Settings");                           

      // change the tab bar position to bottom, change the border style of the buttons
      lv_tabview_set_tab_bar_position(mp_TabView, LV_DIR_BOTTOM);
      lv_obj_t *p_TabButtons = lv_tabview_get_tab_btns(mp_TabView);
      for(uint32_t i = 0; i < lv_tabview_get_tab_count(mp_TabView); i++) 
      {
        lv_obj_t *p_Button = lv_obj_get_child(p_TabButtons, i);
        lv_obj_set_style_border_side(p_Button, LV_BORDER_SIDE_TOP, LV_STATE_CHECKED);
      }

      m_TabSettings.setup(p_TabSettings);
      m_WlanPasswordDialogbox.setup(p_TabSettings);
    }
  }
}


LvTabSettings *LvMain::getTabSettings(void)
{
  return &m_TabSettings;
}


void LvMain::showWlanPasswordDialog(void)
{
  char ac_Ssid[WifiData::MAX_SSID_LENGTH + 1];
  g_controller.getModel()->getData()->getWifiData()->getSelectedNetwork(ac_Ssid, nullptr);
  
  m_WlanPasswordDialogbox.show(ac_Ssid, nullptr);
}


void LvMain::updateWlanSymbol(void)
{
  if(mp_WifiSymbol == nullptr)
    return;

  WifiData::EState e_State = g_controller.getModel()->getData()->getWifiData()->getState();
  if(e_State == WifiData::EState::Connected)
  {
    lv_obj_clear_flag(mp_WifiSymbol, LV_OBJ_FLAG_HIDDEN);
    LV_LOG_USER("Wifi visible");
  }
  else
  {
    lv_obj_add_flag(mp_WifiSymbol, LV_OBJ_FLAG_HIDDEN);
    LV_LOG_USER("Wifi hidden");
  }
}
