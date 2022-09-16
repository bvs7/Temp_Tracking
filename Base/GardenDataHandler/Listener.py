from paho.mqtt.client import Client as MQTTClient, MQTTMessage
import sqlite3
import os
from configparser import ConfigParser
from typing import Tuple, Optional
from datetime import datetime
import logging

from .DBAccessor import DBAccessor

config = ConfigParser()
config.read('GardenDataHandler/config.ini')

logging.basicConfig(
    level=config.get('logging', 'LogLevel'),
    filename=config.get('logging', 'LogFile')
)

GARDEN_DATA_TOPIC = config.get("mqtt", "GardenDataTopic")

PORT_STATES = ["DISABLED", "IDLE!", "FORCED!", "ENABLED"]
PORT_DEFAULT = "UNKNOWN"



class Listener(MQTTClient):
    """MQTT Client that listens for Garden Device Data. When data is received,
    an event is stored in a database, and the device state is updated."""

    def __init__(self, db_fname=None, dir_name = None, 
                 DBAccessorClass=DBAccessor,
                 MQTTClientArgs=None, ):
        if not MQTTClientArgs:
            MQTTClientArgs = {}
        super().__init__(self.__class__.__name__, **MQTTClientArgs)
        self.db_accessor = DBAccessorClass(db_fname, dir_name)

    def run(self):
        """Run the MQTT client. Blocking."""
        logging.debug(
            f"Starting {self.__class__.__name__}. Connecting to MQTT")
        self.connect(
            config.get('mqtt', 'BrokerAddress'),
            config.getint('mqtt', 'BrokerPort'),
            config.getint('mqtt', 'BrokerKeepAlive')
        )
        logging.debug("Connected to MQTT.")
        self.loop_forever()

    def on_connect(self, client: MQTTClient, userdata, flags, rc):
        logging.info(f"Connected to MQTT broker with result code {rc}")
        # Connection results:
        #  0: Connection successful
        #  1: Connection refused - incorrect protocol version
        #  2: Connection refused - invalid client identifier
        #  3: Connection refused - server unavailable
        #  4: Connection refused - bad username or password
        #  5: Connection refused - not authorised
        logging.info(f"Subscribing to topic {GARDEN_DATA_TOPIC}")
        client.subscribe(GARDEN_DATA_TOPIC, qos=1)

    def on_message(self, client: MQTTClient, userdata, msg: MQTTMessage):
        logging.info(f"Received message on topic {msg.topic}: {msg.payload}")
        topic = msg.topic
        topic_levels = topic.split("/")
        assert all([len(topic_levels) == 4, topic_levels[0] == "data",
                   topic_levels[1] == "garden"]), "Invalid topic!"
        payload = msg.payload.decode("utf-8")
        station_name = topic_levels[2]
        device_type = topic_levels[3][0]
        device_idx = int(topic_levels[3][1:])
        event_time = datetime.now()
        p_state = None if device_type != "P" else \
            payload if payload in PORT_STATES else PORT_DEFAULT
        a_value = None if device_type != "A" else int(payload)

        self.db_accessor.submit_event(station_name, device_type, device_idx,
                                      event_time, p_state, a_value)

