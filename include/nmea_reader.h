#ifndef _nmea_reader_H_
#define _nmea_reader_H_

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "NMEA2000_CAN.h"
#include <N2kMessages.h>
#include "packet.pb.h"
#include <google/protobuf/util/time_util.h>
#include "udp_pub.h"
#include <iomanip>

void printPacket(const capstone_protobuf::Packet& packet);

// Implement these in their own libraries / classes / helpers and move to separate file
void encryptPayload(capstone_protobuf::Packet& packet)
{
  return; // TODO
}
void udpSend(capstone_protobuf::Packet& packet)
{
  return; // TODO
}
std::string getDigitalSignature()
{
    return "12345"; // TODO
}

template <typename T>
capstone_protobuf::Packet generatePacket(google::protobuf::Timestamp *timestamp, const T& sensor_data, std::string label)
{
  capstone_protobuf::Packet packet;
  
  capstone_protobuf::MetaData *metadata = new capstone_protobuf::MetaData();
  metadata->set_board_id_msg_origin(2);
  metadata->set_nonce(1);
  packet.set_allocated_metadata(metadata);

  capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
  payload->set_allocated_time_data_captured(timestamp);
  
  capstone_protobuf::Packet::Payload::Data *data = payload->mutable_data();
  data->set_label(label);
  data->add_data()->PackFrom(sensor_data);

  // Set protocol and additional details
  payload->set_protocol(capstone_protobuf::Packet::Payload::CAN);
  payload->set_original_message("Env Params 311");
  payload->set_digital_signature(getDigitalSignature());

  packet.set_allocated_payload(payload); // Ownership transferred to packet

  printPacket(packet);

  return packet;
}

template <typename T>
void generateAndSendPacket(google::protobuf::Timestamp *timestamp, T& sensor_data, std::string label)
{
  capstone_protobuf::Packet packet = generatePacket(timestamp, sensor_data, label);
  encryptPayload(packet);
  udpSend(packet);
}


#endif