#include <iostream> // For console output

#include "os.h"


Os::Os()
{
}


void Os::setup()
{
}


void Os::loop()
{
}


void Os::log(const char *pc_Message)
{
    // Log the message to the console or a file
    std::cout << pc_Message << std::endl;
}
