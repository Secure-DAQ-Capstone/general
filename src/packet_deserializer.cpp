#include "packet_deserializer.h"
//#include <N2kMessagesEnumToStr.h>
using namespace std;
//using google::protobuf::util::TimeUtil;

std::string readAndDeleteFirstStringFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::string first_line;
    std::getline(file, first_line);  // Read the first line (string)

    // Read the remaining lines and save them in a stringstream
    std::stringstream remaining_content;
    std::string line;
    while (std::getline(file, line)) {
        remaining_content << line << "\n";
    }

    file.close();

    // Now, reopen the file in write mode to clear it and write the remaining content back
    std::ofstream out_file(filename, std::ios::trunc);  // Open the file with truncation
    if (!out_file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return "";
    }

    out_file << remaining_content.str();  // Write the remaining content to the file
    out_file.close();

    return first_line;  // Return the first string
}

int main(void)
{
    //GOOGLE_PROTOBUF_VERIFY_VERSION;


     while(1) {
        cout <<  readAndDeleteFirstStringFromFile("str_msgs.txt")      << endl;                                  // Will send out CAN messages in open text 
    }
    
    return 0;
}