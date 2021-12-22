
#include "connection.h"

#include "utility.h"


#include "connection_secret.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2,3); // RX, TX
#endif

#define AT_BAUD_RATE 9600

int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiEspClient wifi_client;
PubSubClient mqtt_client(wifi_client);

bool wifi_connect(){
  WiFi.init(&Serial1);
  Serial.println("Attempting to setup WiFi");
  // check for the presence of the ESP8266
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("ESP8266 not detected");
    return false;
  }

  int attempts = 0;
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(conn_sett.ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(conn_sett.ssid, conn_sett.pass);
    
    if(attempts > 10){
      Serial.println("Could not connect");
      return false;
    }
  }
  Serial.println("You're connected to the network!");
  return true;
}

//void mqtt_callback(char* topic, byte* payload, unsigned int length) {
//  dot();
//}

void mqtt_onConnect(){
  
}

bool mqtt_connect(){
  Serial.print("Attempting MQTT connection...");
  if(mqtt_client.connect(conn_sett.mqtt_id)){
    Serial.println("Connected!");
    mqtt_onConnect();
  }else{
    Serial.print("failed, rc=");
    Serial.print(mqtt_client.state());
    Serial.println("...");
  }
}

bool reconnect(){
  if(wifi_client.connected() && mqtt_client.connected()){
    return true;
  }
  dot(); space(); // . represents trying to reconnect
  if(!wifi_client.connected()){
    dot(); dot(); space(); // .. represents missing wifi connection
    if(!wifi_connect()){
      dash();dash();space(); space(); // -- represents failed wifi connection
      return false;
    }
  }

  if(!mqtt_client.connected()){
    dot(); dot(); dot(); space(); // ... represents missing mqtt server connection
    if(!mqtt_connect()){
      dash();dash();dash(); space(); space();
      // --- represents failed mqtt server connection
      return false;
    }
  }
  dot(); dash(); dot(); dash(); // .-.- represents successfully connected
  return true;
}

void connection_setup(){
  // initialize serial for ESP module
  Serial1.begin(AT_BAUD_RATE);
  
  mqtt_client.setServer(conn_sett.server, 1883);
  //mqtt_client.setCallback(mqtt_callback);
}

void connection_loop(){

  static byte n = 0;
  while(!reconnect()); // Reconnect

  mqtt_client.loop();

  if(!n++){
    mqtt_client.publish("TestTopic", "Hello World!");
    Serial.println("Sending testTopic publish");
  }
}
