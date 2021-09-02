from paho.mqtt.client import Client as MQTTClient
import sqlite3
import time
import logging

DATABASE_FNAME = "../data/garden_data.db"

LOG_FNAME = "log/garden_data_client.log"

GARDEN_STATUS_TOPIC = "home/garden/+/+/status"

create_table = \
"""CREATE TABLE {table_name} (
  id integer PRIMARY KEY AUTOINCREMENT, 
  time timestamp NOT NULL, 
  status {status_type}  NOT NULL)"""

basic_insert = \
"""INSERT INTO {table_name} (time, status) VALUES (CURRENT_TIMESTAMP, {status})"""

logging.basicConfig(level=logging.DEBUG, filename=LOG_FNAME)

class GardenDataClient(MQTTClient):

  def on_connect(self, client, userdata, flags, rc, properties=None):
    logging.debug(f"GardenLoggerClient.on_connect: {client},{userdata},{flags},{rc},{properties}")
    # Subscribe to all tracked data
    client.subscribe(GARDEN_STATUS_TOPIC)
    client.message_callback_add(GARDEN_STATUS_TOPIC, self.on_status_topic)
    self.con = sqlite3.connect(DATABASE_FNAME)

  def on_disconnect(self, client, userdata, rc):
    logging.debug(f"GardenLoggerClient.on_disconnect: {client},{userdata},{rc}")
    self.con.commit()
    self.con.close()

  def on_subscribe(self, client, userdata, mid, granted_qos, properties=None):
    logging.debug(f"GardenLoggerClient.on_subscribe: {client},{userdata},{mid},{granted_qos},{properties}")

  def on_status_topic(self, client, userdata, message):
    logging.debug(f"GardenLoggerClient.on_status_topic: {client}, {userdata}, {message.topic}, {message.payload.decode('utf-8')}")
    topic_words = message.topic.split("/")
    table_name = "_".join(topic_words[2:-1]).replace('-',"") # Assume home/garden/[type]/[name]/status format
    status = message.payload.decode("utf-8")
    try:
      status = int(status)
    except ValueError:
      try:
        status = float(status)
      except ValueError:
        pass
    self.submit_data(table_name, status)

  def on_message(self, client, userdata, message):
    logging.debug(f"GardenLoggerClient.on_message: {client}, {userdata}, {message.topic}, {message.payload.decode('utf-8')}")

  def submit_data(self, table_name, status):
    cur = self.con.cursor()
    if type(status) == str:
      status = f"\"{status}\""
    while True:
      try:
        command = basic_insert.format(table_name=table_name,status=status)
        logging.debug(f"SQL Command: {command}")
        cur.execute(command)
        break
      except sqlite3.OperationalError as oe:
        logging.warning(f"Error {oe} while writing to {table_name}. Creating table")
        status_type_dict = {str:"TEXT",float:"REAL",int:"INTEGER"}
        cur.execute(create_table.format(
          table_name=table_name, 
          status_type=status_type_dict[type(status)]))
        self.con.commit()
    self.con.commit()
    cur.close()


client = GardenDataClient()
client.connect("localhost")
while True:
  try:
    client.loop()
  except Exception as e:
    logging.exception("Exception while looping GardenDataClient")