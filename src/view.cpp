#include "view.h"



void View::setup(void)
{
  // Initialization is performed inside the UI task (UiTask::setup)
}


void View::begin(void)
{
  Serial.println("Starting UiTask");
  m_UiTask.begin();
}


void View::loop(void)
{
  // Arduino context: The UI updates are handled in the UiTask, 
  // so we can keep this loop empty or use it for other periodic updates if needed.
}


LvMain *View::getLvMain(void)
{
  return m_UiTask.getLvMain();
}


void View::updateSystemInfo(void)
{
  m_UiTask.getLvMain()->getTabInfo()->updateFreeRTOSInfo();
  m_UiTask.getLvMain()->getTabInfo()->updateLVGLInfo();
}


void View::updateWlanState(void)
{
  m_UiTask.getLvMain()->updateWlanSymbol();
  m_UiTask.getLvMain()->getTabSettings()->updateWlanStatePanel();
}


void View::updateWlanList(void)
{
  m_UiTask.getLvMain()->getTabSettings()->updateWlanSelectList();
}


void View::showWlanPasswdDialog(void)
{
  m_UiTask.getLvMain()->showWlanPasswdDialog();
}
