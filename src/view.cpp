#include <lvgl.h>

#include "view.h"


void View::setup(void)
{
  // Initialize the view components here
  // For example, setting up UI elements, loading resources, etc.

  lv_init();  
  m_ui.setup();
  
  m_LvMain.setup();
}


void View::loop(void)
{
  m_ui.loop();
}


LvMain *View::getLvMain(void)
{
  return &m_LvMain;
}


void View::updateSystemInfo(void)
{
  m_LvMain.getTabInfo()->updateFreeRTOSInfo();
  m_LvMain.getTabInfo()->updateLVGLInfo();
}


void View::updateWlanState(void)
{
  m_LvMain.updateWlanSymbol();
  m_LvMain.getTabSettings()->updateWlanStatePanel();
}


void View::updateWlanList(void)
{
  m_LvMain.getTabSettings()->updateWlanSelectList();
}


void View::showWlanPasswdDialog(void)
{
  m_LvMain.showWlanPasswdDialog();
}
