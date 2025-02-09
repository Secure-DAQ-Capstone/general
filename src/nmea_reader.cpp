#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "NMEA2000_CAN.h"
#include <N2kMessages.h>
#include "packet.pb.h"
#include <google/protobuf/util/time_util.h>
#include "udp_pub.h"
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
void Humidity(const tN2kMsg &N2kMsg);
void TemperatureExt(const tN2kMsg &N2kMsg);
void VesselHeading(const tN2kMsg &N2kMsg);
void printPacket(const capstone_protobuf::Packet& packet);

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
  {130316L,&VesselHeading},
  {0,0}
};


void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg) {
      google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
    timestamp->set_seconds(time(nullptr));
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

    capstone_protobuf::Packet packet;

    // Set timestamp
    capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
    payload->set_allocated_time_data_captured(timestamp);

    // Set data
    capstone_protobuf::Pressure pressure_data;
    pressure_data.set_pressure(AtmosphericPressure);
    google::protobuf::Any any_data;
    any_data.PackFrom(pressure_data);
    capstone_protobuf::Packet::Payload::Data *data = new capstone_protobuf::Packet::Payload::Data();
    data->set_label("pressure");
    data->add_data()->PackFrom(pressure_data);

    // Fill out payload
    payload->set_allocated_data(data);
    payload->set_protocol(capstone_protobuf::Packet::Payload::CAN);
    payload->set_original_message("Outside Env 310");
    payload->set_digital_signature("12345");

    packet.set_allocated_payload(payload);
std::string string_data;
       packet.SerializeToString(&string_data);
       printPacket(packet);

// TEMP
    capstone_protobuf::Packet::Payload *payload2 = new capstone_protobuf::Packet::Payload();
    payload2->set_allocated_time_data_captured(timestamp);

    // Set data
    capstone_protobuf::Temperature temp_data;
    temp_data.set_temperature(OutsideAmbientAirTemperature);
    //google::protobuf::Any any_data;
    any_data.PackFrom(temp_data);
    capstone_protobuf::Packet::Payload::Data *data2 = new capstone_protobuf::Packet::Payload::Data();
    data2->set_label("temperature");
    data2->add_data()->PackFrom(temp_data);

    // Fill out payload
    payload2->set_allocated_data(data2);
    payload2->set_protocol(capstone_protobuf::Packet::Payload::CAN);
    payload2->set_original_message("Outside Env 310");
    payload2->set_digital_signature("12345");

    packet.set_allocated_payload(payload2);
       printPacket(packet);



}
void OutsideEnvironmental2(const tN2kMsg &N2kMsg) {
      google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
    timestamp->set_seconds(time(nullptr));
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

    capstone_protobuf::Packet packet;

    // Set timestamp
    capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
    payload->set_allocated_time_data_captured(timestamp);

    // Set data
    capstone_protobuf::Pressure pressure_data;
    pressure_data.set_pressure(AtmosphericPressure);
    google::protobuf::Any any_data;
    any_data.PackFrom(pressure_data);
    capstone_protobuf::Packet::Payload::Data *data = new capstone_protobuf::Packet::Payload::Data();
    data->set_label("pressure");
    data->add_data()->PackFrom(pressure_data);

    // Fill out payload
    payload->set_allocated_data(data);
    payload->set_protocol(capstone_protobuf::Packet::Payload::CAN);
    payload->set_original_message("Env Params 311");
    payload->set_digital_signature("12345");

    packet.set_allocated_payload(payload);
std::string string_data;
       packet.SerializeToString(&string_data);
       printPacket(packet);

// TEMP
    capstone_protobuf::Packet::Payload *payload2 = new capstone_protobuf::Packet::Payload();
    payload2->set_allocated_time_data_captured(timestamp);

    // Set data
    capstone_protobuf::Temperature temp_data;
    temp_data.set_temperature(Temperature);
    //google::protobuf::Any any_data;
    any_data.PackFrom(temp_data);
    capstone_protobuf::Packet::Payload::Data *data2 = new capstone_protobuf::Packet::Payload::Data();
    data2->set_label("temperature");
    data2->add_data()->PackFrom(temp_data);

    // Fill out payload
    payload2->set_allocated_data(data2);
    payload2->set_protocol(capstone_protobuf::Packet::Payload::CAN);
    payload2->set_original_message("Env Params 311");
    payload2->set_digital_signature("12345");

    packet.set_allocated_payload(payload2);
       printPacket(packet);

// Pressure
    capstone_protobuf::Packet::Payload *payload3 = new capstone_protobuf::Packet::Payload();
    payload3->set_allocated_time_data_captured(timestamp);

    // Set data
    capstone_protobuf::Humidity humidity_data;
    humidity_data.set_humidity(Humidity);
    //google::protobuf::Any any_data;
    any_data.PackFrom(humidity_data);
    capstone_protobuf::Packet::Payload::Data *data3 = new capstone_protobuf::Packet::Payload::Data();
    data3->set_label("humidity");
    data3->add_data()->PackFrom(humidity_data);

    // Fill out payload
    payload3->set_allocated_data(data3);
    payload3->set_protocol(capstone_protobuf::Packet::Payload::CAN);
    payload3->set_original_message("Env Params 311");
    payload3->set_digital_signature("12345");

    packet.set_allocated_payload(payload3);
       printPacket(packet);

}
//*****************************************************************************
void Temperature(const tN2kMsg &N2kMsg) {
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

    capstone_protobuf::Packet packet;

    // Set timestamp
    capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
    payload->set_allocated_time_data_captured(timestamp);

    // Set data
    capstone_protobuf::Temperature temp_data;
    temp_data.set_temperature(ActualTemperature);
    google::protobuf::Any any_data;
    any_data.PackFrom(temp_data);
    capstone_protobuf::Packet::Payload::Data *data = new capstone_protobuf::Packet::Payload::Data();
    data->set_label("temperature");
    data->add_data()->PackFrom(temp_data);

    // Fill out payload
    payload->set_allocated_data(data);
    payload->set_protocol(capstone_protobuf::Packet::Payload::CAN);
    payload->set_original_message("1300312");
    payload->set_digital_signature("12345");

    packet.set_allocated_payload(payload);

 	std::string string_data;
       packet.SerializeToString(&string_data);
// 	cout << string_data << endl;

       capstone_protobuf::Packet p2;
       p2.ParseFromString(string_data);
 printPacket(p2);

    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }


}
void VesselHeading(const tN2kMsg &N2kMsg) {
    // get time captured data
    google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
    timestamp->set_seconds(time(nullptr));

    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    double Heading;
    double Deviation;
    double Variation;
    tN2kHeadingReference ref;
    
    
    if (ParseN2kHeading(N2kMsg,SID,Heading,Deviation,Variation,ref) ) {
                        //serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
      //PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
      //PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);

    capstone_protobuf::Packet packet;

    // Set timestamp
    capstone_protobuf::Packet::Payload *payload = new capstone_protobuf::Packet::Payload();
    payload->set_allocated_time_data_captured(timestamp);

    // Set data
    capstone_protobuf::Heading heading_data;
    heading_data.set_heading(Heading);
    heading_data.set_deviation(Deviation);
    heading_data.set_variation(Variation);

    google::protobuf::Any any_data;
    any_data.PackFrom(heading_data);
    capstone_protobuf::Packet::Payload::Data *data = new capstone_protobuf::Packet::Payload::Data();
    data->set_label("heading");
    data->add_data()->PackFrom(heading_data);

    // Fill out payload
    payload->set_allocated_data(data);
    payload->set_protocol(capstone_protobuf::Packet::Payload::CAN);
    payload->set_original_message("Vessel Heading");
    payload->set_digital_signature("12345");

    packet.set_allocated_payload(payload);

 	std::string string_data;
       packet.SerializeToString(&string_data);
// 	cout << string_data << endl;

       capstone_protobuf::Packet p2;
       p2.ParseFromString(string_data);
 printPacket(p2);

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
    cout << "Packet Message:" << endl;
    cout << "  Board ID: " << packet.board_id_msg_origin() << endl;
    cout << "  Nonce: " << packet.nonce() << endl;
    cout << "  Time Received: " << packet.time_received() << endl;
    cout << "  Time Sent: " << packet.time_sent() << endl;

    // Check if payload exists
    if (packet.has_payload()) {
        cout << "  Payload:" << endl;

        // Convert timestamp to a human-readable string
        cout << "    Time Data Captured: " 
             << google::protobuf::util::TimeUtil::ToString(packet.payload().time_data_captured()) << endl;

        // Print Data fields
        if (packet.payload().has_data()) {
            cout << "    Data:" << endl;
            cout << "      Label: " << packet.payload().data().label() << endl;
            cout << "      Fields:" << endl;

            for (const auto& field : packet.payload().data().data()) {
                cout << "        - " << field.DebugString() << endl;
            }
        }

        // Print protocol
        cout << "    Protocol: ";
        switch (packet.payload().protocol()) {
            case capstone_protobuf::Packet::Payload::CAN: cout << "CAN"; break;
            case capstone_protobuf::Packet::Payload::Modbus: cout << "Modbus"; break;
            default: cout << "Unknown";
        }
        cout << endl;

        // Print original message as hex
        cout << "    Original Message: ";
        for (unsigned char c : packet.payload().original_message()) {
            printf("%02X ", c);
        }
        cout << endl;

        // Print digital signature
        cout << "    Digital Signature: " << packet.payload().digital_signature() << endl;
    } else {
        cout << "  No Payload Data." << endl;
    }
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
