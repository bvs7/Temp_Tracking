from paho.mqtt.client import Client as MQTTClient, MQTTMessage
import sqlite3
import os
from configparser import ConfigParser
from typing import Tuple, Optional
from datetime import datetime
import logging

from .Listener import Listener
from .DBAccessor import DBAccessor

config = ConfigParser()
config.read('GardenDataHandler/config.ini')

logging.basicConfig(
    level=config.get('logging', 'LogLevel'),
    filename=config.get('logging', 'LogFile')
)

if __name__ == "__main__":
    logging.log(logging.CRITICAL, "\n\n START \n")
    try:
        Listener(
            config.get('data', 'DatabaseFileName'),
            config.get('data', 'DirectoryName')
        ).run()
    except KeyboardInterrupt as ki:
        logging.info("Exiting...")
    except Exception as e:
        logging.error(e, exc_info=True)
