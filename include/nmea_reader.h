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
capstone_protobuf::Packet generatePacket(google::protobuf::Timestamp *timestamp, const T& sensor_data, std::string label, int original_msg_id, const void* original_msg, int msg_len)
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
  payload->set_protocol(capstone_protobuf::Packet::Payload::NMEA);
  payload->set_original_message_id(original_msg_id);
  payload->set_original_message(original_msg, static_cast<size_t>(msg_len));
  payload->set_digital_signature(getDigitalSignature());

  packet.set_allocated_payload(payload); // Ownership transferred to packet

  printPacket(packet);

  return packet;
}

template <typename T>
void generateAndSendPacket(google::protobuf::Timestamp *timestamp, T& sensor_data, std::string label, int original_msg_id, const void* original_msg, int msg_len)
{
  capstone_protobuf::Packet packet = generatePacket(timestamp, sensor_data, label, original_msg_id, original_msg, msg_len);
  encryptPayload(packet);
  udpSend(packet);
}

template <typename T>
void generateAndSendNMEAPacket(google::protobuf::Timestamp *timestamp, T& sensor_data, std::string label, const tN2kMsg& NMEA_msg)
{
  int msg_id = static_cast<int>(NMEA_msg.PGN);
  const unsigned char (*dataPtr)[223] = &NMEA_msg.Data;
  int msg_len = static_cast<size_t>(NMEA_msg.DataLen);
  generateAndSendPacket(timestamp, sensor_data, label, msg_id, static_cast<const void*>(dataPtr), msg_len);
}


#endif