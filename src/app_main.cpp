#include "application.h"
#include "constants.h"

// Run the Application class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <board_id> [receive_ip]" << std::endl;
        std::cout << "  <board_id>    : The ID of the board." << std::endl;
        std::cout << "  [receive_ip]  : The IP address to receive packets." << std::endl;
        return 1;
    }
    else
    {
        //Get the board id
        board_id = argv[1];
    }

     // defaults for command line arguments
     const char *receive_ip = VisorLab::Tony;;
     int receive_port = PUBLISHER_PORT;
 
     // Parse command-line arguments
     if (argc > 2)
     {
         receive_ip = argv[2];
         std::cout << "Receive IP: " << receive_ip << std::endl;
     }
 

    // Init Application
    bool debug_application = true;
    bool debug_sub = false;
    Application application(receive_port, receive_ip, debug_application, debug_sub);

    // run the loop
    while (true)
    {
        application.update();
    }

    return 0;
}