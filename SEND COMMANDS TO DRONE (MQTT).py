import paho.mqtt.client as mqtt

client = mqtt.Client()
client.connect("192.168.1.10", 1883)

def send_command(cmd):
    client.publish("drone/control", cmd)
