#include "application.h"
#include "constants.h"

// Run the Application class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <board_id>" << std::endl;
        return 1;
    }
    else
    {
        //Get the board id
        board_id = argv[1];
    }

    // Init Application
    bool debug_application = true;
    bool debug_sub = false;
    Application application(PUBLISHER_PORT, "10.0.0.109", debug_application, debug_sub);

    // run the loop
    while (true)
    {
        application.update();
    }

    return 0;
}