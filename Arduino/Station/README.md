# Homie Garden Station

[Homie](https://homieiot.github.io/specification/) is an MQTT convention for IoT devices. It is slightly heavier than the current (9/20/2022) Garden Station implementation. I am going to try implementing it efficiently to fit as much of it onto the Arduino pro mini as possible. We will see if that implementation is small enough for the Arduino Pro Mini

## Structure

- Device ID: `tank-0`
    - Node IDs: `p0`/`a0`/`update`
        - Properties: `ctrl,state`/`value`

So...

`homie/tank-0/update/now/set` -> send true to update a values now. not retained

`homie/tank-0/update/rate/set` -> send to set update rate in seconds

`homie/tank-0/p0/ctrl/set` -> set ctrl

`homie/tank-0/p0/state` -> state data published here (1 byte)

## Device

`$state` -> init/ready/(disconnected/sleeping/)lost/alert

`$homie` -> "4.0.0" for now?

`$name` -> "" (possibly, tank-0?)

`$nodes` -> "p0,p1,a0"

`$extensions` -> ""



`$state` -> ready

## Node

`$name` -> ""

`$type` -> ""

`$properties` -> "state" / "value"

## Properties

### Port

#### state
`$name` -> ""

`$datatype` -> integer

`$format` -> 0:3

#### ctrl
`$name` -> ""

`$datatype` -> "boolean"

`$settable` -> "true"

### Analog
#### value
`$name` -> ""

`$datatype` -> "integer"

`$unit` -> "mm" or "°F" or ""

`$format` -> Either "0:1023" or "0:4000" (4m)

### Update
#### now
`$name` -> ""

`$datatype` -> "boolean"

`$retained` -> "false"

#### rate
`$name` -> ""

`$datatype` -> "int"

`$format` -> "0:86400"

`$settable` -> "true"

## Subscription

`homie/tank-0/+/ctrl/set` (can be retained by controller for persistent state)

## Operation notes

Notably, most of this is just done in the setup phase. Once the device is connected, send all of the above properties. Only need to subscribe to a a certain subscription, then get the device # and set.



