

# Arduino Homie Garden Station Structure

Device Setup requires mqtt connection. Connect on startup

## devices

### `device_setup`

- publish `gemini-home/<station-name>/$homie`; `4.0.0`
- publish `gemini-home/<station-name>/$state`; `init`
- publish `gemini-home/<station-name>/$nodes`; `p0,p2`

- Node setup

- publish `gemini-home/<station-name>/$extensions`; ` `
- publish `gemini-home/<station-name>/$state`; `ready`

### `node_setup`
0        1         2         3         4
1234567890123456789012345678901234567890
gemini-home/tank-0/p0/ctrl/$datatype_

- publish `gemini-home/<station-id>/<node-id>/$name`; ` `
- publish `gemini-home/<station-id>/<node-id>/$type`; ` `
- publish `gemini-home/<station-id>/<node-id>/$properties`; `ctrl,state`


### `port_properties_setup`

- publish `gemini-home/<station-name>/<node-id>/ctrl/$name`; ` `
- publish `gemini-home/<station-name>/<node-id>/ctrl/$datatype`; `boolean`
- publish `gemini-home/<station-name>/<node-id>/ctrl/$settable`; `true`

- publish `gemini-home/<station-name>/<node-id>/state/$name`; ` `
- publish `gemini-home/<station-name>/<node-id>/state/$datatype`; `integer`
- publish `gemini-home/<station-name>/<node-id>/state/$format`; `0:3`

## EEPROM Settings

- Valid byte 0x000 1
- FW_VERSION str 0x010 16
- CATEGORY str 0x020 16
- STATION_NAME str 0x030

| Name | Type | len | addr |
|------|------|-----|------|
|VALID | byte | 1  | 0x000 |
|FW_VERSION|str|16 | 0x010 |
|CATEGORY| str |16 | 0x020 |
|STATION_NAME|str|16|0x030 |
|WIFI_SSID|str| 16 | 0x040 |
|WIFI_PASSWD|str|16| 0x050 |
|MQTT_SERVER|str|16| 0x060 |
|MQTT_PORT|int16| 2| 0x070 |
|POLL_INTERVAL|int16|2|0x080|
| **P** | ... | 16 | 0x090 |
| P_SENSE0 | byte | 1 | 0x090 |
| P_CTRL0  | byte | 1 | 0x091 |
| P_SENSE1 | byte | 1 | 0x092 |
| P_CTRL1  | byte | 1 | 0x093 |
| P_SENSE2 | byte | 1 | 0x094 |
| P_CTRL2  | byte | 1 | 0x095 |
| P_SENSE3 | byte | 1 | 0x096 |
| P_CTRL3  | byte | 1 | 0x097 |
| P_SENSE4 | byte | 1 | 0x098 |
| P_CTRL4  | byte | 1 | 0x099 |
| **A** | ... | 16 | 0x0A0 |
| A_SENSE0 | byte | 1 | 0x0A0 |
| A_CTLR0  | byte | 1 | 0x0A1 |
| A_SENSE1 | byte | 1 | 0x0A2 |
| A_CTLR1  | byte | 1 | 0x0A3 |
| A_SENSE2 | byte | 1 | 0x0A4 |
| A_CTLR2  | byte | 1 | 0x0A5 |
| A_SENSE3 | byte | 1 | 0x0A6 |
| A_CTLR3  | byte | 1 | 0x0A7 |
| A_SENSE4 | byte | 1 | 0x0A8 |
| A_CTLR4  | byte | 1 | 0x0A9 |
| A_SENSE5 | byte | 1 | 0x0AA |
| A_CTLR5  | byte | 1 | 0x0AB |
| A_SENSE6 | byte | 1 | 0x0AC |
| A_CTLR6  | byte | 1 | 0x0AD |
| A_SENSE7 | byte | 1 | 0x0AE |
| A_CTLR7  | byte | 1 | 0x0AF |

None of this is writable, but it is programmable.




