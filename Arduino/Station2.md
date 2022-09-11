# Simplified Station Architecture

Main Ideas: Most firmware data is stored in flash, i.e., defines.

These can be routed to specific locations and kept there?

Idea: Kept in EEPROM?

Struct is only used to get offset, which grabs momentarily from EEPROM.

### Flash
- Simple
- No Space concerns
- No timing concerns
- **Must reflash to change?**

### EEPROM
- Complicated
- Need to allocate memory to pull
- Takes time to pull
- Can change in the field

- Certain parameters, like connection info, are only needed once
- Names for devices are needed all the time.
  - Don't do names? Just pins/states?

## Commands
- Serial and MQTT.
- Allocate buffers dynamically...
- Figure out Serial?
- Serial is optional!!! Can be turned off with defines, for testing.

Root commands are sent to the station mqtt topic `cmd/garden-station1` or something. These include:
- `hi` respond "Hi!"
- `reboot` reboot Arduino? Needs testing
- `ssid [value]` read ssid or write.
- `password` `mqtt_server` `mqtt_port` `station_name`
- station name is a global

device commands are sent to a device topic `cmd/garden-station1/P0`, or sent to root with `P0` command
- ` ` read
- `ON/OFF` set
- `sense` `ctrl` set pin, used on reset

