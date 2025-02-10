#include "packet_deserializer.h"
//#include <N2kMessagesEnumToStr.h>
using namespace std;
using google::protobuf::util::TimeUtil;

std::string readAndDeleteFirstStringFromFile(const std::string& filename, bool delete_after_read) {
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

    if (delete_after_read)
    {
      // Now, reopen the file in write mode to clear it and write the remaining content back
      std::ofstream out_file(filename, std::ios::trunc);  // Open the file with truncation
      if (!out_file.is_open()) {
          std::cerr << "Failed to open file for writing: " << filename << std::endl;
          return "";
      }

      out_file << remaining_content.str();  // Write the remaining content to the file
      out_file.close();
    }


    return first_line;  // Return the first string
}

std::string decryptString(std::string str)
{
  return str;
}

int main(void)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;


  while(1) {      
    std::string msg = readAndDeleteFirstStringFromFile("str_msgs.txt", true);

    capstone_protobuf::EncryptedPacket encrypted_packet;

    // Use ParseFromString to parse the string into the Protobuf message
    if (encrypted_packet.ParseFromString(msg)) {
        //std::cout << "Successfully parsed string into Protobuf message!" << std::endl;
        capstone_protobuf::Packet packet;

        std::string payload_str = decryptString(encrypted_packet.encrypted_payload());

        capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();

        *metadata_copy = *encrypted_packet.mutable_metadata();

        packet.set_allocated_metadata(metadata_copy);

        capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();

        payload->ParseFromString(payload_str);

        packet.set_allocated_payload(payload);

        cout << packet.DebugString() << endl;

    } else {
        std::cerr << "Failed to parse string into Protobuf message!" << std::endl;
    }



  }
    
    return 0;
}