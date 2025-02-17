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
    size_t max_buffer_size = UDP_BUFFER_SIZE;
    Application application(max_buffer_size, PUBLISHER_PORT, GracesHouse::Ventana1_ETH1, debug_application, debug_sub);

    // run the loop
    while (true)
    {
        application.update();
    }

    return 0;
}