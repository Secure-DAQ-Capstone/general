#include "nmea_reader.h"
//#include <N2kMessagesEnumToStr.h>
using namespace std;
using google::protobuf::util::TimeUtil;

typedef struct {
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg); 
} tNMEA2000Handler;

tSocketStream serStream; 
std::vector<int> PGNS;

void Temperature(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg);
void OutsideEnvironmental2(const tN2kMsg &N2kMsg);

void VesselHeading(const tN2kMsg &N2kMsg);


void printData(const unsigned char* data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        // Print each byte in hexadecimal format
        std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)data[i] << " ";
        // Print a newline after every 16 bytes (for readability)
        if ((i + 1) % 16 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::dec << std::endl;  // Switch back to decimal formatting
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

tNMEA2000Handler NMEA2000Handlers[]={
  {130310L,&OutsideEnvironmental},
  {130312L,&Temperature},
  {130311L,&OutsideEnvironmental2},
  {127250L,&VesselHeading},
  {0,0}
};


void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg) {
  cout << "  OutsideEnvironmental" << endl;
    google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
    timestamp->set_seconds(time(nullptr));
    google::protobuf::Timestamp *timestamp2 = new google::protobuf::Timestamp();
    *timestamp2 = *timestamp;
    unsigned char SID;
    double WaterTemperature;
    double OutsideAmbientAirTemperature;
    double AtmosphericPressure;
    
    if (ParseN2kOutsideEnvironmentalParameters(N2kMsg,SID,WaterTemperature,OutsideAmbientAirTemperature,AtmosphericPressure) ) {
      //PrintLabelValWithConversionCheckUnDef("Water temp: ",WaterTemperature,&KelvinToC);
      //PrintLabelValWithConversionCheckUnDef(", outside ambient temp: ",OutsideAmbientAirTemperature,&KelvinToC);
      //PrintLabelValWithConversionCheckUnDef(", pressure: ",AtmosphericPressure,0,true);
    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }

    capstone_protobuf::Pressure pressure_data;
    pressure_data.set_pressure(AtmosphericPressure);
    generateAndSendPacket(timestamp, pressure_data, "pressure");

    capstone_protobuf::Temperature temperature_data;
    temperature_data.set_temperature(OutsideAmbientAirTemperature);
    generateAndSendPacket(timestamp2, temperature_data, "temperature");

cout << "END  OutsideEnvironmental" << endl;

}
void OutsideEnvironmental2(const tN2kMsg &N2kMsg) {
    cout << "  OutsideEnvironmental2" << endl;

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
    
    if (ParseN2kEnvironmentalParameters(N2kMsg,SID,TempSource,Temperature, HumiditySource,Humidity,AtmosphericPressure) ) {
      //PrintLabelValWithConversionCheckUnDef("Water temp: ",WaterTemperature,&KelvinToC);
      //PrintLabelValWithConversionCheckUnDef(", outside ambient temp: ",OutsideAmbientAirTemperature,&KelvinToC);
      //PrintLabelValWithConversionCheckUnDef(", pressure: ",AtmosphericPressure,0,true);
    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }
    
    capstone_protobuf::Pressure pressure_data;
    pressure_data.set_pressure(AtmosphericPressure);
    generateAndSendPacket(timestamp, pressure_data, "pressure");

    capstone_protobuf::Temperature temperature_data;
    temperature_data.set_temperature(Temperature);
    generateAndSendPacket(timestamp2, temperature_data, "temperature");

    capstone_protobuf::Humidity humidity_data;
    humidity_data.set_humidity(Humidity);
    generateAndSendPacket(timestamp3, humidity_data, "humidity");

    cout << "END OutsideEnvironmental2" << endl;;

}
//*****************************************************************************
void Temperature(const tN2kMsg &N2kMsg) {
  cout << "  Temperature" << endl;
    // get time captured data
    google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
    timestamp->set_seconds(time(nullptr));

    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;

    
    
    if (ParseN2kTemperature(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                        //serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
      //PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
      //PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);

    capstone_protobuf::Temperature temperature_data;
    temperature_data.set_temperature(ActualTemperature);
    generateAndSendPacket(timestamp, temperature_data, "temperature");

    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }


}
void VesselHeading(const tN2kMsg &N2kMsg) {
  cout << "VEsselHeading" << endl;
    // get time captured data
    google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
    timestamp->set_seconds(time(nullptr));

    unsigned char SID;
    double Heading;
    double Deviation;
    double Variation;
    tN2kHeadingReference ref;
    
    
    if (ParseN2kHeading(N2kMsg,SID,Heading,Deviation,Variation,ref) ) {
                        //serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
      //PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
      //PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);

    printData(N2kMsg.Data, 223); 
    capstone_protobuf::Heading heading_data;
    heading_data.set_heading(Heading);
    heading_data.set_deviation(Deviation);
    heading_data.set_variation(Variation);
    generateAndSendPacket(timestamp, heading_data, "heading");

    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }


}

//*****************************************************************************



void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  int iHandler;
  
  // Find handler
  //serStream.print("In Main Handler: "); serStream.println(N2kMsg.PGN);
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  
  if (NMEA2000Handlers[iHandler].PGN!=0) {
    NMEA2000Handlers[iHandler].Handler(N2kMsg); 
  }
    // Check if the PGN is already in the vector
    bool pgnExists = false;
    for (int i = 0; i < PGNS.size(); ++i) {
        if (PGNS[i] == N2kMsg.PGN) {
            pgnExists = true;
            break;
        }
    }

    // If the PGN is not in the vector, add it and print the current PGNs
    if (!pgnExists) {
        PGNS.push_back(N2kMsg.PGN);
        std::cout << "Added PGN: " << N2kMsg.PGN << std::endl;
        std::cout << "Current PGNs in vector: ";
        for (int pgn : PGNS) {
            std::cout << pgn << " ";
        }
        std::cout << std::endl;
    }
}

void printPacket(const capstone_protobuf::Packet& packet) {
  cout << packet.DebugString() << endl;
    // cout << "Packet Message:" << endl;
    // cout << "  Board ID: " << packet.board_id_msg_origin() << endl;
    // cout << "  Nonce: " << packet.nonce() << endl;
    // cout << "  Time Received: " << packet.time_received() << endl;
    // cout << "  Time Sent: " << packet.time_sent() << endl;

    // // Check if payload exists
    // if (packet.has_payload()) {
    //     cout << "  Payload:" << endl;

    //     // Convert timestamp to a human-readable string
    //     cout << "    Time Data Captured: " 
    //          << google::protobuf::util::TimeUtil::ToString(packet.payload().time_data_captured()) << endl;

    //     // Print Data fields
    //     if (packet.payload().has_data()) {
    //         cout << "    Data:" << endl;
    //         cout << "      Label: " << packet.payload().data().label() << endl;
    //         cout << "      Fields:" << endl;

    //         for (const auto& field : packet.payload().data().data()) {
    //             cout << "        - " << field.DebugString() << endl;
    //         }
    //     }

    //     // Print protocol
    //     cout << "    Protocol: ";
    //     switch (packet.payload().protocol()) {
    //         case capstone_protobuf::Packet::Payload::CAN: cout << "CAN"; break;
    //         case capstone_protobuf::Packet::Payload::Modbus: cout << "Modbus"; break;
    //         default: cout << "Unknown";
    //     }
    //     cout << endl;

    //     // Print original message as hex
    //     cout << "    Original Message: ";
    //     for (unsigned char c : packet.payload().original_message()) {
    //         printf("%02X ", c);
    //     }
    //     cout << endl;

    //     // Print digital signature
    //     cout << "    Digital Signature: " << packet.payload().digital_signature() << endl;
    // } else {
    //     cout << "  No Payload Data." << endl;
    // }
}



int main(void)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    UDPPub pub(1,2,3);
    cout << "Starting CAN watching" << endl;

    setvbuf (stdout, NULL, _IONBF, 0);                                          // No buffering on stdout, just send chars as they come.
 
   
    //NMEA2000.SetForwardStream(&serStream);                                      // Connect bridge function for streaming output.
    NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);   
    NMEA2000.SetMsgHandler(HandleNMEA2000Msg);                           // Show in clear text (for now)
       
    if (!NMEA2000.Open()) {
       cout << "Failed to open CAN port" << endl;
       return 1;
   }
    
    cout  << endl << "CAN started, going to watch it now" << endl;

     while(1) {
         NMEA2000.ParseMessages();                                               // Will send out CAN messages in open text 
    }
    
    return 0;
}
