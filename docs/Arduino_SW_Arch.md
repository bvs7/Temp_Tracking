
## Breakdown of data

- Station
  - mqtt communication
    - (wifi, mqtt publish handle, mqtt subscription callback)
    - ssid, pass, server, mqtt_id
  - devices
    - Control pin
    - Sense pin
    - State (OFF, UNPOWERED, OVERRIDE, ON)
    - has a name and a type of device (and an id?)
  - sensors (soil moisture, temp, light, distance)
    - Analog input

## Types of actions

- Change setting
  - station name
  - connection settings
  - device ctrl setting (has specific topics)
- Poll devices
  - update state -> report
  - heartbeat (MQTT?)

## Access Points

- Serial Debug
  - echo/liveness
  - [setting] [value] : set a setting
  - [setting] : get a setting
  - test_mqtt [topic] [payload] : test a mqtt injection
  - status : report all settings
- MQTT
  - station topic
    - same commands as serial debug
  - device topic
    - like a setting command

`cmd/irrigation/[station]/[device]`

`data/irrigation/[station]/[device]`

`error/irrigation/[station]`

settings struct...

- station name
- connection settings
  - ssid
  - pass
  - server
  - mqtt_id
- device[5]
  - ctrl_pin
  - sense_pin
  - state
