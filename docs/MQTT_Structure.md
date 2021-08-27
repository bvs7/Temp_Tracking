# Home Automation MQTT Structure

Structure based on ~~microcontrollers~~ or **conceptually**?


For valves and pumps, status is one of OVERRIDE/ON/OFF/UNPOWERED/NULL?:
NULL is used if device is alive but this actuator isn't implemented?

| Enable\Sense  | ON  | OFF  |
|---|---|---|
| ON  | ON  | UNPOWERED  |
| OFF | OVERRIDE  | OFF  |

status is returned over `/status` topic. Sent whenever an update is sent, or status requested
control can be sent as ON/OFF to `/control` topic. Triggers a status update usually.
status update can be requested by publishing "status" to `/valve/+` specific sensor?

## Garden

- `home/garden`
  - `/valve` (Irrigation Valves)
  - `/pump` (Irrigation Pumps)
  - `/main-tank` (Rainwater Tank)

### Valves

- `home/garden/valve`
  - `/sw-trees` 
    - `/control`, `/status`
  - `/beds1`
    - `/control`, `/status`
  - `/beds2`
    - `/control`, `/status`
  - `/compost`
    - `/control`, `/status`

### Pump

- `home/garden/pump`
  - `/main-tank`
    - `/control`, `/status`

### Main-Tank

- `home/garden/main-tank`
  - `/level-sensor`
    - `/status`

### Station

Have a heartbeat for stations? Publish every n minutes? n+1 minutes without heartbeat triggers error?

- `home/station`
  - `/main-tank` (commands sent here)
    - `/heartbeat`
  - `/beds`
    - `/heartbeat`

## Server? Central Control Pi? 

What gets published here? What subscribes?

- `home/station/base`
  - `/nest-handler` Nest interface client

