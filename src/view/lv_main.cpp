#include "lv_main.h"

#include "controller.h"


static const uint8_t SCREEN_BAR_HEIGHT = 40;


extern Controller g_controller;


LvMain::LvMain(void)
{
}



void LvMain::setup(void)
{
  lv_theme_t *p_Theme = lv_theme_default_init(nullptr,  /*Use the DPI, size, etc from this display*/
                                        lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN),   /*Primary and secondary palette*/
                                        true,    /* false = Light or true = dark mode*/
                                        &lv_font_montserrat_18); 

  lv_disp_set_theme(nullptr, p_Theme); /*Assign the theme to the display*/

//  Serial.println("Creating main grid...");

  lv_obj_t *p_Grid = lv_obj_create(lv_screen_active()); /*Create the screen object, this is needed to use the screen as a parent for other objects*/
  {
    lv_obj_remove_style_all(p_Grid);

    static const lv_coord_t a_ColumnDesc[] = {lv_pct(100), LV_GRID_TEMPLATE_LAST}; 
    static const lv_coord_t a_RowDesc[] = {SCREEN_BAR_HEIGHT, lv_pct(100)-SCREEN_BAR_HEIGHT, LV_GRID_TEMPLATE_LAST}; 
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
      lv_obj_set_flex_align(p_TitleBar, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

      LV_IMAGE_DECLARE(wifi_icon);
      mp_WifiSymbol = lv_image_create(p_TitleBar);
      lv_image_set_src(mp_WifiSymbol, &wifi_icon);
      lv_obj_set_style_margin_left(mp_WifiSymbol, 8, 0);
      lv_obj_set_style_margin_right(mp_WifiSymbol, 8, 0);
      lv_obj_add_flag(mp_WifiSymbol, LV_OBJ_FLAG_HIDDEN);

      LV_IMAGE_DECLARE(cloud_icon);
      mp_CloudSymbol = lv_image_create(p_TitleBar);
      lv_image_set_src(mp_CloudSymbol, &cloud_icon);
      lv_obj_set_style_margin_left(mp_CloudSymbol, 8, 0);
      lv_obj_set_style_margin_right(mp_CloudSymbol, 8, 0);
      lv_obj_add_flag(mp_CloudSymbol, LV_OBJ_FLAG_HIDDEN);

    }


    mp_TabView = lv_tabview_create(p_Grid);
    {
      lv_obj_set_grid_cell(mp_TabView, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                                  LV_GRID_ALIGN_STRETCH, 1, 1);      //row

      lv_obj_set_size(mp_TabView, lv_pct(100), lv_pct(100));

      static lv_style_t menuTabStyle;
      lv_style_init(&menuTabStyle);

      lv_obj_t *p_TabInfo = lv_tabview_add_tab(mp_TabView, "");
      lv_obj_t *p_TabHistory = lv_tabview_add_tab(mp_TabView, "");
      lv_obj_t *p_TabSettings = lv_tabview_add_tab(mp_TabView, "");                           

      // change the tab bar position to bottom, change the border style of the buttons
      lv_tabview_set_tab_bar_position(mp_TabView, LV_DIR_BOTTOM);
      lv_obj_t *p_TabButtons = lv_tabview_get_tab_btns(mp_TabView);

      // Porsche background image for the tab bar, more height
      lv_obj_set_size(p_TabButtons, lv_pct(100), 96);
      LV_IMAGE_DECLARE(menu_bar_normal);
      lv_style_set_bg_img_src(&menuTabStyle, &menu_bar_normal);
      lv_style_set_bg_color(&menuTabStyle, lv_color_black());
      lv_obj_add_style(p_TabButtons, &menuTabStyle, LV_PART_MAIN);

      for(uint32_t i = 0; i < lv_tabview_get_tab_count(mp_TabView); i++) 
      {
        lv_obj_t *p_Button = lv_obj_get_child(p_TabButtons, i);
        lv_obj_set_style_border_side(p_Button, LV_BORDER_SIDE_TOP, LV_STATE_CHECKED);
      }

      m_TabInfo.setup(p_TabInfo);
      m_TabHistory.setup(p_TabHistory);
      m_TabSettings.setup(p_TabSettings);
    }
  }

  m_Keyboard.setup();

//  Serial.println("LvMain setup completed");
}


void LvMain::showMessageBox(const char *pc_Title, const char *pc_Message)
{
  lv_obj_t *p_MsgBox = lv_msgbox_create(lv_screen_active());
  lv_msgbox_add_title(p_MsgBox, pc_Title);
  lv_msgbox_add_text(p_MsgBox, pc_Message);
  lv_msgbox_add_close_button(p_MsgBox);
  lv_obj_set_size(p_MsgBox, lv_pct(80), LV_SIZE_CONTENT);
  lv_obj_set_style_pad_all(p_MsgBox, 8, LV_PART_MAIN);
  lv_obj_center(p_MsgBox);
}



LvKeyboard *LvMain::getKeyboard(void)
{
  return &m_Keyboard;
}


LvTabInfo *LvMain::getTabInfo(void)
{
  return &m_TabInfo;
}

LvTabHistory *LvMain::getTabHistory(void)
{
  return &m_TabHistory;
}


LvTabSettings *LvMain::getTabSettings(void)
{
  return &m_TabSettings;
}



void LvMain::setWlanSymbol(bool b_Visible)
{
  if(mp_WifiSymbol == nullptr)
    return;

  if(b_Visible)
  {
    lv_obj_clear_flag(mp_WifiSymbol, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_obj_add_flag(mp_WifiSymbol, LV_OBJ_FLAG_HIDDEN);
  }
}


void LvMain::setCloudSymbol(bool b_Visible)
{
  if(mp_CloudSymbol == nullptr)
    return;

  if(b_Visible)
  {
    lv_obj_clear_flag(mp_CloudSymbol, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_obj_add_flag(mp_CloudSymbol, LV_OBJ_FLAG_HIDDEN);
  }
}