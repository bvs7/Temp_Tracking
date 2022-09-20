import sqlite3
import os
from typing import Tuple
from datetime import datetime

from .util import *


class DBAccessor():
    DeviceEvents_Create = """
    CREATE TABLE IF NOT EXISTS DeviceEvents(
        station_name    VARCHAR(16) NOT NULL,
        device_type     TEXT NOT NULL,
        device_idx      INT NOT NULL,
        event_time      DATETIME NOT NULL,
        p_state         TEXT,
        a_value         INT
    )"""

    DeviceEvents_Insert = \
        "INSERT INTO DeviceEvents (station_name, device_type, device_idx,"\
        "event_time, p_state, a_value) VALUES (?,?,?,?,?,?)"

    DeviceEvents_Select_Date = \
        "SELECT * FROM DeviceEvents WHERE (station_name, device_type, "\
        "device_idx) = (?,?,?) AND date(event_time) = date(?)"

    DeviceEvents_Select_Since = \
        "SELECT * FROM DeviceEvents WHERE (station_name, device_type, "\
        "device_idx) = (?,?,?) AND event_time >= ?"

    def __init__(self, db_fname: str, dir_name: str):
        self.db_fname = db_fname
        self.dir_name = dir_name

        self.create_table()

    def create_table(self):
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            cursor.execute(self.DeviceEvents_Create)
            # cursor.execute(self.DeviceStates_Create)
            # cursor.execute(self.DeviceNames_Create)

    def drop_table(self):
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            cursor.execute("DROP TABLE IF EXISTS DeviceEvents;")
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

    def get_device_names(self):
        path = os.path.join(self.dir_name)
        for station_name in os.listdir(path):
            for device_id in os.listdir(os.path.join(path, station_name)):
                device_type = device_id[0]
                device_idx = int(device_id[1:])
                device_name = self.get_device_name(station_name, device_type,
                                                    device_idx)
                yield device_name, station_name, device_type, device_idx

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
        results = [None, None, None]
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

    def put_event(self, station_name: str, device_type: str,
                  device_idx: int, event_time: datetime,
                  p_state: str, a_value: int):
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            data = (station_name, device_type, device_idx, event_time, p_state, a_value)
            cursor.execute(self.DeviceEvents_Insert, data)

    def get_events_date(self, station_name: str, device_type: str,
                          device_idx: int, event_time: datetime) -> Tuple[str, int]:
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            data = (station_name, device_type, device_idx, event_time)
            cursor.execute(self.DeviceEvents_Select_Date, data)
            rows = cursor.fetchall()
            return rows

    def get_events_since(self, station_name:str, device_type:str,
                        device_idx:int, since_time:datetime) -> Tuple[str, int]:
        with sqlite3.connect(self.db_fname) as conn:
            cursor = conn.cursor()
            data = (station_name, device_type, device_idx, since_time)
            cursor.execute(self.DeviceEvents_Select_Since, data)
            rows = cursor.fetchall()
            return rows

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
                self.put_device_name(device_name, station_name, device_type, device_idx)
                logging.info(f"New Device Named: {device_name}")
            if device_name != display_device_name:
                display_device_name = \
                    f"{device_name} ({display_device_name})"
        logging.info(
            f"{display_device_name} Event:"
            f"{display_value} @ {event_time}")

