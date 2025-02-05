/* 
 * File:   main.cpp
 * Author: al
 *
 * Testing for CAN and RPI 
 * 
 * See: https://github.com/thomasonw/NMEA2000_socketCAN
 *
 * Created on February 12, 2017, 2:37 PM
 */
 
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "NMEA2000_CAN.h"
#include <N2kMessages.h>
#include "packet.pb.h"
#include <google/protobuf/util/time_util.h>
//#include <N2kMessagesEnumToStr.h>
using namespace std;
using google::protobuf::util::TimeUtil;
using namespace tutorial;
typedef struct {
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg); 
} tNMEA2000Handler;

tSocketStream serStream; 

void Temperature(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg);
void Humidity(const tN2kMsg &N2kMsg);
void TemperatureExt(const tN2kMsg &N2kMsg);
void printPacket(const Packet& packet);

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
  //{130310L,&OutsideEnvironmental},
  {130312L,&Temperature},
  //{130313L,&Humidity},
  //{130316L,&TemperatureExt},
  {0,0}
};


void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double WaterTemperature;
    double OutsideAmbientAirTemperature;
    double AtmosphericPressure;
    
    if (ParseN2kOutsideEnvironmentalParameters(N2kMsg,SID,WaterTemperature,OutsideAmbientAirTemperature,AtmosphericPressure) ) {
      PrintLabelValWithConversionCheckUnDef("Water temp: ",WaterTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(", outside ambient temp: ",OutsideAmbientAirTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(", pressure: ",AtmosphericPressure,0,true);
    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }
}
//*****************************************************************************
void Temperature(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;

    tutorial::Packet packet;
    
    if (ParseN2kTemperature(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                        //serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
      PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);

      tutorial::Packet_Data* data;

      data->set_type("temperature");

      tutorial::Packet_DataField* data_field = data->add_fields();

      data_field->set_label("temperature");
      data_field->set_value(ActualTemperature);

      packet.set_allocated_data(data);

      packet.set_board_id(123);
      
      // Set timestamp
      *packet.mutable_time_data_read() = TimeUtil::GetCurrentTime();



      // Set protocol
      packet.set_protocol(tutorial::Packet::CAN);

      // Set original message
      packet.set_original_message("\xDE\xAD\xBE\xEF", 4);

      // Set digital signature
      packet.set_digital_signature(9876543210);

      // Print the packet
      printPacket(packet);


    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }


}

//*****************************************************************************
void Humidity(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kHumiditySource HumiditySource;
    double ActualHumidity,SetHumidity;
    
    if ( ParseN2kHumidity(N2kMsg,SID,Instance,HumiditySource,ActualHumidity,SetHumidity) ) {
                       // serStream.print("Humidity source: "); PrintN2kEnumType(HumiditySource,serStream,false);
      PrintLabelValWithConversionCheckUnDef(", humidity: ",ActualHumidity,0,false);
      PrintLabelValWithConversionCheckUnDef(", set humidity: ",SetHumidity,0,true);
    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }
}

void TemperatureExt(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    
    if (ParseN2kTemperatureExt(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                      //  serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
      PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);
    } else {
      serStream.print("Failed to parse PGN: ");  serStream.println(N2kMsg.PGN);
    }
}

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  int iHandler;
  
  // Find handler
  serStream.print("In Main Handler: "); serStream.println(N2kMsg.PGN);
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  
  if (NMEA2000Handlers[iHandler].PGN!=0) {
    NMEA2000Handlers[iHandler].Handler(N2kMsg); 
  }
}

// Function to print the Packet message in a readable format
void printPacket(const Packet& packet) {
    cout << "Packet Message:" << endl;
    cout << "  Board ID: " << packet.board_id() << endl;

    // Convert timestamp to a human-readable string
    cout << "  Time Data Read: " << TimeUtil::ToString(packet.time_data_read()) << endl;

    // Print Data fields
    if (packet.has_data()) {
        cout << "  Data:" << endl;
        cout << "    Type: " << packet.data().type() << endl;
        cout << "    Fields:" << endl;
        for (const auto& field : packet.data().fields()) {
            cout << "      - " << field.label() << ": " << field.value() << endl;
        }
    }

    // Print protocol
    cout << "  Protocol: ";
    switch (packet.protocol()) {
        case Packet::CAN: cout << "CAN"; break;
        case Packet::Modbus: cout << "Modbus"; break;
        default: cout << "Unknown";
    }
    cout << endl;

    // Print original message as hex
    cout << "  Original Message: ";
    for (unsigned char c : packet.original_message()) {
        printf("%02X ", c);
    }
    cout << endl;

    // Print digital signature
    cout << "  Digital Signature: " << packet.digital_signature() << endl;
}



int main(void)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    
    cout << "Starting CAN watching" << endl;

    setvbuf (stdout, NULL, _IONBF, 0);                                          // No buffering on stdout, just send chars as they come.
 
   
    NMEA2000.SetForwardStream(&serStream);                                      // Connect bridge function for streaming output.
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