from configparser import ConfigParser
import logging

config = ConfigParser()
config.read('GardenDataHandler/config.ini')

logging.basicConfig(
    level=config.get('logging', 'LogLevel'),
    filename=config.get('logging', 'LogFile')
)

GARDEN_DATA_TOPIC = config.get("mqtt", "GardenDataTopic")

PORT_STATES = ["DISABLED", "IDLE!", "FORCED!", "ENABLED"]
PORT_DEFAULT = "UNKNOWN"
