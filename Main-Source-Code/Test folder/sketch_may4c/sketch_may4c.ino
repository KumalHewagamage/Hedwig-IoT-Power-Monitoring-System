/* ATM90E36 Energy Monitor Demo Application

   The MIT License (MIT)

  Copyright (c) 2016 whatnick and Ryzee

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <SPI.h>
#include "ATM90E36.h"
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "Galaxy M029ae8"; // Enter your Wi-Fi name
const char *password = "12345678";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "137.184.9.146";

const char *topic = "Energy_moniter/moniter_1/phase_3";
const char *phase_A_Power = "Energy_moniter/moniter_1/phase_A/Power";
//voltage topics
const char *phase_A_Volatge = "Energy_moniter/moniter_1/phase_A/Voltge";
const char *phase_B_Volatge = "Energy_moniter/moniter_1/phase_B/Voltge";
const char *phase_C_Volatge = "Energy_moniter/moniter_1/phase_C/Voltge";

//Current Topics
const char *phase_A_Current = "Energy_moniter/moniter_1/phase_A/Current";
const char *phase_B_Current = "Energy_moniter/moniter_1/phase_B/Current";
const char *phase_C_Current = "Energy_moniter/moniter_1/phase_C/Current";


const char *heart_beat = "Energy_moniter/moniter_1/heart_beat";
const char *status = "topic/status";
const char *mqtt_username = "mihiran";
const char *mqtt_password = "mihiran";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);



ATM90E36 eic(7);






















void setup() {
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);
  digitalWrite(17,HIGH);
  digitalWrite(18,HIGH);
  Serial.begin(115200);
    // Connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
  
//My Code
// Define MISO, MOSI, and SCK pins
const int misoPin = 6;
const int mosiPin = 5;
const int sckPin = 4;
const int ss = 7;

  // Start SPI with specified settings
  SPI.begin(sckPin, misoPin, mosiPin, ss);

  // Set SPI settings
  SPISettings settings(200000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(settings);
// My end



  /* Initialize the serial port to host */
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  

  
  Serial.println("Start ATM90E36");
  /*Initialise the ATM90E36 + SPI port */
  eic.begin();
  
  delay(1000);
}

void callback(char *topic, byte *payload, unsigned int length) {
  String word; 
 for (int i = 0; i < length; i++) {
        word += ((char) payload[i]);
    }
    Serial.println(word);
}


void loop() {
  digitalWrite(17,HIGH);
  digitalWrite(18,HIGH);
   client.publish(topic, "Working");
  
  /*Repeatedly fetch some values from the ATM90E36 */
  double voltageA,freq,voltageB,voltageC,currentA,currentB,currentC,power,pf,new_current,new_power;
  int sys0=eic.GetSysStatus0();
  int sys1=eic.GetSysStatus1();
  int en0=eic.GetMeterStatus0();
  int en1=eic.GetMeterStatus1();

  String message = "S0:0x" + String(sys0, HEX);
 
  client.publish(topic, message.c_str());
  Serial.println("S0:0x"+String(sys0,HEX));
  delay(10);
  Serial.println("S1:0x"+String(sys1,HEX));
  message = "S1:0x"+String(sys1,HEX);
  client.publish(topic, message.c_str());
  delay(10);
  Serial.println("E0:0x"+String(en0,HEX));
   message = "E0:0x"+String(en0,HEX);
  client.publish(topic, message.c_str());
  delay(10);
  Serial.println("E1:0x"+String(en1,HEX));
  message = "E1:0x"+String(en1,HEX);
  client.publish(topic, message.c_str());
  
  //VolatgeA RMS
  voltageA=eic.GetLineVoltageA();
  Serial.println("VA:"+String(voltageA)+"V");
  message =  "VA:"+String(voltageA)+"V";
  client.publish(phase_A_Volatge,message.c_str());

  //VoltageB RMS
  voltageB=eic.GetLineVoltageB();
  Serial.println("VB:"+String(voltageB)+"V");
  message =  "VB:"+String(voltageB)+"V";
  client.publish(phase_B_Volatge,message.c_str());

  //VoltageC Rms
  voltageC=eic.GetLineVoltageC();
  Serial.println("VC:"+String(voltageC)+"V");
  message =  "VC:"+String(voltageC)+"V";
  client.publish(phase_C_Volatge,message.c_str());



  //Currents
  currentA = eic.GetLineCurrentA();
  Serial.println("IA:"+String(currentA)+"A");
  message =  String(currentA);
  client.publish(phase_A_Current,message.c_str());

  currentB = eic.GetLineCurrentB();
  Serial.println("IB:"+String(currentB)+"A");
  message =  String(currentB);
  client.publish(phase_B_Current,message.c_str());

  currentC = eic.GetLineCurrentC();
  Serial.println("IC:"+String(currentC)+"A");
  message =  String(currentC);
  client.publish(phase_C_Current,message.c_str());
  
  digitalWrite(17,LOW);
  digitalWrite(18,LOW);
  delay(1000);
}