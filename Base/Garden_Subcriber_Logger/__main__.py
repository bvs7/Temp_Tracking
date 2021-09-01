from paho.mqtt.client import Client as MQTTClient
import logging

logging.basicConfig(level=logging.DEBUG)

class GardenLoggerClient(MQTTClient):

  @staticmethod
  def on_connect(client, userdata, flags, rc, properties=None):
    logging.debug(f"GardenLoggerClient.on_connect: {client},{userdata},{flags},{rc},{properties}")
    # Subscribe to all tracked data
    client.subscribe("home/garden/#")

  @staticmethod
  def on_subscribe(client, userdata, mid, granted_qos, properties=None):
    logging.debug(f"GardenLoggerClient.on_subscribe: {client},{userdata},{mid},{granted_qos},{properties}")

  @staticmethod
  def on_message(client, userdata, message):
    logging.debug(f"GardenLogger.on_message: {client}, {userdata}, {message.topic}, {message.payload.decode('utf-8')}")

client = GardenLoggerClient()

client.connect("localhost")
while True:
  client.loop()
