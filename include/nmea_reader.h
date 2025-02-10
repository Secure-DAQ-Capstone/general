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
#include <fstream> 
void printPacket(const capstone_protobuf::Packet& packet);

std::string encryptString(std::string str) //TODO
{
  return str;
}

std::string decryptString(std::string str)
{
  return str;
}

void udpSendString(std::string str)
{
  std::string filename = "str_msgs.txt";
  // Open the file in append mode
  std::ofstream file(filename, std::ios::app);

  // Check if the file opened successfully
  if (!file.is_open()) {
      std::cerr << "Failed to open file for appending: " << filename << std::endl;
      return;
  }

  // Write the string to the file
  file << str << std::endl;

  // Close the file
  file.close();

  return;
}
// Implement these in their own libraries / classes / helpers and move to separate file
capstone_protobuf::EncryptedPacket encryptPayload(capstone_protobuf::Packet& packet)
{
  std::string str_payload;
  packet.payload().SerializeToString(&str_payload);

  std::string encrypted_payload = encryptString(str_payload);  // TODO

  capstone_protobuf::EncryptedPacket encrypted_packet;

  capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();

  *metadata_copy = *packet.mutable_metadata();

  encrypted_packet.set_allocated_metadata(metadata_copy);
  encrypted_packet.set_encrypted_payload(encrypted_payload);

  return encrypted_packet; 
}

void udpSend(capstone_protobuf::Packet& packet)
{
  std::string packet_str;
  packet.SerializeToString(&packet_str);
  udpSendString(packet_str);
  return;
}

void udpSend(capstone_protobuf::EncryptedPacket& encrypted_packet)
{
  std::string packet_str;
  encrypted_packet.SerializeToString(&packet_str);
  udpSendString(packet_str);

    if (encrypted_packet.ParseFromString(packet_str)) {
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
  return;
}

std::string getDigitalSignature(capstone_protobuf::Packet& packet)
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
  

  packet.set_allocated_payload(payload); // Ownership transferred to packet

  payload->set_digital_signature(getDigitalSignature(packet));

  printPacket(packet);

  return packet;
}

template <typename T>
void generateAndSendPacket(google::protobuf::Timestamp *timestamp, T& sensor_data, std::string label, int original_msg_id, const void* original_msg, int msg_len)
{
  capstone_protobuf::Packet packet = generatePacket(timestamp, sensor_data, label, original_msg_id, original_msg, msg_len);
  capstone_protobuf::EncryptedPacket encrypted_packet = encryptPayload(packet);
  udpSend(encrypted_packet);
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