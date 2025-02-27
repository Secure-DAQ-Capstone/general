#include "nmea_reader.h"
#include "constants.h"
// #include <N2kMessagesEnumToStr.h>

tNMEA2000Handler NMEA2000Handlers[] = {
    {130310L, &OutsideEnvironmental},  // works
    {130312L, &Temperature},           // works
    {130311L, &OutsideEnvironmental2}, // only temp works
    {127250L, &VesselHeading},         // works but GPS doesn't change data value
    {129025L, &PositionRapid},         // works
    {127258L, &MagneticVariation},     // works
    {130316L, &TemperatureExt},        // works
    {129026L, &COGSOGRapid},           // works
    {126992L, &SysTime},               // works
    {129029L, &GNSSPosition},          // works
    {129540L, &SatsInView},            // failed to parse
    {130306L, &WindData},              // works
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

void TemperatureExt(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  unsigned char SID;
  double SetTemperature;
  unsigned char TempInstance;
  tN2kTempSource TempSource;
  double ActualTemperature;

  if (ParseN2kTemperatureExt(N2kMsg, SID, TempInstance, TempSource, ActualTemperature, SetTemperature))
  {

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

void COGSOGRapid(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  unsigned char SID;
  tN2kHeadingReference ref;
  double COG;
  double SOG;

  if (ParseN2kCOGSOGRapid(N2kMsg, SID, ref, COG, SOG))
  {

    capstone_protobuf::COGSOG cogsog_data;
    cogsog_data.set_cog(COG);
    cogsog_data.set_sog(SOG);
    generateAndSendNMEAPacket(timestamp, cogsog_data, "cog sog", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

void SysTime(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  unsigned char SID;
  tN2kHeadingReference ref;
  uint16_t SystemDate;
  double SystemTime;
  double SOG;
  tN2kTimeSource TimeSource;

  if (ParseN2kSystemTime(N2kMsg, SID, SystemDate, SystemTime, TimeSource))
  {

    capstone_protobuf::SysTime systime_data;
    systime_data.set_date(SystemDate);
    systime_data.set_time(SystemTime);
    generateAndSendNMEAPacket(timestamp, systime_data, "system time", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

void GNSSPosition(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  unsigned char SID;
  uint16_t DaysSince1970;
  double SecondsSinceMidnight;
  double Latitude;
  double Longitude;
  double Altitude;
  tN2kGNSStype GNSStype;
  tN2kGNSSmethod GNSSmethod;
  unsigned char nSatellites;
  double HDOP;
  double PDOP;
  double GeoidalSeparation;
  unsigned char nReferenceStations;
  tN2kGNSStype ReferenceStationType;
  uint16_t ReferenceSationID;
  double AgeOfCorrection;

  if (ParseN2kGNSS(N2kMsg, SID, DaysSince1970, SecondsSinceMidnight,
                   Latitude, Longitude, Altitude,
                   GNSStype, GNSSmethod,
                   nSatellites, HDOP, PDOP, GeoidalSeparation,
                   nReferenceStations, ReferenceStationType, ReferenceSationID,
                   AgeOfCorrection))
  {

    capstone_protobuf::GNSS gnss_data;
    gnss_data.set_date(DaysSince1970);
    gnss_data.set_time(SecondsSinceMidnight);
    gnss_data.set_latitude(Latitude);
    gnss_data.set_longitude(Longitude);
    gnss_data.set_altitude(Altitude);
    gnss_data.set_num_satellites(nSatellites);
    gnss_data.set_hdop(HDOP);
    gnss_data.set_pdop(PDOP);
    gnss_data.set_geoidal_separation(GeoidalSeparation);
    gnss_data.set_num_reference_stations(nReferenceStations);
    generateAndSendNMEAPacket(timestamp, gnss_data, "gnss", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

void SatsInView(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  uint8_t SVIndex;
  tSatelliteInfo SatelliteInfo; // Not currently used

  if (ParseN2kPGNSatellitesInView(N2kMsg, SVIndex, SatelliteInfo))
  {

    capstone_protobuf::SatsInView sats_data;
    sats_data.set_sats_in_view(SVIndex);
    generateAndSendNMEAPacket(timestamp, sats_data, "satellites in view", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

void WindData(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  uint8_t SVIndex;
  unsigned char SID;
  double WindSpeed;
  double WindAngle;
  tN2kWindReference WindReference;

  if (ParseN2kWindSpeed(N2kMsg, SID, WindSpeed, WindAngle, WindReference))
  {

    capstone_protobuf::Wind wind_data;
    wind_data.set_speed(WindSpeed);
    wind_data.set_angle(WindAngle);
    generateAndSendNMEAPacket(timestamp, wind_data, "wind data", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

void PositionRapid(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  double Longitude;
  double Latitude;

  if (ParseN2kPositionRapid(N2kMsg, Longitude, Latitude))
  {

    capstone_protobuf::Position position_data;
    position_data.set_latitude(Latitude);
    position_data.set_longitude(Longitude);
    generateAndSendNMEAPacket(timestamp, position_data, "position rapid", N2kMsg);
  }
  else
  {
    serStream.print("Failed to parse PGN: ");
    serStream.println(N2kMsg.PGN);
  }
}

void MagneticVariation(const tN2kMsg &N2kMsg)
{

  google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
  timestamp->set_seconds(time(nullptr));

  unsigned char SID;
  tN2kMagneticVariation Source;
  uint16_t DaysSince1970;
  double Variation;

  if (ParseN2kMagneticVariation(N2kMsg, SID, Source, DaysSince1970, Variation))
  {

    capstone_protobuf::MagneticVariation variation_data;
    variation_data.set_age_of_service(DaysSince1970);
    variation_data.set_variation(Variation);
    generateAndSendNMEAPacket(timestamp, variation_data, "magnetic variation", N2kMsg);
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

  encryption_data_t encrypted_payload = encryptString(str_payload);

  std::string encrypted_payload_str = encrypted_payload.encrypted_string;
  std::string nonce_str = encrypted_payload.nonce;

  capstone_protobuf::EncryptedPacket encrypted_packet;

  capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();

  *metadata_copy = *packet.mutable_metadata();

  metadata_copy->set_nonce(nonce_str);

  encrypted_packet.set_allocated_metadata(metadata_copy);
  encrypted_packet.set_encrypted_payload(encrypted_payload_str);

  return encrypted_packet;
}

/**
 * Define the UDP Publisher with default values and change them if needed in main
 */
// defaults for command line arguments
const char *publish_ip = VisorLab::Ventana1;
int publish_port = PUBLISHER_PORT;
UDPPub pub(publish_port, publish_ip);

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

  cout << encrypted_packet.DebugString() << endl;

  // // Temp Derserializing Packet Example
  // if (encrypted_packet.ParseFromString(packet_str)) {
  //   capstone_protobuf::Packet packet;
  //   //TODO: GET THE NONCE FROM THE PACKET

  //   unsigned char nonce[crypto_secretbox_NONCEBYTES];
  //   //Generate the nonce
  //   symmetric_key_security_agent.generateNonce(nonce);

  //   std::string nonce_str(nonce, nonce+crypto_secretbox_NONCEBYTES);

  //   std::string payload_str = decryptString(encrypted_packet.encrypted_payload(), nonce_str);
  //   capstone_protobuf::MetaData *metadata_copy = new capstone_protobuf::MetaData();
  //   *metadata_copy = *encrypted_packet.mutable_metadata();
  //   packet.set_allocated_metadata(metadata_copy);
  //   capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
  //   payload->ParseFromString(payload_str);
  //   packet.set_allocated_payload(payload);
  //   cout << packet.DebugString() << endl;

  // } else {
  //      std::cerr << "Failed to parse string into Protobuf message!" << std::endl;
  // }
  return;
}

int main(int argc, char *argv[])
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

  // defaults for command line arguments
  // const char *publish_ip = VisorLab::MattLaptop;
  // int publish_port = PUBLISHER_PORT;

  // Parse command-line arguments
  if (std::strcmp(argv[1], "-h") == 0)
  {
    std::cout << "Usage: " << argv[0] << " [publish_ip] [publish_port]" << std::endl;
    std::cout << "  publish_ip: IP address to publish to (default: " << publish_ip << ")" << std::endl;
    std::cout << "  publish_port: Port to publish to (default: " << publish_port << ")" << std::endl;
    return 0;
  }

  // if (argc > 1)
  // {
  //   publish_ip = argv[1];
  //   std::cout << "Receive IP: " << publish_ip << std::endl;
  //   if (argc > 2)
  //   {
  //     publish_port = std::stoi(argv[2]);
  //     std::cout << "Receive Port: " << publish_port << std::endl;
  //   }
  //   // Reinitialize the publisher with the new IP and port
  //   // pub = UDPPub(publish_port, publish_ip);
  // }

  while (1)
  {
    NMEA2000.ParseMessages(); // Will send out CAN messages in open text
  }

  return 0;
}
