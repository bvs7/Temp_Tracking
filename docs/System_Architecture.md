# gemini-home System Architecture

## Systems

### Garden Stations

The garden system deals with Arduino Stations that can turn on/off valves and a pump, and can sense via distance sensors and analog sensors

The stations are Arduino Pro Minis with ESP8266s for communication. They can read analog voltages, run a distance sensor, and sense and control a 12V port.

### Base

The base station will run on a Raspberry Pi. It will consist of...
- A Data Listener, which will receive data publications from the Stations
- A Station Controller, which can push commands to the Station or update stations on a schedule
- A Backend Server, which can access the system data, and do some processing

### App

Users will interface with the system to do the following:
- Edit Event Calendars (to turn on/off various systems)
- See current device states and modify them.
- See historical device data/graphs

## Communication

### Station MQTT

Stations are subscribed to `cmd/garden/<station_name>/#`

-`cmd/garden/<station_name>` sends a root command in payload
    - Response goes to: `log/garden/<station_name>`

`cmd/garden/<station_name>/<device_id>`
- device command in payload

Publishes to:
- `data/garden/<station_name>/<device_id>`
    - p_devices: `<device_state>`
    - a_devices: `<int>`
- `log/garden/<station_name>`
    - Responses to mqtt commands
    - Other logging updates

### Backend HTTP

Backend is a flask server with the following endpoints:


