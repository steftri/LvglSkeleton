#include <iostream>
#include <inttypes.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "model.h"
#include "view.h"
#include "controller.h"


static Model g_model;
static View g_view;
Controller g_controller(g_model, g_view);


int main(int argc, char *argv[])
{
    std::cout << APPLICATION_NAME << std::endl;
    std::cout << __DATE__ " " __TIME__ << std::endl;

    g_controller.setup();
    g_controller.begin();

    /* Start the FreeRTOS Windows scheduler.
     * This never returns under normal operation – all application logic
     * runs inside the FreeRTOS tasks created by g_controller.begin(). */
    vTaskStartScheduler();

    /* Reached only if heap allocation failed inside vTaskStartScheduler() */
    std::cerr << "vTaskStartScheduler() returned unexpectedly!" << std::endl;
    return 1;
}
