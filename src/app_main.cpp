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
    cout << "Usage: " << argv[0] << " <machine_number>" << endl;
    cout << "  <machine_number>: The number corresponding to the machine ID (e.g., 1 for Tony Computer)" << endl;
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
        return 1;
    }
  }

    // Init Application
    bool debug_application = true;
    bool debug_sub = false;
    Application application(PUBLISHER_PORT, VisorLab::FaresLaptop, debug_application, debug_sub);

    // run the loop
    while (true)
    {
        application.update();
    }

    return 0;
}