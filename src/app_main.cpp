#include "application.h"
#include "constants.h"


// Run the Application class
int main()
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Init Application
    bool debug_application = true;
    bool debug_sub = false;
    Application application(PUBLISHER_PORT, GracesHouse::FaresLaptop, debug_application, debug_sub);

    // run the loop
    while (true)
    {
        application.update();
    }

    return 0;
}