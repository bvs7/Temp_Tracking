
## Loop tasks:
- Serial Commands
  - get/set settings
  - control device state
  - get device (status)
  - reset?
- Mqtt handling
- Poll sense pins for changes

## Global data:


## Modules:
- Main
  - Setup
    - Subscribe to topics
  - Loop
    - Poll sense pins
- Connection
  - setup
    - Initiate Serial1
  - loop Manages mqtt connection
    - reconnect
    - callback (recieve mqtt message)
  - connection settings can be written and reset (to defaults?)
  - puts/gets settings to/from EEPROM
  - connection settings
    - wifi ssid
    - wifi password
    - mqtt server address
    - mqtt server port?
    - mqtt client id
  - topics can be published to.
- SerialCommands
  - Only for debug/settings setup
  - setup
    - Initial Serial
  - loop
    - handle serial commands


## Connection:

Setup

Loop

mqtt_on_connect:
---
When MQTT connection is made, subscribe to a list of topics?



