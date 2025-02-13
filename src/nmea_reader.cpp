#include "nmea_reader.h"
#include "constants.h"
// #include <N2kMessagesEnumToStr.h>

tNMEA2000Handler NMEA2000Handlers[] = {
    {130310L, &OutsideEnvironmental},
    {130312L, &Temperature},
    {130311L, &OutsideEnvironmental2},
    {127250L, &VesselHeading},
    {0, 0}};

// Handlers************************************************************************************************
void OutsideEnvironmental(const tN2kMsg &N2kMsg)
{
  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));
  google::protobuf::Timestamp *timestamp2 = new google::protobuf::Timestamp();
  *timestamp2 = *timestamp;
  unsigned char SID;
  double WaterTemperature;
  double OutsideAmbientAirTemperature;
  double AtmosphericPressure;

  if (ParseN2kOutsideEnvironmentalParameters(N2kMsg, SID, WaterTemperature, OutsideAmbientAirTemperature, AtmosphericPressure))
  {
    // PrintLabelValWithConversionCheckUnDef("Water temp: ",WaterTemperature,&KelvinToC);
    // PrintLabelValWithConversionCheckUnDef(", outside ambient temp: ",OutsideAmbientAirTemperature,&KelvinToC);
    // PrintLabelValWithConversionCheckUnDef(", pressure: ",AtmosphericPressure,0,true);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }

  capstone_protobuf::Pressure pressure_data;
  pressure_data.set_pressure(AtmosphericPressure);
  generateAndSendNMEAPacket(timestamp, pressure_data, "pressure", N2kMsg);

  capstone_protobuf::Temperature temperature_data;
  temperature_data.set_temperature(OutsideAmbientAirTemperature);
  generateAndSendNMEAPacket(timestamp2, temperature_data, "temperature", N2kMsg);
}

void OutsideEnvironmental2(const tN2kMsg &N2kMsg)
{
  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));
  google::protobuf::Timestamp *timestamp2 = new google::protobuf::Timestamp();
  *timestamp2 = *timestamp;
  google::protobuf::Timestamp *timestamp3 = new google::protobuf::Timestamp();
  *timestamp3 = *timestamp;

  unsigned char SID;
  double WaterTemperature;
  double OutsideAmbientAirTemperature;
  double AtmosphericPressure;
  tN2kHumiditySource HumiditySource;
  double Humidity;
  tN2kTempSource TempSource;
  double Temperature;

  if (ParseN2kEnvironmentalParameters(N2kMsg, SID, TempSource, Temperature, HumiditySource, Humidity, AtmosphericPressure))
  {
    // PrintLabelValWithConversionCheckUnDef("Water temp: ",WaterTemperature,&KelvinToC);
    // PrintLabelValWithConversionCheckUnDef(", outside ambient temp: ",OutsideAmbientAirTemperature,&KelvinToC);
    // PrintLabelValWithConversionCheckUnDef(", pressure: ",AtmosphericPressure,0,true);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }

  capstone_protobuf::Pressure pressure_data;
  pressure_data.set_pressure(AtmosphericPressure);
  generateAndSendNMEAPacket(timestamp, pressure_data, "pressure", N2kMsg);

  capstone_protobuf::Temperature temperature_data;
  temperature_data.set_temperature(Temperature);
  generateAndSendNMEAPacket(timestamp2, temperature_data, "temperature", N2kMsg);

  capstone_protobuf::Humidity humidity_data;
  humidity_data.set_humidity(Humidity);
  generateAndSendNMEAPacket(timestamp3, humidity_data, "humidity", N2kMsg);
}

void Temperature(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  unsigned char SID;
  unsigned char TempInstance;
  tN2kTempSource TempSource;
  double ActualTemperature;
  double SetTemperature;

  if (ParseN2kTemperature(N2kMsg, SID, TempInstance, TempSource, ActualTemperature, SetTemperature))
  {
    // serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
    // PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
    // PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);

    capstone_protobuf::Temperature temperature_data;
    temperature_data.set_temperature(ActualTemperature);
    generateAndSendNMEAPacket(timestamp, temperature_data, "temperature", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

void VesselHeading(const tN2kMsg &N2kMsg)
{
  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  unsigned char SID;
  double Heading;
  double Deviation;
  double Variation;
  tN2kHeadingReference ref;

  if (ParseN2kHeading(N2kMsg, SID, Heading, Deviation, Variation, ref))
  {
    // serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
    // PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
    // PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);

    capstone_protobuf::Heading heading_data;
    heading_data.set_heading(Heading);
    heading_data.set_deviation(Deviation);
    heading_data.set_variation(Variation);
    generateAndSendNMEAPacket(timestamp, heading_data, "heading", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

//*****************************************************************************

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg)
{
  int iHandler;

  // Find handler
  // serStream.print("In Main Handler: "); serStream.println(N2kMsg.PGN);
  for (iHandler = 0; NMEA2000Handlers[iHandler].PGN != 0 && !(N2kMsg.PGN == NMEA2000Handlers[iHandler].PGN); iHandler++)
    ;

  if (NMEA2000Handlers[iHandler].PGN != 0)
  {
    NMEA2000Handlers[iHandler].Handler(N2kMsg);
  }
  // Check if the PGN is already in the vector
  bool pgnExists = false;
  for (int i = 0; i < PGNS.size(); ++i)
  {
    if (PGNS[i] == N2kMsg.PGN)
    {
      pgnExists = true;
      break;
    }
  }

  // If the PGN is not in the vector, add it and print the current PGNs
  if (!pgnExists)
  {
    PGNS.push_back(N2kMsg.PGN);
    std::cout << "Added PGN: " << N2kMsg.PGN << std::endl;
    std::cout << "Current PGNs in vector: ";
    for (int pgn : PGNS)
    {
      std::cout << pgn << " ";
    }
    std::cout << std::endl;
  }
}

capstone_protobuf::EncryptedPacket encryptPayload(capstone_protobuf::Packet &packet)
{
  std::string str_payload;
  packet.payload().SerializeToString(&str_payload);

  std::string encrypted_payload = encryptString(str_payload); // TODO

  capstone_protobuf::EncryptedPacket encrypted_packet;

  capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();

  *metadata_copy = *packet.mutable_metadata();

  encrypted_packet.set_allocated_metadata(metadata_copy);
  encrypted_packet.set_encrypted_payload(encrypted_payload);

  return encrypted_packet;
}

const bool UDP_DEBUG = true;
UDPPub pub(UDP_BUFFER_SIZE, PUBLISHER_PORT, GracesHouse::Ventana1, false, true);

void udpSendString(std::string packet_str)
{
  bool debug = true;
  pub.write(packet_str, debug);
}

void udpSend(capstone_protobuf::Packet &packet)
{
  std::string packet_str;
  packet.SerializeToString(&packet_str);
  udpSendString(packet_str);
  return;
}

void udpSend(capstone_protobuf::EncryptedPacket &encrypted_packet)
{
  std::string packet_str;
  encrypted_packet.SerializeToString(&packet_str);
  udpSendString(packet_str);

  // Temp Derserializing Packet Example
  // if (encrypted_packet.ParseFromString(packet_str)) {
  //   capstone_protobuf::Packet packet;
  //   std::string payload_str = decryptString(encrypted_packet.encrypted_payload());
  //   capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();
  //   *metadata_copy = *encrypted_packet.mutable_metadata();
  //   packet.set_allocated_metadata(metadata_copy);
  //   capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
  //   payload->ParseFromString(payload_str);
  //   packet.set_allocated_payload(payload);
  //   cout << packet.DebugString() << endl;

  // } else {
  //     std::cerr << "Failed to parse string into Protobuf message!" << std::endl;
  // }
  return;
}

int main(void)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // UDPPub pub(1,2,3);
  cout << "Starting CAN watching" << endl;

  setvbuf(stdout, NULL, _IONBF, 0); // No buffering on stdout, just send chars as they come.

  // NMEA2000.SetForwardStream(&serStream);                                      // Connect bridge function for streaming output.
  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg); // Show in clear text (for now)

  if (!NMEA2000.Open())
  {
    cout << "Failed to open CAN port" << endl;
    return 1;
  }

  cout << endl
       << "CAN started, going to watch it now" << endl;

  while (1)
  {
    NMEA2000.ParseMessages(); // Will send out CAN messages in open text
  }

  return 0;
}
