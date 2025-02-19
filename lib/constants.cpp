#include "constants.h"

// Define the global variable
security_base symmetric_key_security_agent("../symmetric_key_boards.txt", 0);

security_base signature_verifier_security_agent("../public_key_boards.txt", 2);
