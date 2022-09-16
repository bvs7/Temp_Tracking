from paho.mqtt.client import Client as MQTTClient

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe("cmd/#")
    client.subscribe("log/#")
    client.subscribe("data/#")


def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))

if __name__ == "__main__":

    client = MQTTClient()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect("localhost", 1883, 60)

    client.loop_forever()