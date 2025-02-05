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
//#include <N2kMessagesEnumToStr.h>
using namespace std;

typedef struct {
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg); 
} tNMEA2000Handler;

tSocketStream serStream; 

void Temperature(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg);
void Humidity(const tN2kMsg &N2kMsg);
void TemperatureExt(const tN2kMsg &N2kMsg);

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
    
    if (ParseN2kTemperature(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                        //serStream.print("Temperature source: "); PrintN2kEnumType(TempSource,serStream,false);
      PrintLabelValWithConversionCheckUnDef(", actual temperature: ",ActualTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(", set temperature: ",SetTemperature,&KelvinToC,true);
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

int main(void)
{
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