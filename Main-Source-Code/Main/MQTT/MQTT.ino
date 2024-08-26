#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "Wi likes Fi"; // Enter your Wi-Fi name
const char *password = "mihiran1";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "137.184.9.146";

const char *topic = "Energy_moniter/moniter_1/phase_A/Power";
const char *phase_A_Power = "Energy_moniter/moniter_1/phase_A/Power";
const char *phase_A_Volatge = "Energy_moniter/moniter_1/phase_A/Voltge";
const char *heart_beat = "Energy_moniter/moniter_1/heart_beat";
const char *status = "topic/status";
const char *mqtt_username = "mihiran";
const char *mqtt_password = "mihiran";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
const int cmd = 13;

const int status_ = 12;


int status_status;
int status_old = 0;
int status_new;

void setup() {
  pinMode(cmd, OUTPUT);
  pinMode(status_, INPUT);

  
  digitalWrite(cmd,LOW);
    // Set software serial baud to 115200;
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
    // Publish and subscribe
    client.publish(topic, "100");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  String word; 
 for (int i = 0; i < length; i++) {
        word += ((char) payload[i]);
    }
    Serial.println(word);
}






void loop() {
  client.publish(heart_beat, "100");
  client.publish(phase_A_Power, "100");
  delay(100);
  client.publish(phase_A_Volatge, "368");
  delay(100);
  client.publish(phase_A_Power, "25");
  delay(100);
  client.publish(phase_A_Volatge, "402");
  delay(100);
  client.publish(phase_A_Power, "2");
  delay(100);
  client.publish(phase_A_Volatge, "400");




  client.loop();
}








