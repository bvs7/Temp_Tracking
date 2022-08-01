
#include "connection.h"

#include "connection_secret.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2,3); // RX, TX
#endif

#define AT_BAUD_RATE 9600

int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiEspClient wifi_client;
PubSubClient mqtt_client(wifi_client);

unsigned long reconnect_time_millis = 0;

bool wifi_connect(){
  WiFi.init(&Serial1);
  // check for the presence of the ESP8266
  if (WiFi.status() == WL_NO_SHIELD) {
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
  
  Serial.println("Connected to Wifi");
  return true;
}

//void mqtt_callback(char* topic, byte* payload, unsigned int length) {
//  dot();
//}

void mqtt_onConnect(){
  mqtt_client.subscribe("cmd/irrigation/garden_station0");
  Serial.println("Subscribed to station topic");
}

bool mqtt_connect(){
  Serial.print("Attempting MQTT connection...");
  delay(100);
  if(mqtt_client.connect(conn_sett.mqtt_id)){
    Serial.println("Connected to mqtt server");
    mqtt_onConnect();
  }else{
    Serial.print("failed, rc=");
    Serial.print(mqtt_client.state());
    Serial.println("...");
    return false;
  }
  return true;
}

bool reconnect(){  
  if(wifi_client.connected() && mqtt_client.connected()){
    return true;
  }
  
  if(!wifi_client.connected()){
    if(!wifi_connect()){
      dash(); space();space(); dash(); space();space(); dash(); space();space();space();// -- represents failed wifi connection
      return false;
    }
  }

  if(!mqtt_client.connected()){
    //dot(); dot(); dot(); space(); // ... represents missing mqtt server connection
    if(!mqtt_connect()){
      dash(); space();space(); dot(); dash(); space();space(); dot(); dash(); space();space();space();
      // --- represents failed mqtt server connection
      return false;
    }
  }
  dot(); dot(); dot(); dot(); dot(); dot();space(); space(); 
  dot(); dot(); dot(); dot(); dot(); dot();space(); space(); // .-.- represents successfully connected
  return true;
}

void connection_setup(){
  // initialize serial for ESP module
  Serial1.begin(AT_BAUD_RATE);
  
  mqtt_client.setServer(conn_sett.server, 1883);

  delay(100);
}

void connection_loop(){
  if(mqtt_client.connected()){
    mqtt_client.loop();
  }else{
    if(millis() > reconnect_time_millis){
      reconnect();
      reconnect_time_millis = millis() + 60000;
    }
  }


}
