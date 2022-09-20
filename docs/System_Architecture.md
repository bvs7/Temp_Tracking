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

Backend is a flask server.

The app will provide the following services:
- Current state info
    - Current state controls
    - Real time updates on this state
- Historical graphs
    - On time per day (bar chart)
    - Analog line graphs for soil moisture/temp/water level
- Scheduling.

What kind of service from the backend will this require?

- Current state of various systems
    - Device state for pump/solenoid valves
- Controls for current system?
    - Request a system to be turned on for X amount of time
        - (Turn on now, and schedule an off after X time)
- Graphs
    - Request blob of data for a graph?
        - On time per day for last X days
        - Analog values averaged every X time? Or just every 5 min value?
- Scheduler
    - Editing an ics calendar with events to turn on a system
    - Potentially just have a shared calendar that can be edited in other calendar apps?


Endpoints:

- `/current/[<device-name>/]`
    - GET request. Without device name, returns all devices. With device name, just that one

### OpenHAB Exec Bindings?

Another option is to use a smart home platform like OpenHAB. It has an option to have an "Exec" "Binding" which is pretty much a shell command that can be called upon a certain input. We could create a Garden Controller, which interfaces to OpenHAB this way.

Persistence for garden data:
everyUpdate and
"0 0 0 * * ?"
(every 0th sec of every 0th min of every 0th hour of every day, or the start of every day?)

Charts from persisted data?

Direct MQTT communication for commands?
Use a calendar to trigger events
Edit that calendar with another app?