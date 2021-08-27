
ESP8266 Wifi Client

MQTT Client

## Tank Arduino

### Interfaces

#### main-tank Pump
- PUMP-CTRL line
- PUMP-SENSE line
- 12V
- GND

#### sw-trees Valve
- VALVE-CTRL output
- VALVE-SENSE input
- 12V
- GND

#### main-tank level-sensor
Distance sensor
- TRIG output
- ECHO input
- 3.3V
- GND

### MQTT Interface

#### Publishing

- `home/garden/valve/sw-trees/status`
  - Update of sw-tree valve status
- `home/garden/pump/main-tank/status`
  - Update of main-tank pump status
- `home/garden/main-tank/level-sensor/status`
  - Return level of main-tank sensor status?
- `home/station/main-tank/log`
  - General purpose logging and debug info
- `home/station/main-tank/heartbeat`
  - Published to regularly except when in sleep mode

#### Subscribing

- `home/garden/valve/sw-trees/control`
  - Set valve control line to ON or OFF
- `home/garden/pump/main-tank/control`
  - Set pump control line to ON or OFF
- `home/garden/valve/sw-trees`
  - Command to send status update
- `home/garden/pump/main-tank`
  - Command to send status update
- `home/garden/main-tank/level-sensor`
  - Command to send level-sensor status update
- `home/station/main-tank`
  - General station commands

## Garden Arduino

### Interfaces

#### Valves
3 valves, `/beds1`, `/beds2`, `/compost`
- VALVE-CTRL output
- VALVE-SENSE input
- 12V
- GND

#### Moisture Sensors

10 sensors
Make use of analog mux to interface
- SENSE
- 3.3V
- GND

#### Light Sensors

2 sensors, use different resistances in series/parallel to tune sensitivity in direct sunlight/shade?
- SENSE
- 3.3V
- GND

#### Temperature Sensors

3 sensors
- SENSE
- 3.3V
- GND

### MQTT Interface

#### Publishing

- `home/garden/valve/+/status`
  - Update of valve status for `beds1`, `beds2`, `compost` valves
- `home/garden/sensor/temperature/+/status`
  - Update of temperature status for 3 `tempN` sensors
- `home/garden/sensor/light/+/status`
  - Update of temperature status for 2 `lightN` sensors
- `home/garden/sensor/moisture/+/status`
  - Update of temperature status for 10 `moistureN` sensors
- `home/station/beds/log`
  - General purpose logging and debug info
- `home/station/beds/heartbeat`
  - Published to regularly except when in sleep mode

#### Subscribing

Can also specify sensors down to specific sensor

- `home/garden/valve/+/control`
  - Set valve control line to ON or OFF for `beds1`, `beds2`, `compost` valves
- `home/garden/valve/+`
  - Command to send status update for `beds1`, `beds2`, `compost` valves?
- `home/garden/sensor/temperature`
  - Command to read all temperature sensor values
- `home/garden/sensor/temperature/+`
  - Command to read specific `tempN` sensors
- `home/garden/sensor/light`
  - Command to read all light sensor values
- `home/garden/sensor/light/+`
  - Command to read specific `lightN` sensors
- `home/garden/sensor/moisture`
  - Command to read all moisture sensor values
- `home/garden/sensor/moisture/+`
  - Command to read specific `moistureN` sensors
- `home/station/beds`
  - General station commands