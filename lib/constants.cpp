#include "constants.h"

// Define the global variable
security_base security_agent;

const std::vector<MachineInfo> MachineRegistry::machines = {
    {5, "Tony", "06317250767f47b29c2f8d5e4c14e927"},
    {1, "Ventana 1", "4088253719cb401487ea11178840430d1"},
    {2, "Ventana 2", "4088253719cb401487ea11178840430d"},
    {3, "Ventana 3", "4088253719cb401487ea11178840430d3"},
    {4, "Ventana 4", "4088253719cb401487ea11178840430d4"},
    {6, "David", "6e824f19507d4a1dac5e15101c0ae908"},
    // Add other machines as needed
};

std::string GLOBAL_BOARD_ID;