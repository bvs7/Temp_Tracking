from configparser import ConfigParser
from datetime import datetime, timedelta
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
        self.names = {} # device_name -> (station_name, device_type, device_idx)

    def current(self):
        """Get the current state of all devices"""
        self.update_device_names()
        return {name: self.get_current_state(name) for name in self.names}

    def get_device_names(self):
        """Get the names of all devices"""
        self.update_device_names()
        return list(self.names.keys())

    def update_device_names(self):
        """Update the device names"""
        self.names = {}
        for name, station_name, device_type, device_idx\
            in self.db_accessor.get_device_names():
            self.names[name] = \
                (station_name, device_type, device_idx)

    def get_type(self, device_name:str):
        """Get the type of a device"""
        self.update_device_names()
        if not device_name in self.names:
            return None
        else:
            station_name, device_type, device_idx = self.names[device_name]
            return device_type

    def get_current_state(self, device_name:str):
        """Get the current state of a device"""
        self.update_device_names()
        if not device_name in self.names:
            return None
        else:
            station_name, device_type, device_idx = self.names[device_name]

            event_time, p_state, a_value = self.db_accessor.get_state(
                station_name, device_type, device_idx)
            if device_type == "P":
                return event_time, p_state
            elif device_type == "A":
                return event_time, a_value
            else:
                return None

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
            p_on_times[last_state] += (datetime(date.year, date.month, date.day+1, 0, 0, 0) - last_time).total_seconds()
            return p_on_times

    def get_history(self, device_name:str, days:int):
        """Get the state history for a device for a given time"""
        self.update_device_names()
        if not device_name in self.names:
            return None
        else:
            station_name, device_type, device_idx = self.names[device_name]
            since_time = datetime.now() - timedelta(days=days)
            events = self.db_accessor.get_events_since(
                station_name, device_type, device_idx, since_time)
            # Record the on time for the day
            if device_type == "P":
                return list([(event[3], event[4]) for event in events])
            elif device_type == "A":
                return [(event[3], event[5]) for event in events]
            else:
                return None
                

        