
/*
typedef enum {
	WL_NO_SHIELD = 255,
	WL_IDLE_STATUS = 0,
	//WL_NO_SSID_AVAIL,
	//WL_SCAN_COMPLETED,
	WL_CONNECTED,
	WL_CONNECT_FAILED,
	//WL_CONNECTION_LOST,
	WL_DISCONNECTED
} wl_status_t;
*/

#include "WiFiEsp.h"
#include "PubSubClient.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);  // RX, TX
#endif

char ssid[] = "gemini";  // your network SSID (name)
char pass[] = "VictorRalph";  // your network password
int status = WL_IDLE_STATUS;  // the Wifi radio's status

char mqtt_server[] = "192.168.0.30";
int mqtt_port = 1883;

WiFiEspClient wifi_client;
PubSubClient mqtt_client(wifi_client);

unsigned long lastMillis = 0;
unsigned long seconds = 0;

void onMessage(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connect(){
    Serial.print("checking wifi...");
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("done");
    Serial.print("Connecting to MQTT server.");
    int conn = 0;
    while(!conn){
        conn = mqtt_client.connect("testClient");
        delay(500);
        conn = mqtt_client.connected();
        Serial.print(".");
        delay(100);
    }
    Serial.println("");

    mqtt_client.loop();

    Serial.println("Connected to MQTT server.");

    mqtt_client.subscribe("testTopic/a", 0);
    mqtt_client.subscribe("testTopic/b", 0);
    mqtt_client.subscribe("testTopic/c", 0);

}

void setup(){

    Serial.begin(9600);
    Serial1.begin(9600);

    Serial.println("Start");

    mqtt_client.setServer(mqtt_server, mqtt_port);
    mqtt_client.setCallback(onMessage);

    WiFi.init(&Serial1);

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi device not found");
        // don't continue:
        while (true);
    }

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        Serial.println(status);
        delay(100);
    }

    printWifiStatus();

    delay(500);

    // Starting MQTT connection
    

}

void loop() {
    Serial.print("[");
    mqtt_client.loop();
    Serial.print("]");
    delay(10);

    if (!mqtt_client.connected()) {
        connect();
    }

    // publish a message roughly every second.
    if (millis() - lastMillis > 1000) {
        while(millis() - lastMillis > 1000){
            lastMillis += 1000;
        }
        seconds += 1;
        char sec_buf[10] = {0};
        itoa(seconds, sec_buf, 10);
        Serial.print("<");
        mqtt_client.publish("/hb", sec_buf);
        Serial.print(">");
    }
}