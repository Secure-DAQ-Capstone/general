#ifndef PROTO_JSON_CONVERTER_H
#define PROTO_JSON_CONVERTER_H

#include "packet.pb.h"
#include <string>
#include <google/protobuf/util/json_util.h>

class ProtoJsonConverter {
public:
    static std::string toJson(const capstone_protobuf::Packet &packet);
};

#endif // PROTO_JSON_CONVERTER_H