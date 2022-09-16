from paho.mqtt.client import Client as MQTTClient
import sys

if __name__ == "__main__":
    client = MQTTClient()
    client.connect("localhost", 1883, 60)
    client.publish(sys.argv[1], sys.argv[2])
    client.disconnect()
