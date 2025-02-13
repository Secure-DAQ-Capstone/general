/// ProtoJsonConverter.cpp
#include "proto_json_converter.h"

std::string ProtoJsonConverter::toJson(const capstone_protobuf::Packet &packet) {
    std::string json_output;
    google::protobuf::util::MessageToJsonString(packet, &json_output);
    return json_output;
}