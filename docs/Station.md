
# Station

An Arduino station for interfacing with the real world via a wireless connection

Serial communication for setup/initial settings, and testing

# Spec


## Issue!

Arduino wireless capabilities seem to lack robustness. Test to determine what can be done.

### Wifi Tests

- Time it takes to connect
- Staying connected for a long time
- Arduino sleeping while connected
- Test various areas outside.
- Connection loss. What happens?

### TCP Tests

- Payload size within reason
- Send after a very long dry spell
- Continuous operation over days
- Graceful recovery when server is down?

### MQTT Tests

- Station publish consistency
- Station subscribe/receive consistency
- Stress test
- Long term testing

## Definitions
Use unknown bit to flag devices for publication. When device_state changes in main loop, data is published. So set unknown bit, and it will be unset upon main loop, prompting publish.

A devices are updated and published when their value is <0 while entering main loop. Request update by setting to -1. Automatically set to -2 every polling period

- Port devices (p_device)
    - device_state [0b0000_0ZSC]
        - if Z == 1: Requested update (*) else Sense/Ctrl:
        - 0/0 DISABLED
        - 1/0 FORCED!
        - 0/1 IDLE!
        - 1/1 ENABLED

## Root Commands
A command to the station as a whole:
- `hello` Responds: `Hello!`
- `reboot` Responds `Rebooting...` and sets watchdog timer 2s
- `sync` Resets seconds counter, synchronizing polling
- `mqtt [topic] [payload]` Test MQTT message
- `ssid/password/mqtt_server/mqtt_port/poll` Read Settings Value
    - `[value]` Set new Connection Value
- `version/name` Read FW Version or station name
- `[device_id]` Update device state, respond with `state`, `sense`, and `ctrl`
    - (p_device) `[ON/OFF]` Set control state, which will trigger device update
    - (p_device) `sense/ctrl` Read `sense/ctrl` pin
        - `[pin]` Set `sense/ctrl` pin, set up devices
    - (a_device) `input/trig` Read `input/trig` pin
        - `[pin]` Set `input/trig` pin, set up devices

 

## MQTT

Subscribed to `cmd/[station_name]/#`

-`cmd/[station_name]` sends a root command in payload
    - Response goes to: `log/[station_name]`

`cmd/[station_name]/[device_name]`
- root commands -> `[device_id]` payload

Publishes to:
- `data/[station id]/[device_name]`
    - p_devices: `[device_state]`
    - a_devices: `[int]`
- `log/[station id]`
    - Responses to mqtt commands
    - START


