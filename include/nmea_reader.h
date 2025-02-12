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

using namespace std;
using google::protobuf::util::TimeUtil;

typedef struct {
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg); 
} tNMEA2000Handler;

tSocketStream serStream; 
std::vector<int> PGNS;

// TODO - Temporary Implementations **********************************************************************
std::string encryptString(std::string str) //TODO
{
  return str;
}

std::string decryptString(std::string str)
{
  return str;
}

void udpSendStringToFile(std::string str) 
{
  std::string filename = "str_msgs.txt";
  std::ofstream file(filename, std::ios::app);

  if (!file.is_open()) {
      std::cerr << "Failed to open file for appending: " << filename << std::endl;
      return;
  }

  file << str << std::endl;

  file.close();
  return;
}

std::string getDigitalSignature(capstone_protobuf::Packet& packet)
{
    return "12345"; // TODO
}
//********************************************************************************************* */

void Temperature(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg);
void OutsideEnvironmental2(const tN2kMsg &N2kMsg);
void VesselHeading(const tN2kMsg &N2kMsg);
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);
capstone_protobuf::EncryptedPacket encryptPayload(capstone_protobuf::Packet& packet);

void udpSend(capstone_protobuf::Packet& packet);
void udpSend(capstone_protobuf::EncryptedPacket& encrypted_packet);
void udpSend(std::string str);



// Template Function Implementations ********************************************************************

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

template<typename T> void PrintLabelValWithConversionCheckUnDef(const char* label, T val, double (*ConvFunc)(double val)=0, bool AddLf=false, int8_t Desim=-1 ) {
  serStream.print(label);
  if (!N2kIsNA(val)) {
    if ( Desim<0 ) {
      if (ConvFunc) { serStream.print(ConvFunc(val)); } else { serStream.print(val); }
    } else {
      if (ConvFunc) { serStream.print(ConvFunc(val),Desim); } else { serStream.print(val,Desim); }
    }
  } else serStream.print("not available");
  if (AddLf) serStream.println();
}



#endif