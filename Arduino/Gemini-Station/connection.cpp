
#include "connection.h"

#include <EEPROM.h>
#include "utility.h"
#include "connection_secret.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2,3); // RX, TX
#endif

#define AT_BAUD_RATE 9600
#define CONNSETT_ADDR_START 0x20

connection_settings_t conn_sett;


WiFiEspClient wifi_client;
PubSubClient mqtt_client(wifi_client);

void mqtt_on_connect_def(){
  // Default
}

void (*mqtt_on_connect)(void) = &mqtt_on_connect_def;

bool wifi_connect(){

  static int status = WL_IDLE_STATUS;

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

bool mqtt_connect(){
  mqtt_client.setServer(conn_sett.server, 1883);
  Serial.print("Attempting MQTT connection... Server: ");
  Serial.println("conn_sett.server");
  if(mqtt_client.connect(conn_sett.mqtt_id)){
    Serial.println("Connected to MQTT server!");
    mqtt_on_connect();
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

void save_conn(){
  EEPROM.put(CONNSETT_ADDR_START, conn_sett);
}

void connection_setup(void (*mqtt_on_connect_)(void)){
  // initialize serial for ESP module
  Serial1.begin(AT_BAUD_RATE);
  mqtt_on_connect = mqtt_on_connect_;
  EEPROM.get(CONNSETT_ADDR_START, conn_sett);
}

void connection_loop(){
  while(!reconnect()); // Reconnect

  mqtt_client.loop();
}
