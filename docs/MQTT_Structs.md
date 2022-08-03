
## Prefix

Idea: Use a prefix topic to describe what type of message this is:
- cmd: A command for an endpoint topic, can contain arguments in payload
- data: Data from an endpoint, value in payload
- alert: Updates from a device, such as "invalid command", "just came online", "powering down intentionally", "heartbeat (I'm still online)"

Put it at the beginning of the topic structure, so a data collector could potentially subscribe to data/# to collect all data

## Mid Routing

Question: What infrastructure do we provide for future automation?

Idea: Keep it simple for now. Just have:

`[prefix]/irrigation/...`

We can change to something else later

## Endpoints

Idea: Have topics for both
- Processing devices (a specific Arduino, Raspberry Pi, PC, etc) or "stations"
- Actuators/sensors (type of device, and a name) or "devices"

So if we have two Arduinos, garden_station1, and tank_station2, they could have the following endpoints:

- `[prefix]/irrigation/garden_station1`
- `[prefix]/irrigation/garden_station1/valve/north_bed`
- `[prefix]/irrigation/garden_station1/valve/south_bed`
- `[prefix]/irrigation/garden_station1/moisture_sensor/north_bed0`
- `[prefix]/irrigation/tank_station2`
- `[prefix]/irrigation/tank_station2/valve/citrus_trees`
- `[prefix]/irrigation/tank_station2/pump/main_tank`

That allows for the following subscription:

- `data/irrigation/+/valve/+` will subscribe to all valves. Other things like this.

Overall, the structure is...

`[prefix]/irrigation/[station]/[device_type]/[device_name]`

## Commands

First, don't implement the device topic endpoints. Start with just the station.

Commands sent to station endpoints are treated like serial debug commands sent in. This makes testing easy (use serial debug). Later, advanced commands can be added if needed.

A device endpoint with an empty payload is a status request, which prompts the station to send out the data for that device. With a payload, if the payload parses correctly, is a set command, which will send a command to the device. If that prompts a change in the device, the data will be updated momentarily.
