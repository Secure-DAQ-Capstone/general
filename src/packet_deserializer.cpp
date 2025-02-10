#include "packet_deserializer.h"
//#include <N2kMessagesEnumToStr.h>
using namespace std;
using google::protobuf::util::TimeUtil;

std::string readAndDeleteFirstStringFromFile(const std::string& filename, bool delete_after_read) {
    //std::string filename = "str_msgs.txt";
    std::ifstream file(filename);

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return "err";
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            break;
        }
    }

    file.close();

    return line;
}

std::string decryptString(std::string str)
{
  return str;
}

int main(void)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;


  while(1) {      
  //   std::string msg = readAndDeleteFirstStringFromFile("str_msgs.txt", true);

  //     cout << "\n encrypted packet string: \n";
  // cout << msg << endl;
  // cout << "\n END string \n";

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