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
#include "security.h"
#include "constants.h"

using namespace std;
using google::protobuf::util::TimeUtil;

typedef struct
{
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg);
} tNMEA2000Handler;

typedef struct
{
  string encrypted_string;
  string nonce;
} encryption_data_t;

tSocketStream serStream;
std::vector<int> PGNS;

// Call the encryption function from the security class
encryption_data_t encryptString(std::string str, std::string board_id)
{
  unsigned char nonce[crypto_secretbox_NONCEBYTES];
  // Generate the nonce
  security_agent.generateNonce(nonce);

  // Encrypt the data and turn it into a string
  vector<unsigned char> encrypted = security_agent.encrypt((unsigned char *)str.data(), str.length(), nonce, board_id);

  string encrypted_str((const char *)(encrypted.data()), encrypted.size());
  string nonce_str(nonce, nonce + crypto_secretbox_NONCEBYTES);

  // Add the encrypted data and the nonce to the struct
  encryption_data_t encrypted_data;
  encrypted_data.encrypted_string = encrypted_str;
  encrypted_data.nonce = nonce_str;

  return encrypted_data;
}

// Call the decryption function from the security class
std::string decryptString(std::string str, std::string nonce_str, std::string board_id)
{
  unsigned char nonce[crypto_secretbox_NONCEBYTES];

  // Convert the nonce string to a char array
  copy(nonce_str.begin(), nonce_str.end(), nonce);

  // Decrypt the data
  vector<unsigned char> decrypted_array(str.begin(), str.end());
  vector<unsigned char> decrypted = security_agent.decrypt(decrypted_array, str.length(), nonce, board_id);
  string decrypted_str(decrypted.begin(), decrypted.end());

  return decrypted_str;
}

//Debugging Function
void udpSendStringToFile(std::string str)
{
  std::string filename = "str_msgs.txt";
  std::ofstream file(filename, std::ios::app);

  if (!file.is_open())
  {
    std::cerr << "Failed to open file for appending: " << filename << std::endl;
    return;
  }

  file << str << std::endl;

  file.close();
  return;
}

std::string getDigitalSignature(capstone_protobuf::Packet &packet)
{
  std::string str_payload;
  packet.payload().SerializeToString(&str_payload);

  string board_id = packet.metadata().board_id_msg_origin();
  unsigned char digital_signature[crypto_sign_BYTES];

  security_agent.generateSignature((unsigned char *)str_payload.data(), str_payload.length(), digital_signature, board_id);

  string digital_signature_str(reinterpret_cast<const char *>(digital_signature), crypto_sign_BYTES);
  return digital_signature_str;
}


void Temperature(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg);
void OutsideEnvironmental2(const tN2kMsg &N2kMsg);
void VesselHeading(const tN2kMsg &N2kMsg);
void PositionRapid(const tN2kMsg &N2kMsg);
void MagneticVariation(const tN2kMsg &N2kMsg);
void TemperatureExt(const tN2kMsg &N2kMsg);
void COGSOGRapid(const tN2kMsg &N2kMsg);
void SysTime(const tN2kMsg &N2kMsg);
void GNSSPosition(const tN2kMsg &N2kMsg);
void SatsInView(const tN2kMsg &N2kMsg);
void WindData(const tN2kMsg &N2kMsg);
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);
capstone_protobuf::EncryptedPacket encryptPayload(capstone_protobuf::Packet &packet);

void udpSend(capstone_protobuf::Packet &packet);
void udpSend(capstone_protobuf::EncryptedPacket &encrypted_packet);
void udpSend(std::string str);

// Template Function Implementations ********************************************************************

struct PacketArgs
{
  google::protobuf::Timestamp *timestamp;
  std::string label;
  int original_msg_id;
  const void *original_msg;
  int msg_len;
  std::vector<capstone_protobuf::MetaData::RelayChainEntry> relay_chain_entries;

  PacketArgs(google::protobuf::Timestamp *ts, const std::string &lbl, int msg_id, const void *orig_msg, int len, const std::vector<capstone_protobuf::MetaData::RelayChainEntry> &entries)
      : timestamp(ts), label(lbl), original_msg_id(msg_id), original_msg(orig_msg), msg_len(len), relay_chain_entries(entries) {}
};

void generateMetaData(
    std::string board_id,
    int32_t time_received,
    int32_t time_sent,
    capstone_protobuf::MetaData *metadata)
{
  metadata->set_time_received(time_received);
  metadata->set_time_sent(time_sent);
  metadata->set_board_id_msg_origin(board_id);

  // Make and add a relay chain entry
  capstone_protobuf::MetaData::RelayChainEntry *entry = metadata->add_relay_chain();
  entry->set_board_id(board_id);
  entry->set_timestamp(static_cast<int32_t>(time(nullptr))); // Set the current timestamp
}

template <typename T>
capstone_protobuf::Packet generatePacket(
    google::protobuf::Timestamp *timestamp,
    const T &sensor_data, std::string label,
    int original_msg_id,
    const void *original_msg,
    int msg_len,
    capstone_protobuf::MetaData *metadata)
{
  capstone_protobuf::Packet packet;
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

  metadata->set_digital_signature(getDigitalSignature(packet));

  return packet;
}

template <typename T>
void generateAndSendPacket(google::protobuf::Timestamp *timestamp, T &sensor_data, std::string label, int original_msg_id, const void *original_msg, int msg_len)
{
  capstone_protobuf::MetaData *metadata = new capstone_protobuf::MetaData();

  generateMetaData(board_id, 12345, 0000, metadata);
  capstone_protobuf::Packet packet = generatePacket(
      timestamp, sensor_data,
      label, original_msg_id,
      original_msg, msg_len,
      metadata);
  capstone_protobuf::EncryptedPacket encrypted_packet = encryptPayload(packet);
  udpSend(encrypted_packet);
}

template <typename T>
void generateAndSendNMEAPacket(google::protobuf::Timestamp *timestamp, T &sensor_data, std::string label, const tN2kMsg &NMEA_msg)
{
  int msg_id = static_cast<int>(NMEA_msg.PGN);
  const unsigned char(*dataPtr)[223] = &NMEA_msg.Data;
  int msg_len = static_cast<size_t>(NMEA_msg.DataLen);
  generateAndSendPacket(timestamp, sensor_data, label, msg_id, static_cast<const void *>(dataPtr), msg_len);
}

template <typename T>
void PrintLabelValWithConversionCheckUnDef(const char *label, T val, double (*ConvFunc)(double val) = 0, bool AddLf = false, int8_t Desim = -1)
{
  serStream.print(label);
  if (!N2kIsNA(val))
  {
    if (Desim < 0)
    {
      if (ConvFunc)
      {
        serStream.print(ConvFunc(val));
      }
      else
      {
        serStream.print(val);
      }
    }
    else
    {
      if (ConvFunc)
      {
        serStream.print(ConvFunc(val), Desim);
      }
      else
      {
        serStream.print(val, Desim);
      }
    }
  }
  else
    serStream.print("not available");
  if (AddLf)
    serStream.println();
}

#endif
