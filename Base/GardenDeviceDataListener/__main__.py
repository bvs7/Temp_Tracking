from paho.mqtt.client import Client as MQTTClient, MQTTMessage
import sqlite3
import os
from configparser import ConfigParser
from typing import Tuple, Optional
from datetime import datetime
import logging


config = ConfigParser()
config.read('GardenDeviceDataListener/config.ini')

logging.basicConfig(
    level=config.get('logging', 'LogLevel'),
    filename=config.get('logging', 'LogFileName')
)

GARDEN_DATA_TOPIC = "data/garden/+/+"

PORT_STATES = ["DISABLED", "IDLE!", "FORCED!", "ENABLED"]
PORT_DEFAULT = "UNKNOWN"


class GardenDeviceDBAccessor():
    DeviceEvents_Create = """
    CREATE TABLE IF NOT EXISTS DeviceEvents(
        station_name    VARCHAR(16) NOT NULL,
        device_type     TEXT NOT NULL,
        device_idx      INT NOT NULL,
        event_time      DATETIME NOT NULL,
        p_state         TEXT,
        a_value         INT
    );"""

    DeviceStates_Create = """
    CREATE TABLE IF NOT EXISTS DeviceStates (
        station_name    VARCHAR(16) NOT NULL,
        device_type     TEXT CHECK('P', 'A') NOT NULL,
        device_idx      INT NOT NULL,
        last_update     DATETIME NOT NULL,
        p_state         TEXT CHECK("DISABLED", "IDLE!", "FORCED!", "ENABLED", "UNKNOWN"),
        a_value         INT,
        PRIMARY KEY (station_name, device_type, device_id)
    );"""

    DeviceNames_Create = """
    CREATE TABLE IF NOT EXISTS DeviceNames (
        station_name    VARCHAR(16) NOT NULL,
        device_type     TEXT CHECK('P', 'A') NOT NULL,
        device_idx      INT NOT NULL,
        device_name     VARCHAR(255) NOT NULL,
        PRIMARY KEY (station_name, device_type, device_id)
    );"""

    DeviceEvents_Insert = \
        "INSERT INTO DeviceEvents (station_name, device_type, device_idx,"\
        "event_time, p_state, a_value) VALUES (?,?,?,?,?,?)"

    DeviceStates_Replace = \
        "INSERT OR REPLACE INTO DeviceStates (station_name, device_type, " \
        "device_idx, last_update, p_state, a_value) VALUES ({station_name}, " \
        "{device_type}, {device_idx}, {last_update}, {p_state}, {a_value}) "

    DeviceStates_Select = \
        "SELECT last_update, p_state, a_value FROM DeviceStates WHERE " \
        "(station_name, device_type, device_idx) = ({station_name}, " \
        "{device_type}, {device_idx})"

    DeviceNames_Select = \
        "SELECT device_name FROM DeviceNames WHERE (station_name, device_type," \
        "device_idx) = ({station_name}, {device_type}, {device_idx})"

    DeviceNames_Insert = \
        "INSERT INTO DeviceNames (device_name station_name, device_type, " \
        "device_idx) VALUES ({device_name}, {station_name}, {device_type}," \
        "{device_idx})"

    def __init__(self, db_fname: str, dir_name: str):
        self.db_fname = db_fname
        self.dir_name = dir_name

    def create_tables(self):
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            cursor.execute(self.DeviceEvents_Create)
            # cursor.execute(self.DeviceStates_Create)
            # cursor.execute(self.DeviceNames_Create)

    def drop_tables(self):
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            cursor.execute("DROP TABLE IF EXISTS DeviceEvents")
            # cursor.execute("DROP TABLE IF EXISTS DeviceStates")
            # cursor.execute("DROP TABLE IF EXISTS DeviceNames")

    def check_dir(self, station_name: str, device_id: str):
        if not os.path.exists(self.dir_name):
            os.makedirs(self.dir_name)
        if not os.path.exists(os.path.join(self.dir_name, station_name)):
            os.makedirs(os.path.join(self.dir_name, station_name))
        if not os.path.exists(os.path.join(self.dir_name, station_name,
                                           device_id)):
            os.makedirs(os.path.join(self.dir_name, station_name, device_id))

    def get_device_name(self, station_name: str, device_type: str,
                        device_idx: int) -> str:
        device_id = f"{device_type}{device_idx}"
        self.check_dir(station_name, device_id)
        path = os.path.join(self.dir_name, station_name, device_id)
        try:
            with open(os.path.join(path, "device_name.txt"), "r") as f:
                return f.read()
        except FileNotFoundError:
            logging.warning(f"Device name file not found for {station_name}, "
                         f"{device_type}, {device_idx}")
            display_device_name = f"{station_name}/{device_type}{device_idx}"
            self.put_device_name(display_device_name, station_name,
                                 device_type, device_idx)
            return display_device_name

    def put_device_name(self, device_name: str, station_name: str,
                        device_type: str, device_idx: int):
        device_id = f"{device_type}{device_idx}"
        self.check_dir(station_name, device_id)
        path = os.path.join(self.dir_name, station_name, device_id)
        with open(os.path.join(path, "device_name.txt"), "w") as f:
            f.write(device_name)

    def get_state(self, station_name: str, device_type: str,
                  device_idx: int) -> Tuple[datetime, str, int]:
        device_id = f"{device_type}{device_idx}"
        self.check_dir(station_name, device_id)
        path = os.path.join(self.dir_name, station_name, device_id)
        results = (None, None, None)
        for i, fname in enumerate(("last_update.txt", "p_state.txt",
                                   "a_value.txt")):
            try:
                with open(os.path.join(path, fname), "r") as f:
                    results[i] = f.read()
            except FileNotFoundError:
                logging.warn(f"File not found: {fname}")
                return results
        last_update, p_state, a_value = results
        if a_value:
            a_value = int(a_value)
        return last_update, p_state, a_value

    def update_state(self, station_name: str, device_type: str,
                     device_idx: int, last_update: datetime, p_state: str,
                     a_value: int):
        device_id = f"{device_type}{device_idx}"
        self.check_dir(station_name, device_id)
        path = os.path.join(self.dir_name, station_name, device_id)
        with open(os.path.join(path, "last_update.txt"), "w") as f:
            f.write(last_update.isoformat())
        if p_state:
            with open(os.path.join(path, "p_state.txt"), "w") as f:
                f.write(p_state)
        if a_value:
            with open(os.path.join(path, "a_value.txt"), "w") as f:
                f.write(str(a_value))

    # def get_device_name(self, station_name: str, device_type: str,
    #                     device_idx: int) -> str:
    #     with sqlite3.connect(self.db_fname) as conn:
    #         cursor = conn.cursor()
    #         cursor.execute(self.DeviceNames_Select.format(*locals()))
    #         result = cursor.fetchone()
    #         if result is None:
    #             return None
    #         return result[0]

    # def put_device_name(self, device_name: str, station_name: str,
    #                     device_type: str, device_idx: int):
    #     with sqlite3.connect(self.db_fname) as conn:
    #         cursor = conn.cursor()
    #         cursor.execute(self.DeviceNames_Insert.format(*locals()))

    # def get_state(self, station_name: str, device_type: str,
    #               device_idx: int) -> Optional[Tuple[datetime, str, int]]:
    #     with sqlite3.connect(self.db_fname) as conn:
    #         cursor = conn.cursor()
    #         cursor.execute(self.DeviceStates_Select.format(*locals()))
    #         result = cursor.fetchone()
    #         if result is None:
    #             return None
    #         return result

    # def update_state(self, station_name: str, device_type: str,
    #                  device_idx: int, last_update: datetime,
    #                  p_state: str, a_value: int):
    #     with self.SQLConnectionFn(self.db_fname) as conn:
    #         cursor = conn.cursor()
    #         cursor.execute(self.DeviceStates_Replace.format(*locals()))

    def put_event(self, station_name: str, device_type: str,
                  device_idx: int, event_time: datetime,
                  p_state: str, a_value: int):
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            data = (station_name, device_type, device_idx, event_time, p_state, a_value)
            cursor.execute(self.DeviceEvents_Insert, data)

    def submit_event(self, station_name: str, device_type: str,
                  device_idx: int, event_time: datetime,
                  p_state: str, a_value: int):
        display_device_name = \
            "{station_name}/{device_type}{device_idx}".format(**locals())
        if not p_state == None:
            display_value = p_state
        elif not a_value == None:
            display_value = a_value
        else:
            display_value = None
        if self.db_fname:
            self.put_event(station_name, device_type, device_idx, event_time, p_state, a_value)
            self.update_state(station_name, device_type, device_idx, event_time, p_state, a_value)
            device_name = self.get_device_name(station_name, device_type, device_idx)
            if device_name is None:
                device_name = display_device_name
                self.put_device_name(device_name, station_name, device_type, device_idxs)
                logging.info(f"New Device Named: {device_name}")
            if device_name != display_device_name:
                display_device_name = \
                    f"{device_name} ({display_device_name})"
        logging.info(
            f"{display_device_name} Event:"
            f"{display_value} @ {event_time}")


class GardenDeviceDataListener(MQTTClient):
    """MQTT Client that listens for Garden Device Data. When data is received,
    an event is stored in a database, and the device state is updated."""

    def __init__(self, db_fname=None, dir_name = None, 
                 DBAccessorClass=GardenDeviceDBAccessor,
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


if __name__ == "__main__":
    logging.log(logging.CRITICAL, "\n\n START \n")
    try:
        GardenDeviceDataListener(
            config.get('data', 'DatabaseFileName'),
            config.get('data', 'DirectoryName')
        ).run()
    except KeyboardInterrupt as ki:
        logging.info("Exiting...")
    except Exception as e:
        logging.error(e, exc_info=True)
