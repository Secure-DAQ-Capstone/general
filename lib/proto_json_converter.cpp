/// ProtoJsonConverter.cpp
#include "proto_json_converter.h"

std::string ProtoJsonConverter::toJson(const capstone_protobuf::Packet &packet) {
    std::string json_output;
    
    // Create JsonOptions to include default values
    google::protobuf::util::JsonOptions options;
    options.always_print_primitive_fields = true;  // Include fields with default values
    
    google::protobuf::util::MessageToJsonString(packet, &json_output, options);
    return json_output;
}

std::string ProtoJsonConverter::toJson(const capstone_protobuf::EncryptedPacket &packet) {
    std::string json_output;
    
    // Create JsonOptions to include default values
    google::protobuf::util::JsonOptions options;
    options.always_print_primitive_fields = true;  // Include fields with default values
    
    google::protobuf::util::MessageToJsonString(packet, &json_output, options);
    return json_output;
}