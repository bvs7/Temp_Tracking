from configparser import ConfigParser
from datetime import datetime
import logging

from .util import *

from .DBAccessor import DBAccessor


class Analyzer:
    """Get data from the database, including current state, 
    on time for a day, etc"""

    def __init__(self, db_fname=None, dir_name = None, 
                 DBAccessorClass=DBAccessor):
        if db_fname is None:
            db_fname = config.get('data', 'DatabaseFileName')
        if dir_name is None:
            dir_name = config.get('data', 'DirectoryName')
        self.db_accessor = DBAccessorClass(db_fname, dir_name)
        self.names = {}

    def update_device_names(self):
        """Update the device names"""
        self.names = {}
        for name, station_name, device_type, device_idx\
            in self.db_accessor.get_device_names():
            self.names[name] = \
                (station_name, device_type, device_idx)

    def get_current_state(self, device_name:str):
        """Get the current state of a device"""
        self.update_device_names()
        if not device_name in self.names:
            return None
        else:
            station_name, device_type, device_idx = self.names[device_name]
            return self.db_accessor.get_state(
                station_name, device_type, device_idx)

    def get_on_time(self, device_name:str, date:datetime):
        """Get the on time for a device on a given date"""
        self.update_device_names()
        if not device_name in self.names:
            return None
        else:
            station_name, device_type, device_idx = self.names[device_name]
            events = self.db_accessor.get_events_date(
                station_name, device_type, device_idx, date)
            # Record the on time for the day
            p_on_times = dict((s, 0) for s in PORT_STATES)
            p_on_times[PORT_DEFAULT] = 0
            last_time = datetime(date.year, date.month, date.day, 0, 0, 0)
            # TODO get initial state from database
            last_state = PORT_STATES[0]
            for event in events:
                _, _, _, time_str, state, value  = event
                time = datetime.strptime(time_str, '%Y-%m-%d %H:%M:%S.%f')
                if device_type == "P":
                    p_on_times[last_state] += (time - last_time).total_seconds()
                    last_time = time
                    last_state = state

            _, state, value = self.db_accessor.get_state(station_name, device_type, device_idx)
            p_on_times[last_state] += (datetime(date.year, date.month, date.day, 23, 59, 59) - last_time).total_seconds()
            return p_on_times


                

        