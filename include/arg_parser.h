#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include "constants.h"
#include <string>
#include <iostream>

struct ParsedArgs {
    std::string machine_id;
    const char *receive_ip;
    const char *publish_ip;
};

ParsedArgs parseArguments(int argc, char *argv[]);

#endif // ARG_PARSER_H