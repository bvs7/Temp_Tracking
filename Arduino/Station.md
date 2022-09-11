
# Station

An Arduino station for interfacing with the real world via a wireless connection

Serial communication for setup/initial settings, and testing

## Spec

### Definitions
Use unknown bit to flag devices for publication. When device_state changes in main loop, data is published. So set unknown bit, and it will be unset upon main loop, prompting publish.

A devices are updated and published when their value is <0 while entering main loop. Request update by setting to -1. Automatically set to -2 every polling period

- Port devices (p_device)
    - device_state [0b0000_0ZSC]
        - if Z == 1: UNKNOWN else S/C:
        - 0/0 OFF+UNSET
        - 1/0 ON!UNSET
        - 0/1 OFF!SET
        - 1/1 ON+SET

### Root Commands
A command to the station as a whole:
- `hello`
    - Responds: `hello!`
- `[device_name]`
    - Publish device state to `data/irrigation/[station_name]/[device_name]`
    - Also get response of `[device_name]: [device_state]`
    - a_device is updated
- `[p_device_name] [ON/OFF]`
    - Set port ctrl pin to ON/OFF. An update in state will publish device state
    - Responds `[p_device_name] -> [ON/OFF]`
- `reboot`
    - Responds `Rebooting...` and sets watchdog timer 2s
- change connection settings saved immediately, not used until reboot? Responds with `[key]: [value]` for read or `[key] -> [value]` for write
    - `ssid [new ssid]`
    - `password [new password]`
    - `mqtt_server [new mqtt_server]`
    - `mqtt_port [new mqtt_port]`
    - `station_name [new station_name]`
- device config read: Responds with `[p_device_id].[key]: [value]`
    - `[p_device_id] [sense/ctrl/name]`
    - `[a_device_id] [input/trig/name]`
- device config write: Responds with `[p_device_id].[key] -> [value]` or `ERROR`
    - `[p_device_id] [sense/ctrl/name] [value]`
    - `[a_device_id] [input/trig/name] [value]`

#### Reboot
Reboot is necessary for... changing any 

### MQTT

Subscribed to `cmd/irrigation/[station_name]/#`

-`cmd/irrigation/[station_name]` sends a root command.
    - Response goes to: `log/irrigation/[station_name]`

`cmd/irrigation/[station_name]/[device_name]`
- No payload, publish device state (update a/s devices)
- payload ON/OFF for p_device: set p_device. change in state prompts publish

Publishes to:
- `data/irrigation/[station id]/[device_name]`
  - Specific updates about devices
    - p_devices: OFF+UNSET, OFF!SET, ON!UNSET, ON+SET (device state)
    - a_devices: [int]
- `log/irrigation/[station id]`
  - Generic station info: STARTUP, SHUTDOWN, errors, warn, info, etc.


## Architecture

- `settings.cpp` defines data structures
    - config struct
        - connection
            - char ssid[S]
            - char password[S]
            - char mqtt_server[S]
            - char station_name[N]
            - uint16_t mqtt_port
        - p_device_config[5]
            - pin sense (0xFF for unused)
            - pin ctrl
            - char name[N]
        - int a_device_poll_freq
        - a_device_config[8]
            - pin input (0xFF for unused)
            - pin trig
            - char name[N]
    - state
        - p_device_state[5]
            - byte device_state
        - a_device_state[8]
            - int device_value

- `utility.cpp` morse code LED debug, logging debug

- `connection.cpp` handles wifi and mqtt connections. 
    - Static connection data, copied on boot
    - `setup`
        - Starts communication with ESP8266
        - Copies connection data
        - Connects to wifi/mqtt
            - Subscribes to device name / #
    - `loop`
        - Loops mqtt client
        - if disconnected, set flag
        - if disconnected for X time, reconnect

    - `set_callback`: Sets callback up when device name published
    - `publish_data`: Publish data msg to topic_suffix.
    - `publish_log` : Publish msg to log topic

- `devices.cpp` handles devices.
    - `setup`
        - Sets input/output pins (recognizes conflicts via 32 bit bitwise?)
            - If conflict, set to 0xFF (disable device)
        - Set timing info for polling/last poll
    - `loop`
        - upon tick of a second
            - Check all p_devices
            - Check polling period
            - Check all a_devices
    - `p_id_from_name` Compare str to p device names and return index if found, else -1
    - `a_id_from_name` Compare str to a device names and return index if found, else -1
    - `[p/a]_changePin` Change given pin for given device id. If conflict, set to 0xFF
    - `[p/a]_flag` Flag the given device for a publication. P: set unknown bit. A: set value negative

- `commands.cpp` parse commands
    - `root_handle(char *input, Stream *resp)` Root command handling
        - `hello`
            - publish_log hello
        - `reboot`
            - publish_log reboot msg, set watchdog 2s?
        - Read/Set `ssid/password/mqtt_server/mqtt_port/station_name`
        - Check for `[device_id]` and reroute
        - Check for `[device_name]` and reroute
    - `[p/a]_device_config_handle(uint8_t device_idx, char *input, Stream *resp)`
        - read/write config setting
    - `[p/a]_device_state_handle(uint8_t device_idx, char *input, Stream *resp)`
        - Read state (prompt publish data) or set p_device ON/OFF

- `Station.ino`
    - `mqtt_callback`
        - Check for station topic, pass Loopback Stream LogOut to root_handle
        - Check for device name, pass to device_state_handle
        - Check Loopback Stream LogOut and publish_log
    - `serial_debug_callback`
        - 
        - pass Serial to root_handle
    - `setup`
        - Load Settings
        - Setup connection
        - set mqtt callback
        - Setup devices
        - publish_log startup msg
    - `loop`
        - Loop connection
        - loop devices
        - Pull Serial to Loopback Stream SerialIn. If '\n', serial_debug_callback
        


