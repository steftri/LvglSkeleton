#include "view.h"



void View::setup(void)
{
  // Initialization is performed inside the UI task (UiTask::setup)
}


void View::begin(void)
{
  Serial.println("Starting UiTask");
  m_Task.begin();
}


void View::loop(void)
{
  // Arduino context: The UI updates are handled in the UiTask, 
  // so we can keep this loop empty or use it for other periodic updates if needed.
}


LvMain *View::getLvMain(void)
{
  return m_Task.getLvMain();
}


void View::updateSystemInfo(void)
{
  m_Task.getLvMain()->getTabInfo()->updateFreeRTOSInfo();
  m_Task.getLvMain()->getTabInfo()->updateLVGLInfo();
}


void View::updateWlanState(void)
{
  m_Task.getLvMain()->updateWlanSymbol();
  m_Task.getLvMain()->getTabSettings()->updateWlanStatePanel();
}


void View::updateWlanList(void)
{
  m_Task.getLvMain()->getTabSettings()->updateWlanSelectList();
}


void View::showWlanPasswdDialog(void)
{
  m_Task.getLvMain()->showWlanPasswdDialog();
}
