#include "application.h"
#include "constants.h"

// Run the Application class
int main(int argc, char *argv[])
{
    // Init protobuf variables
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Parse command line arguments
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <board_id> [receive_ip]" << std::endl;
        std::cout << "  <board_id>    : The ID of the board." << std::endl;
        std::cout << "  [receive_ip]  : The IP address to receive packets." << std::endl;
        return 1;
    }
  else
  {
    // Get the machine ID based on the number
    int machine_number = std::stoi(argv[1]);
    GLOBAL_BOARD_ID = MachineRegistry::getMachineId(machine_number);
    std::cout << "Machine ID: " << GLOBAL_BOARD_ID << std::endl;
    if (GLOBAL_BOARD_ID.empty()) {
        std::cerr << "Invalid machine number: " << machine_number << std::endl;
    }
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