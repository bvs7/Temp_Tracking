
#include "Arduino.h"

#include "WiFiEsp.h"

// Enable MqttClient logs
#define MQTT_LOG_ENABLED 1
// Include library
#include "MqttClient.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#ifdef ESP8266_SERIAL
#define DEBUG_SERIAL Serial1
#define WIFI_SERIAL Serial
#else
#define DEBUG_SERIAL Serial
#define WIFI_SERIAL Serial1
#endif

// TODO: put this in a .h file not in github
#define SSID "gemini"
#define PASS "123456"

char ssid[] = SSID;
char pass[] = PASS; 
int status = WL_IDLE_STATUS;     // the Wifi radio's status

#define HW_UART_SPEED		    115200L
#define ESP8266_SPEED         9600L
#define MQTT_ID						"TEST-ID"

static MqttClient *mqtt = NULL;
static WiFiEspClient client;

void wifi_setup(){
    // Setup WiFi network
  WIFI_SERIAL.begin(ESP8266_SPEED);
  WiFi.init(&WIFI_SERIAL);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    DEBUG_SERIAL.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  int status = WL_IDLE_STATUS;
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    DEBUG_SERIAL.print("Attempting to connect to WPA SSID: ");
    DEBUG_SERIAL.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
}

// ============== Object to supply system functions ============================
class System: public MqttClient::System {
public:

	unsigned long millis() const {
		return ::millis();
	}

	void yield(void) {
		::yield();
	}
};

// ============== Setup all objects ============================================
void setup() {
	// Setup hardware serial for logging
	DEBUG_SERIAL.begin(HW_UART_SPEED);
	while (!DEBUG_SERIAL);

	wifi_setup();

	// Setup MqttClient
	MqttClient::System *mqttSystem = new System;
	MqttClient::Logger *mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(DEBUG_SERIAL); //
	MqttClient::Network *mqttNetwork = new MqttClient::NetworkClientImpl<WiFiEspClient>(client, *mqttSystem);
	//// Make 128 bytes send buffer
	MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
	//// Make 128 bytes receive buffer
	MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<128>();
	//// Allow up to 2 subscriptions simultaneously
	MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
	//// Configure client options
	MqttClient::Options mqttOptions;
	////// Set command timeout to 10 seconds
	mqttOptions.commandTimeoutMs = 10000;
	//// Make client object
	mqtt = new MqttClient(
		mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
		*mqttRecvBuffer, *mqttMessageHandlers
	);
}

// ============== Main loop ====================================================
void loop() {
	// Check connection status
	if (!mqtt->isConnected()) {
		// Close connection if exists
		client.stop();
		// Re-establish TCP connection with MQTT broker
		client.connect("test.mosquitto.org", 1883);
		if (!client.connected()) {
			delay(5000);
		}
		// Start new MQTT connection
		MqttClient::ConnectResult connectResult;
		// Connect
		{
			MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
			options.MQTTVersion = 4;
			options.clientID.cstring = (char*)MQTT_ID;
			options.cleansession = true;
			options.keepAliveInterval = 15; // 15 seconds
			MqttClient::Error::type rc = mqtt->connect(options, connectResult);
			if (rc != MqttClient::Error::SUCCESS) {
				return;
			}
		}
		{
			// Add subscribe here if required
		}
	} else {
		{
			// Add publish here if required
		}
		// Idle for 30 seconds
		mqtt->yield(30000L);
	}
}
